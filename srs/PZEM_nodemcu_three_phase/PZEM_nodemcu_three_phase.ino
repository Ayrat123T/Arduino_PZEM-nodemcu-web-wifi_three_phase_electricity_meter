#include <Arduino.h>
#include <PZEM004Tv30.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
//#include <WiFiClient.h>
#include "index.h"

#define STASSID "Redmi_DF75"
#define STAPSK "51194303"
#define STASSID2 "Oasis"
#define STAPSK2 "9046711599"
#define ANALOG_PIN A0
#define CLOSE_WIN_FACTOR 10               // 1/X for narrowing window each side
#define ALARM_WT 1000
#define SCALE_TOP 2000

ESP8266WiFiMulti wifiMulti;
ESP8266WebServer server(80);

const char* ssid = STASSID;
const char* password = STAPSK;
const char* ssid2 = STASSID2;
const char* password2 = STAPSK2;

PZEM004Tv30 pzem1(D1, D2); // (RX,TX) connect to TX,RX of PZEM1
PZEM004Tv30 pzem2(D5, D6); // (RX,TX) connect to TX,RX of PZEM2
PZEM004Tv30 pzem3(D7, D0); // (RX,TX) connect to TX,RX of PZEM3

int KYimpNumSumm = 0;                          // текущее кол-во импульсов
int winHi = 0, winLo = 1024;                   // store histeresis limits here
int dataCur;                                   // temporary storage of current index_pzem_data
unsigned long microTimer, microSpent;          // stopWatch timer in microSec
boolean ledState, ledStateOld;                 // current logic state
float meterWattage = 0;                        // store current meterWattage
int constMeterImpsNum = 10000;                 // постояннная счётчика
int сurrentTransformerTransformationRatio = 1; // коэффициент трансформации трансформтора тока
float blincsPerHour = 0;                       // store how much blinks can fill 1 hour
int windowLo = 0;                              // bottom line of scale window in Wt
int windowHi = 1000;                           // top line of scale window in Wt

void SendPzemsValues();

void SetConstMeterImpsNum() {
  //отправляем ответ в формате json
  String constMeterImpsNumStr = server.arg("constMeterImpsNumVal");
  constMeterImpsNum = constMeterImpsNumStr.toInt();
  SendPzemsValues();
}

void SetСurrentTransformerTransformationRatio() {
  String СurrentTransformerTransformationRatioStr = server.arg("сurrentTransformerTransformationRatio");
  сurrentTransformerTransformationRatio = СurrentTransformerTransformationRatioStr.toInt();
  SendPzemsValues();
}

void SendPzemsValues() {

  float current = 0; //суммарный ток
  float power = 0; //суммарная мощность
  float energy = 0; //суммарная энергия
  String str_current = "0.0"; //суммарный ток
  String str_power = "0.0"; //суммарная мощность
  String str_energy = "0.0"; //суммарная энергия

  float voltage1 = pzem1.voltage();
  float current1;
  float power1;
  float energy1;
  String str_voltage1 = "0.0";
  String str_current1 = "0.0";
  String str_frequency1 = "0.0";
  String str_pf1 = "0.0";
  String str_power1 = "0.0";
  String str_energy1 = "0.0";
  if (!isnan(voltage1)) {
    str_voltage1 = String(voltage1);
    current1 = pzem1.current() * сurrentTransformerTransformationRatio;
    str_current1 = String(current1);
    current += current1;
    str_frequency1 = String(pzem1.frequency());
    str_pf1 = String(pzem1.pf());
    power1 = pzem1.power() / 1000 * сurrentTransformerTransformationRatio;
    str_power1 = String(power1);
    power += power1;
    energy1 = pzem1.energy() / 1000 * сurrentTransformerTransformationRatio;
    str_energy1 = String(energy1);
    energy += energy1;
  }

  float voltage2 = pzem2.voltage();
  float current2;
  float power2;
  float energy2;
  String str_voltage2 = "0.0";
  String str_current2 = "0.0";
  String str_frequency2 = "0.0";
  String str_pf2 = "0.0";
  String str_power2 = "0.0";
  String str_energy2 = "0.0";
  if (!isnan(voltage2)) {
    str_voltage2 = String(voltage2);
    current2 = pzem2.current() * сurrentTransformerTransformationRatio;
    str_current2 = String(current2);
    current += current2;
    str_frequency2 = String(pzem2.frequency());
    str_pf2 = String(pzem2.pf());
    power2 = pzem2.power() / 1000 * сurrentTransformerTransformationRatio;
    str_power2 = String(power2);
    power += power2;
    energy2 = pzem2.energy() / 1000 * сurrentTransformerTransformationRatio;
    str_energy2 = String(energy2);
    energy += energy2;
  }

  float voltage3 = pzem3.voltage();
  float current3;
  float power3;
  float energy3;
  String str_voltage3 = "0.0";
  String str_current3 = "0.0";
  String str_frequency3 = "0.0";
  String str_pf3 = "0.0";
  String str_power3 = "0.0";
  String str_energy3 = "0.0";
  if (!isnan(voltage3)) {
    str_voltage3 = String(voltage3);
    current3 = pzem3.current() * сurrentTransformerTransformationRatio;
    str_current3 = String(current3);
    current += current3;
    str_frequency3 = String(pzem3.frequency());
    str_pf3 = String(pzem3.pf());
    power3 = pzem3.power() / 1000 * сurrentTransformerTransformationRatio;
    str_power3 = String(power3);
    power += power3;
    energy3 = pzem3.energy() / 1000 * сurrentTransformerTransformationRatio;
    str_energy3 = String(energy3);
    energy += energy3;
  }


  if (current > 0) str_current = String(current);
  if (power > 0) str_power = String(power);
  if (energy > 0) str_energy = String(energy);

  String SMDAccuraty = "1000";
  if (power) SMDAccuraty = String((power - meterWattage) / power * 100);

  //отправляем ответ в формате json
  String json_pzem_data =
    "{\"voltages\":[";
      json_pzem_data += str_voltage1;
      json_pzem_data += ",";
      json_pzem_data += str_voltage2;
      json_pzem_data += ",";
      json_pzem_data += str_voltage3;
      json_pzem_data += "],";
    json_pzem_data += "\"currents\":[";
      json_pzem_data += str_current1;
      json_pzem_data += ",";
      json_pzem_data += str_current2;
      json_pzem_data += ",";
      json_pzem_data += str_current3;
      json_pzem_data += "],";
    json_pzem_data += "\"powers\":[";
      json_pzem_data += str_power1;
      json_pzem_data += ",";
      json_pzem_data += str_power2;
      json_pzem_data += ",";
      json_pzem_data += str_power3;
      json_pzem_data += "],";
    json_pzem_data += "\"energies\":[";
      json_pzem_data += str_energy1;
      json_pzem_data += ",";
      json_pzem_data += str_energy2;
      json_pzem_data += ",";
      json_pzem_data += str_energy3;
      json_pzem_data += "],";
    json_pzem_data += "\"frequencies\":[";
      json_pzem_data += str_frequency1;
      json_pzem_data += ",";
      json_pzem_data += str_frequency2;
      json_pzem_data += ",";
      json_pzem_data += str_frequency3;
      json_pzem_data += "],";
    json_pzem_data += "\"powerFactories\":[";
      json_pzem_data += str_pf1;
      json_pzem_data += ",";
      json_pzem_data += str_pf2;
      json_pzem_data += ",";
      json_pzem_data += str_pf3;
      json_pzem_data += "],";
    json_pzem_data += "\"FullValues\":{";
      json_pzem_data += "\"current\":";
      json_pzem_data += str_current;
      json_pzem_data += ",";
      json_pzem_data += "\"power\":";
      json_pzem_data += str_power;
      json_pzem_data += ",";
      json_pzem_data += "\"energy\":";
      json_pzem_data += str_energy;
      json_pzem_data += "},";
    json_pzem_data += "\"ResSMDValues\":{";
      json_pzem_data += "\"SMDimpPeriod\":";
      json_pzem_data += String(double(microSpent) /1000000);
      json_pzem_data += ",";
      json_pzem_data += "\"KYimpNumSumm\":";
      json_pzem_data += String(KYimpNumSumm);
      json_pzem_data += ",";
      json_pzem_data += "\"SMDpower\":";
      json_pzem_data += String(meterWattage);
      json_pzem_data += ",";
      json_pzem_data += "\"SMDAccuraty\":";
      json_pzem_data += SMDAccuraty;
      json_pzem_data += "}}";
  
  server.send(200, "application/json", json_pzem_data);
}

void Reset() {
  KYimpNumSumm = 0;
  winHi = 0, winLo = 1024;
  meterWattage = 0;
  constMeterImpsNum = 10000; 
  сurrentTransformerTransformationRatio = 1;
  blincsPerHour = 0;
  windowLo = 0;
  windowHi = 1000;
  if (pzem1.resetEnergy() &&
      pzem2.resetEnergy() &&
      pzem3.resetEnergy()) {
        server.send(200, "text/plane", "Energy in pzems has been reset");
  } else {
    server.send(200, "text/plane", "power reset error!");
  }
}

void setup() {
  Serial.begin(115200);

  // wifi connection section
  WiFi.mode(WIFI_OFF);        //Prevents reconnection issue (taking too long to connect)
  delay(1000);
  WiFi.mode(WIFI_STA);        //This line hides the viewing of ESP as wifi hotspot
  //WiFi.begin(ssid, password);     //Connect to your WiFi router
  //Serial.println("");
  wifiMulti.addAP(ssid, password);
  wifiMulti.addAP(ssid2, password2);
  Serial.println("");
  Serial.print("Connecting");
  // Wait for connection
  while (wifiMulti.run() != WL_CONNECTED) {
    digitalWrite(D4, LOW);
    delay(250);
    Serial.print(".");
    digitalWrite(D4, HIGH);
    delay(250);
  }

  //If connection successful show IP address in serial monitor
  Serial.println("");
  Serial.print("Connected to Network/SSID: ");
  Serial.println(WiFi.SSID());
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());  //IP address assigned to your ESP

// HTTP server setup
	server.on("/", handleRoot);
  server.on("/current_transformer_transformation_ratio", SetСurrentTransformerTransformationRatio);
  server.on("/const_meter_imps_num", SetConstMeterImpsNum);
  server.on("/pzem_values", SendPzemsValues);
  server.on("/reset", Reset);
	server.onNotFound(handle_NotFound);
	server.begin();
	Serial.println("HTTP server started");

  initWindow(); // инициализируем значения порогов аналогового сенсора
}

void loop() {
  while (wifiMulti.run() != WL_CONNECTED) {
    Serial.print(".");
  }
  delay(100);
  server.handleClient();

  dataCur = analogRead(ANALOG_PIN);               // запоминаем значение на сенсоре
  findAnalogWindow(dataCur);                      // расширяем окно, если значение выходит за его пределы
  ledStateOld = ledState;                         // сохраняем в буфер старое значение уровня сенсора
  checkLogic(dataCur);                            // оцениваем состояние сенсора и сохраняем его значение в ledState


  if (ledStateOld && !ledState) {                 // ИНДикатор только что загорелся
    // вычисление длины последнего импульса
    microSpent = micros() - microTimer;           // длина последнего импульса = текущее время - время прошлого перехода
    microTimer = micros();                        // запоминаем время этого перехода в таймер
    // вычисление длины последнего импульса
    blincsPerHour = 3600000000000 / microSpent;   // сколько таких импульсов такой длины поместилось бы в часе
    KYimpNumSumm++;
    meterWattage = (blincsPerHour / constMeterImpsNum) /100; // нагрузка (кВт) = кол-во таких импульсов в часе разделив на 6,4к имп (1кВт*ч) и умножить на 1000
    if (meterWattage > ALARM_WT) {                // если нагрузка больше сигнального порога
      windowLo = ALARM_WT;                        // сменить шкалу нагрузки на тревожную
      windowHi = SCALE_TOP;
    } else {                                      // если нагрузка ниже сигнального порога
      windowLo = 0;                               // установить шкалу нагрузки от 0 до уровня тревоги
      windowHi = ALARM_WT;
    }
  }

  if (!ledStateOld && ledState) {                 // ИНДикатор только что погас
    closeAnalogWindow();                          // ужимаем пороги окна сенсора, чтобы они хронически не росли.
  }
}

void handleRoot() {
 String html_index_h = webpage; //для обновления HTML/css/js в строку "webpage" в "index.h" запустите "front/htmlToH.exe"
 server.send(200, "text/html", html_index_h);
}

void handle_NotFound() {
    server.send(404, "text/plain", "Not found");
}

void initWindow() {
  unsigned long startTimer = millis() + 5000;
   Serial.print("initialization light window");
  while (millis() < startTimer) {
    dataCur = analogRead(ANALOG_PIN);
    findAnalogWindow(dataCur);
    Serial.print(". ");
  }
}

void findAnalogWindow(int analogData) {
  if (analogData > winHi) winHi = analogData;         // запомнить значение как верхнее, если оно выше него
  if (analogData < winLo) winLo = analogData;         // запомнить значение как нижнее, если оно ниже него
}

void checkLogic(int analogData) {
  if (winLo < winHi - 5) {
    int percCur = map(analogData, winLo, winHi, 0, 99); // переводим значение в проценты от шкалы
    if (percCur <= 33) ledState = 0;
    if (percCur >= 66) ledState = 1;
  }
}

void closeAnalogWindow() {
  if (winLo < winHi - 30) {
    int winDif = (winHi - winLo);                    // вычисляем ширину окна
    winHi = winHi - (winDif / CLOSE_WIN_FACTOR);      // вычитаем 1/10 ширины из верхнего порога
    winLo = winLo + (winDif / CLOSE_WIN_FACTOR);      // прибавляем 1/10 ширины к нижнему порогу
  }
}
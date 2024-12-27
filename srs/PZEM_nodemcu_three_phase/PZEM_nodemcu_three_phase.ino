#include <Arduino.h>
#include <PZEM004Tv30.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266WebServer.h>
#include <WiFiUdp.h>
#include <ArduinoJson.h>
#include <WiFiClient.h>
#include "index.h"

#define APSSID "SmartGridComMeterESPap" // Имя точки доступа, которую создаст ESP
#define STASSID "Redmi_DF75" //Точка доступа (или напишите свои), к которой подключится ESP
#define STAPSK "51194303" 
#define STASSID2 "Admin"
#define STAPSK2 "Admin" 
#define ANALOG_PIN A0
#define CLOSE_WIN_FACTOR 10               // 1/X for narrowing window each side
#define ALARM_WT 1000
#define SCALE_TOP 2000

ESP8266WiFiMulti wifiMulti;
ESP8266WebServer server(80);

const char *ap_ssid = APSSID;
const char* ssid = STASSID;
const char* password = STAPSK;
const char* ssid2 = STASSID2;
const char* password2 = STAPSK2;

PZEM004Tv30 pzem1(D1, D2); // (RX,TX) connect to TX,RX of PZEM1
PZEM004Tv30 pzem2(D5, D6); // (RX,TX) connect to TX,RX of PZEM2
PZEM004Tv30 pzem3(D7, D0); // (RX,TX) connect to TX,RX of PZEM3

  float current = 0; //суммарный ток
  float power = 0; //суммарная мощность
  float energy = 0; //суммарная энергия

  float voltage1 = 0;
  float current1= 0;
  float power1= 0;
  float energy1= 0;
  float frequency1= 0;
  float pf1= 0;

  float voltage2 = 0;
  float current2= 0;
  float power2= 0;
  float energy2= 0;
  float frequency2= 0;
  float pf2= 0;

  float voltage3 =0;
  float current3= 0;
  float power3= 0;
  float energy3= 0;
  float frequency3= 0;
  float pf3= 0;

int KYimpNumSumm = 0;                          // текущее кол-во импульсов
int winHi = 0, winLo = 1024;                   // store histeresis limits here
int dataCur;                                   // temporary storage of current index_pzem_data
unsigned long microTimer, microSpent;          // stopWatch timer in microSec
boolean ledState, ledStateOld;                 // current logic state
float meterWattage = 0;                        // store current meterWattage
int constMeterImpsNum = 1000;                 // постояннная счётчика
int сurrentTransformerTransformationRatio = 1; // коэффициент трансформации трансформтора тока
float blincsPerHour = 0;                       // store how much blinks can fill 1 hour
int windowLo = 0;                              // bottom line of scale window in Wt
int windowHi = 1000;                           // top line of scale window in Wt
int WtTokWtScale = 1000;

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

void SetPzem1Values() {
  voltage1 = 0;
  current1= 0;
  power1= 0;
  energy1= 0;
  frequency1= 0;
  pf1= 0;
  if (!isnan(voltage1 = pzem1.voltage())) {
    current1 = pzem1.current() * сurrentTransformerTransformationRatio;
    current += current1;
    frequency1 = pzem1.frequency();
    pf1 = pzem1.pf();
    power1 = pzem1.power() / WtTokWtScale * сurrentTransformerTransformationRatio;
    power += power1;
    energy1 = pzem1.energy() / WtTokWtScale * сurrentTransformerTransformationRatio;
    energy += energy1;
  }
}

void SetPzem2Values() {
  voltage2 = 0;
  current2= 0;
  power2= 0;
  energy2= 0;
  frequency2= 0;
  pf2= 0;
  if (!isnan(voltage2 = pzem2.voltage())) {
    current2 = pzem2.current() * сurrentTransformerTransformationRatio;
    current += current2;
    frequency2 = pzem2.frequency();
    pf2 = pzem2.pf();
    power2 = pzem2.power() / WtTokWtScale * сurrentTransformerTransformationRatio;
    power += power2;
    energy2 = pzem2.energy() / WtTokWtScale * сurrentTransformerTransformationRatio;
    energy += energy2;
  }
}

void SetPzem3Values() {
  voltage3 =0;
  current3= 0;
  power3= 0;
  energy3= 0;
  frequency3= 0;
  pf3= 0;
  if (!isnan(voltage3 = pzem3.voltage())) {
    current3 = pzem3.current() * сurrentTransformerTransformationRatio;
    current += current3;
    frequency3 = pzem3.frequency();
    pf3 = pzem3.pf();
    power3 = pzem3.power() / WtTokWtScale * сurrentTransformerTransformationRatio;
    power += power3;
    energy3 = pzem3.energy() / WtTokWtScale * сurrentTransformerTransformationRatio;
    energy += energy3;
  }
}

void SendPzemsValues() {
  current = 0;
  power = 0;
  energy = 0;

  SetPzem1Values();
  SetPzem2Values();
  SetPzem3Values();

  //float SMDAccuraty = 1000;
  /*if (power)*/ float SMDAccuraty = (power - meterWattage) / power * 100;

  //отправляем ответ в формате json
  JsonDocument doc; // Allocate the JSON document
  // Add an arrays
  JsonArray data = doc["voltages"].to<JsonArray>();
    data.add(voltage1);
    data.add(voltage2);
    data.add(voltage3);
  data = doc["currents"].to<JsonArray>();
    data.add(current1);
    data.add(current2);
    data.add(current3);
  data = doc["powers"].to<JsonArray>();
    data.add(power1);
    data.add(power2);
    data.add(power3);
  data = doc["energies"].to<JsonArray>();
    data.add(energy1);
    data.add(energy2);
    data.add(energy3);
  data = doc["frequencies"].to<JsonArray>();
    data.add(frequency1);
    data.add(frequency2);
    data.add(frequency3);
  data = doc["powerFactories"].to<JsonArray>();
    data.add(pf1);
    data.add(pf2);
    data.add(pf3);
  JsonObject FullValues =  doc["FullValues"].to<JsonObject>();
    FullValues["current"] = current;
    FullValues["power"] = power;
    FullValues["energy"] = energy;
  JsonObject ResSMDValues =  doc["ResSMDValues"].to<JsonObject>();
    ResSMDValues["SMDimpPeriod"] = double(microSpent) /1000000;
    ResSMDValues["KYimpNumSumm"] = KYimpNumSumm;
    ResSMDValues["SMDpower"] = meterWattage;
    ResSMDValues["SMDAccuraty"] = SMDAccuraty;

  server.send(200, "application/json", doc.as<String>());
}

void Reset() {
  KYimpNumSumm = 0;
  winHi = 0, winLo = 1024;
  meterWattage = 0;
  constMeterImpsNum = 1000; 
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

  WiFi.mode(WIFI_OFF); //Prevents reconnection issue (taking too long to connect)
  delay(500);

  // wifi add AP section
  /*WiFi.mode(WIFI_AP);
  Serial.println("Configuring access point...");
  WiFi.softAP(APSSID);         //Starting AccessPoint on given credential
  IPAddress myIP = WiFi.softAPIP();        //IP Address of our Esp8266 accesspoint(where we can host webpages, and see data)
  Serial.print("Access Point Name: "); 
  Serial.println(APSSID);
  Serial.print("Access Point IP address: ");
  Serial.println(myIP); // http://192.168.4.1/
  Serial.println("");
  delay(500);*/

  // wifi connection section
  /*WiFi.mode(WIFI_STA);
  wifiMulti.addAP(ssid, password);
  wifiMulti.addAP(ssid2, password2);
  Serial.println("");
  Serial.print("Connecting");
  // Wait for connection
  while (wifiMulti.run() != WL_CONNECTED) { 
    delay(250);
    Serial.print(".");
    delay(250);
  }
  //If connection successful show IP address in serial monitor
  Serial.println(""); 
  Serial.print("Connected to Network/SSID: ");
  Serial.println(WiFi.SSID());
  Serial.print("IP address: "); //http://192.168.31.146/
  Serial.println(WiFi.localIP());  //IP address assigned to your ESP
  delay(500);*/

  // wifi section
  WiFi.mode(WIFI_STA);// wifi connection section
  wifiMulti.addAP(ssid, password);
  wifiMulti.addAP(ssid2, password2);
  Serial.println("");
  Serial.print("Connecting");
  // Wait for connection
  unsigned long connectionTimer = millis() + 5000;
  while (millis() < connectionTimer && wifiMulti.run() != WL_CONNECTED) { 
    delay(500);
    Serial.print(".");
  }
  //If connection successful show IP address in serial monitor
  if (wifiMulti.run() == WL_CONNECTED) {
    Serial.println(""); 
    Serial.print("Connected to Network/SSID: ");
    Serial.println(WiFi.SSID());
    Serial.print("IP address: "); //http://192.168.31.146/
    Serial.println(WiFi.localIP());  //IP address assigned to your ESP
  } else {
    WiFi.mode(WIFI_AP); // wifi add AP section
    Serial.println("Configuring access point...");
    WiFi.softAP(APSSID);         //Starting AccessPoint on given credential
    IPAddress myIP = WiFi.softAPIP();        //IP Address of our Esp8266 accesspoint(where we can host webpages, and see data)
    Serial.print("Access Point Name: "); 
    Serial.println(APSSID);
    Serial.print("Access Point IP address: ");
    Serial.println(myIP); // http://192.168.4.1/
    Serial.println("");
  }
  delay(500);



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
  /*// если необходимо контролировать wifi подключение, например по светодиоду
  while (wifiMulti.run() != WL_CONNECTED) {
    Serial.print(".");
  }*/
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
    blincsPerHour = 3600000000 / microSpent;   // сколько таких импульсов такой длины поместилось бы в часе
    KYimpNumSumm++;
    meterWattage = (blincsPerHour / constMeterImpsNum); // нагрузка (кВт) = кол-во таких импульсов в часе разделить на имп за 1кВт*ч
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
    delay(1000);
    Serial.print(". ");
  }
  Serial.println("initialization light window complited");
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
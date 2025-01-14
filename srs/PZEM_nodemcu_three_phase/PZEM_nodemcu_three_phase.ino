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
#define STASSID "Redmi_DF75"            // Точка доступа (логин и пароль от wifi), к которой подключится ESP
#define STAPSK "51194303" 
//#define STASSID2 "Admin"
//#define STAPSK2 "Admin" 
#define ANALOG_PIN A0
#define CLOSE_WIN_FACTOR 10             // 1/CLOSE_WIN_FACTOR для сужения окна с каждой стороны

ESP8266WiFiMulti wifiMulti;
ESP8266WebServer server(80);

const char *ap_ssid = APSSID;
const char* ssid = STASSID;
const char* password = STAPSK;
//const char* ssid2 = STASSID2;
//const char* password2 = STAPSK2;

PZEM004Tv30 pzem1(D1, D2); // (RX,TX) подключиться к TX,RX PZEM1
PZEM004Tv30 pzem2(D5, D6); // (RX,TX) подключиться к TX,RX PZEM2
PZEM004Tv30 pzem3(D7, D0); // (RX,TX) подключиться к TX,RX PZEM3

  float current = 0; // суммарный ток
  float power = 0;   // суммарная мощность
  float energy = 0;  // суммарная энергия

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
int winHi = 0, winLo = 1024;                   // пределы гистерезиса
int dataCur;                                   // временное хранение текущих данных фоторезистора
unsigned long microTimer, microSpent;          // Стоп-таймер в микросекундах
boolean ledState, ledStateOld;                 // текущее логическое состояние фоторезистора
float meterWattage = 0;                        // текущая мощность счётчика
int constMeterImpsNum = 1000;                  // постояннная счётчика
float SMDAccuraty = 100;                       // Погрешность счётчика
int WtTokWtScale = 1000;                       // коэффициент перевода Вт в кВт
int сurrentTransformerTransformationRatio = 1; // коэффициент трансформации трансформтора тока

void SetPzem1Values() {
  voltage1 = 0;checkLedState();
  current1= 0;checkLedState();
  power1= 0;checkLedState();
  energy1= 0;checkLedState();
  frequency1= 0;checkLedState();
  pf1= 0;checkLedState();
  if (!isnan(voltage1 = pzem1.voltage())) {
    current1 = pzem1.current() * сurrentTransformerTransformationRatio;checkLedState();
    current += current1;checkLedState();
    frequency1 = pzem1.frequency();checkLedState();
    pf1 = pzem1.pf();checkLedState();
    power1 = pzem1.power() / WtTokWtScale * сurrentTransformerTransformationRatio;checkLedState();
    power += power1;checkLedState();
    energy1 = pzem1.energy() / WtTokWtScale * сurrentTransformerTransformationRatio;checkLedState();
    energy += energy1;checkLedState();
  }
}

void SetPzem2Values() {
  voltage2 = 0;checkLedState();
  current2= 0;checkLedState();
  power2= 0;checkLedState();
  energy2= 0;checkLedState();
  frequency2= 0;checkLedState();
  pf2= 0;checkLedState();
  if (!isnan(voltage2 = pzem2.voltage())) {
    current2 = pzem2.current() * сurrentTransformerTransformationRatio;checkLedState();
    current += current2;checkLedState();
    frequency2 = pzem2.frequency();checkLedState();
    pf2 = pzem2.pf();checkLedState();
    power2 = pzem2.power() / WtTokWtScale * сurrentTransformerTransformationRatio;checkLedState();
    power += power2;checkLedState();
    energy2 = pzem2.energy() / WtTokWtScale * сurrentTransformerTransformationRatio;checkLedState();
    energy += energy2;checkLedState();
  }
}

void SetPzem3Values() {
  voltage3 =0;checkLedState();
  current3= 0;checkLedState();
  power3= 0;checkLedState();
  energy3= 0;checkLedState();
  frequency3= 0;checkLedState();
  pf3= 0;checkLedState();
  if (!isnan(voltage3 = pzem3.voltage())) {
    current3 = pzem3.current() * сurrentTransformerTransformationRatio;checkLedState();
    current += current3;checkLedState();
    frequency3 = pzem3.frequency();checkLedState();
    pf3 = pzem3.pf();checkLedState();
    power3 = pzem3.power() / WtTokWtScale * сurrentTransformerTransformationRatio;checkLedState();
    power += power3;checkLedState();
    energy3 = pzem3.energy() / WtTokWtScale * сurrentTransformerTransformationRatio;checkLedState();
    energy += energy3;checkLedState();
  }
}

void SendPzemsValues() {
  yield();checkLedState();// костыльно решаем проблему многозадачности
  current = 0;checkLedState(); 
  power = 0;checkLedState();
  energy = 0;checkLedState();
  SetPzem1Values();checkLedState();
  SetPzem2Values();checkLedState();
  SetPzem3Values();checkLedState();

  // отправляем ответ в формате json
  JsonDocument doc;checkLedState(); // создаём JSON документ
  // Добавить массивы в JSON документ
  JsonArray data = doc["voltages"].to<JsonArray>();checkLedState();
    data.add(voltage1);checkLedState();
    data.add(voltage2);checkLedState();
    data.add(voltage3);checkLedState();
  data = doc["currents"].to<JsonArray>();checkLedState();
    data.add(current1);checkLedState();
    data.add(current2);checkLedState();
    data.add(current3);checkLedState();
  data = doc["powers"].to<JsonArray>();checkLedState();
    data.add(power1);checkLedState();
    data.add(power2);checkLedState();
    data.add(power3);checkLedState();
  data = doc["energies"].to<JsonArray>();checkLedState();
    data.add(energy1);checkLedState();
    data.add(energy2);checkLedState();
    data.add(energy3);checkLedState();
  data = doc["frequencies"].to<JsonArray>();checkLedState();
    data.add(frequency1);checkLedState();
    data.add(frequency2);checkLedState();
    data.add(frequency3);checkLedState();
  data = doc["powerFactories"].to<JsonArray>();checkLedState();
    data.add(pf1);checkLedState();
    data.add(pf2);checkLedState();
    data.add(pf3);checkLedState();
  // Добавить объекты в JSON документ
  JsonObject FullValues =  doc["FullValues"].to<JsonObject>();checkLedState();
    FullValues["current"] = current;checkLedState();
    FullValues["power"] = power;checkLedState();
    FullValues["energy"] = energy;checkLedState();
  JsonObject ResSMDValues =  doc["ResSMDValues"].to<JsonObject>();checkLedState();
    ResSMDValues["SMDimpPeriod"] = double(microSpent) /1000000;checkLedState();
    ResSMDValues["KYimpNumSumm"] = KYimpNumSumm;checkLedState();
    meterWattage = 3600000000 / microSpent / constMeterImpsNum;checkLedState(); // нагрузка (кВт) = кол-во таких импульсов в часе разделить на имп за 1кВт*ч
    ResSMDValues["SMDpower"] = meterWattage;checkLedState();
    if (power) SMDAccuraty = (power - meterWattage) / power * 100;checkLedState();
    ResSMDValues["SMDAccuraty"] = SMDAccuraty;checkLedState();
  server.send(200, "application/json", doc.as<String>());checkLedState();
  yield();checkLedState();
}

void SetConstMeterImpsNum() {
  String constMeterImpsNumStr = server.arg("constMeterImpsNumVal");
  constMeterImpsNum = constMeterImpsNumStr.toInt();
  SendPzemsValues();
}

void SetСurrentTransformerTransformationRatio() {
  String СurrentTransformerTransformationRatioStr = server.arg("сurrentTransformerTransformationRatio");
  сurrentTransformerTransformationRatio = СurrentTransformerTransformationRatioStr.toInt();
  SendPzemsValues();
}

void Reset() {
  KYimpNumSumm = 0;
  winHi = 0, winLo = 1024;
  meterWattage = 0;
  constMeterImpsNum = 1000; 
  сurrentTransformerTransformationRatio = 1;
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

  WiFi.mode(WIFI_OFF); // Предотвращает проблемы с повторным подключением (слишком долгое подключение)
  delay(500);

  /*// раздел добавления точки доступа wifi
  WiFi.mode(WIFI_AP);
  Serial.println("Configuring access point...");
  WiFi.softAP(APSSID);                     //Запуск AccessPoint с указанными учетными данными
  IPAddress myIP = WiFi.softAPIP();        //IP-адрес нашей точки доступа Esp8266 (где мы можем размещать веб-страницы и просматривать данные)
  Serial.print("Access Point Name: "); 
  Serial.println(APSSID);
  Serial.print("Access Point IP address: ");
  Serial.println(myIP); // http://192.168.4.1/
  Serial.println("");
  delay(500);

  // раздел подключения к Wi-Fi
  WiFi.mode(WIFI_STA);
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
  // Если подключение успешно, отображаем IP-адрес в последовательном мониторе
  Serial.println(""); 
  Serial.print("Connected to Network/SSID: ");
  Serial.println(WiFi.SSID());
  Serial.print("IP address: "); //http://192.168.31.146/
  Serial.println(WiFi.localIP());  // IP-адрес, назначенный ESP
  delay(500);*/

  /*раздел подключения к Wi-Fi*/
  WiFi.mode(WIFI_STA);
  wifiMulti.addAP(ssid, password);
  //wifiMulti.addAP(ssid2, password2);
  Serial.println("");
  Serial.print("Connecting");
  // Ожидаем подключения
  unsigned long connectionTimer = millis() + 5000;
  while (millis() < connectionTimer && wifiMulti.run() != WL_CONNECTED) { 
    if (wifiMulti.run() != WL_CONNECTED) {
      break;
    }
    delay(500);
    Serial.print(".");
  }
  //  Если подключение успешно, отображаем IP-адрес в последовательном мониторе
  if (wifiMulti.run() == WL_CONNECTED) {
    Serial.println(""); 
    Serial.print("Connected to Network/SSID: ");
    Serial.println(WiFi.SSID());
    Serial.print("IP address: "); //http://192.168.31.146/
    Serial.println(WiFi.localIP());  // IP-адрес, назначенный ESP
  } else {
    // раздел добавления точки доступа wifi
    WiFi.mode(WIFI_AP);
    Serial.println("Configuring access point...");
    WiFi.softAP(ap_ssid);                     //Запуск AccessPoint с указанными учетными данными
    Serial.print("Access Point Name: "); 
    Serial.println(ap_ssid);
    IPAddress myIP = WiFi.softAPIP();          //IP-адрес нашей точки доступа Esp8266 (где мы можем размещать веб-страницы и просматривать данные)
    Serial.print("Access Point IP address: ");
    Serial.println(myIP); // http://192.168.4.1/
    Serial.println("");
  }
  delay(500);

  // Настройка HTTP-сервера
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
  server.handleClient();
  delay(10);
  checkLedState();
}

void handleRoot() {
 String html_index_h = webpage; //для обновления HTML/css/js в строку "webpage" в "index.h" запустите "front/htmlToH.exe"
 server.send(200, "text/html", html_index_h);
}

void handle_NotFound() {
    server.send(404, "text/plain", "Not found");
}

void checkLedState() {
  dataCur = analogRead(ANALOG_PIN);                   // запоминаем значение на сенсоре
  findAnalogWindow(dataCur);                          // расширяем окно, если значение выходит за его пределы
  ledStateOld = ledState;                             // сохраняем в буфер старое значение уровня сенсора
  checkLogic(dataCur);                                // оцениваем состояние сенсора и сохраняем его значение в ledState

  if (ledStateOld && !ledState) {                     // ИНДикатор только что загорелся
    //Serial.print("led state has been changed, new blink period = ");
    // вычисление длины последнего импульса
    microSpent = micros() - microTimer;               // длина последнего импульса = текущее время - время прошлого перехода
    //Serial.println(double(microSpent) /1000000);
    microTimer = micros();                            // запоминаем время этого перехода в таймер
    // вычисление длины последнего импульса   
    KYimpNumSumm++;
  }
  if (!ledStateOld && ledState) { // ИНДикатор только что погас
    closeAnalogWindow();          // ужимаем пороги окна сенсора, чтобы они хронически не росли.
  }
}

void initWindow() {
  unsigned long startTimer = millis() + 5000;
  Serial.print("initialization light window");
  while (millis() < startTimer) {
    dataCur = analogRead(ANALOG_PIN);
    findAnalogWindow(dataCur);
    delay(1000);
    Serial.print(" . ");
  }
  Serial.println("initialization light window complited");
}

void findAnalogWindow(int analogData) {
  if (analogData > winHi) winHi = analogData; // запомнить значение как верхнее, если оно выше него
  if (analogData < winLo) winLo = analogData; // запомнить значение как нижнее, если оно ниже него
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
    int winDif = (winHi - winLo);                // вычисляем ширину окна
    winHi = winHi - (winDif / CLOSE_WIN_FACTOR); // вычитаем 1/10 ширины из верхнего порога
    winLo = winLo + (winDif / CLOSE_WIN_FACTOR); // прибавляем 1/10 ширины к нижнему порогу
  }
}
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266WebServer.h>
#include <WiFiUdp.h>
#include <ArduinoJson.h>
#include <WiFiClient.h>
#include "values.h"
#include "meterBlinkPeriodCalc.h"
#include "setValues.h"
#include "index.h"

ESP8266WiFiMulti wifiMulti;
ESP8266WebServer server(80);

void SendPzemsValues() {
  yield();checkLedState();// костыльно решаем проблему многозадачности
  current = 0;checkLedState();
  power = 0;
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
    ResSMDValues["KYimpNumSumm"] = KYimpNumSumm;checkLedState();
    ResSMDValues["SMDimpPeriod"] = meterBlinkPeriod;checkLedState();
    if (printSMDAccuracy) {
      ResSMDValues["SMDpower"] = meterWattage;checkLedState();
      if (power) ResSMDValues["SMDAccuracy"] = (power - meterWattage) / power * 100;checkLedState();
      printSMDAccuracy = false;
    }
    //if (!printSMDAccuracy) meterWattage = NULL;
    //ResSMDValues["SMDpower"] = meterWattage;checkLedState();
    /*if (power && meterWattage && printSMDAccuracy) {
      ResSMDValues["SMDAccuracy"] = (power - meterWattage) / power * 100;checkLedState();
    } /*{
      SMDAccuracy = (power - meterWattage) / power * 100;
    } else {
      SMDAccuracy = nullptr;
    }*/
    //ResSMDValues["SMDAccuracy"] = SMDAccuracy;checkLedState();
  server.send(200, "application/json", doc.as<String>());checkLedState();
  yield();checkLedState();
}

void SetConstMeterImpsNum() {
  String constMeterImpsNumStr = server.arg("constMeterImpsNumVal");
  constMeterImpsNum = constMeterImpsNumStr.toInt();
  server.send(200, "text/plane", "constMeterImpsNumVal has been set");
}

void SetCurrentTransformerTransformationRatio() {
  String CurrentTransformerTransformationRatioStr = server.arg("currentTransformerTransformationRatio");
  currentTransformerTransformationRatio = CurrentTransformerTransformationRatioStr.toInt();
  server.send(200, "text/plane", "currentTransformerTransformationRatio has been set");
}

void SetQueueSizeCalcMeterAccuracy() {
  queueSum = 0;
  printSMDAccuracy = false;
  String queueSizeCalcMeterAccuracyCheckStr = server.arg("queueSizeCalcMeterAccuracy");
  queueSize = queueSizeCalcMeterAccuracyCheckStr.toInt();
  server.send(200, "text/plane", "queueSizeCalcMeterAccuracy has been set");
}

void Reset() {
  resetCurrentValues();
  currentTransformerTransformationRatio = 1;
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
  // Ожидаем подключения в течении 5 секунд
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
  } else { //если подключения нет, создаём свою точку доступа
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
  server.on("/current_transformer_transformation_ratio", SetCurrentTransformerTransformationRatio);
  server.on("/const_meter_imps_num", SetConstMeterImpsNum);
  server.on("/queue_size_calc_meter_accuracy", SetQueueSizeCalcMeterAccuracy);
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
#include <Arduino.h>
#include <PZEM004Tv30.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

#include "index.h"

#define STASSID "Redmi_DF75"
#define STAPSK "51194303"
#define ANALOG_PIN A0
#define ALARM_WT 1000
#define SCALE_TOP 2000
#define HYST_BORDER 10                    // factor for set hysteresis
#define CLOSE_WIN_FACTOR 10               // 1/X for narrowing window each side
// #include <WiFiClient.h>

ESP8266WiFiMulti wifiMulti;
ESP8266WebServer server(80);

const char* ssid = STASSID;
const char* password = STAPSK;


PZEM004Tv30 pzem1(D1, D2); // (RX,TX)connect to TX,RX of PZEM1
PZEM004Tv30 pzem2(D5, D6);  // (RX,TX) connect to TX,RX of PZEM2
PZEM004Tv30 pzem3(D7, D0);  // (RX,TX) connect to TX,RX of PZEM3
#define ONE_WIRE_BUS D3 // ds18b20


int winHi = 0, winLo = 1024;              // store histeresis limits here
int dataCur;                              // temporary storage of current index_pzem_data
unsigned long microTimer, microSpent;     // stopWatch timer in microSec
boolean ledState, ledStateOld;            // current logic state
float wattage = 0;                        // store current wattage
int = impSMDсonst = 10000;                        // постояннная счётчика
float blincsPerHour;                      // store how much blinks can fill 1 hour
int windowLo = 0;                         // bottom line of scale window in Wt
int windowHi = 1000;                      // top line of scale window in Wt
float iCalc;                                // here i'll store calculated impedance (I=P/V)
int vData = 220;                          // gere i'll place voltage index_pzem_data or set it manually;

void GetPzemsValues() {
  float current1 = pzem1.current();
  float power1 = pzem1.power() / 1000;
  float energy1 = pzem1.energy() / 1000;

  float current2 = pzem2.current();
  float power2 = pzem2.power() / 1000;
  float energy2 = pzem2.energy() / 1000;

  float current3 = pzem3.current();
  float power3 = pzem3.power() / 1000;
  float energy3 = pzem3.energy() / 1000;

  float current = current1 + current2 + current3; 
  float power = power1 + power2 + power3; 
  float energy = energy1 + energy2 + energy3; 
 
  String json_pzem_data =
    "{ \"voltages\":[";
      + pzem1.voltage() + ','
      + pzem2.voltage() + ','
      + pzem3.voltage() + "],"
    + "\"currents\":["
      + current1 + ','
      + current2 + ','
      + current3 + "],"
    + "\"powers\":["
      + power1 + ','
      + power2 + ','
      + power3 + "],"
    + "\"energies\":["
      + energy1 + ','
      + energy2 + ','
      + energy3 + "],"
    + "\"frequencies\":["
      + pzem1.frequency(); + ','
      + pzem2.frequency(); + ','
      + pzem3.frequency(); + "],"
    + "\"powerFactories\":["
      + pzem1.pf(); + ','
      + pzem2.pf(); + ','
      + pzem3.pf(); + "],"
    + "\"FullValues\":{"
    + "current:" + current + ','
    + "power:" + power + ','
    + "energy:" + energy + "},"
    + "\"ResSMDValues\":{"
    + "SMDimpPeriod:" + double(microSpent) /1000000 + ','
    + "SMDpower:" + wattage + ','
    + "SMDAccuraty:" + (power - wattage) / power * 100 + "}}";
  
  server.send(200, "text/plane", json_pzem_data);
}

void handleRoot() {
 String html_index_h = webpage;
 server.send(200, "text/html", html_index_h);
}

void setup() {

	pinMode(D4, OUTPUT); // onboard led
  // power for ds18b20. esp8266 max pin current - 12 mA, 18b20 max current - 1.5 mA, so max - 8 sensors
  pinMode(D8, OUTPUT);
  digitalWrite(D8, HIGH);
  
  Serial.begin(115200);

// wifi connection section
  WiFi.mode(WIFI_OFF);        //Prevents reconnection issue (taking too long to connect)
  delay(1000);
  WiFi.mode(WIFI_STA);        //This line hides the viewing of ESP as wifi hotspot
  //WiFi.begin(ssid, password);     //Connect to your WiFi router
  //Serial.println("");
  wifiMulti.addAP(ssid, password);
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

// OTA section
  // Port defaults to 8266
  // ArduinoOTA.setPort(8266);

  // Hostname defaults to esp8266-[ChipID]
  // ArduinoOTA.setHostname("pzem_meter");
  char tmp[15];
  sprintf(tmp, "pzem_meter-%06x", ESP.getChipId());
  ArduinoOTA.setHostname(tmp);

  // No authentication by default
  ArduinoOTA.setPassword("ADMIN_PASSWORD");

  // Password can be set with it's md5 value as well
  // MD5(admin) = 21232f297a57a5a743894a0e4a801fc3
  // ArduinoOTA.setPasswordHash("21232f297a57a5a743894a0e4a801fc3");

  ArduinoOTA.onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH) {
      type = "sketch";
    } else { // U_FS
      type = "filesystem";
    }

    // NOTE: if updating FS this would be the place to unmount FS using FS.end()
    Serial.println("Start updating " + type);
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) {
      Serial.println("Auth Failed");
    } else if (error == OTA_BEGIN_ERROR) {
      Serial.println("Begin Failed");
    } else if (error == OTA_CONNECT_ERROR) {
      Serial.println("Connect Failed");
    } else if (error == OTA_RECEIVE_ERROR) {
      Serial.println("Receive Failed");
    } else if (error == OTA_END_ERROR) {
      Serial.println("End Failed");
    }
  });
  ArduinoOTA.begin();

  //If connection successful show IP address in serial monitor
  Serial.println("");
  Serial.print("Connected to Network/SSID: ");
  Serial.println(WiFi.SSID());
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());  //IP address assigned to your ESP

// HTTP server setup
	server.on("/", handle_OnConnect);
  server.on("/pzem_values)", GetPzemsValues);
	server.onNotFound(handle_NotFound);

	server.begin();
	Serial.println("HTTP server started");

  initWindow(); // инициализируем значения порогов аналогового сенсора
}

void loop() {
  while (wifiMulti.run() != WL_CONNECTED) {
    digitalWrite(D4, LOW);
    delay(250);
    Serial.print(".");
    digitalWrite(D4, HIGH);
    delay(250);
  }
  delay(100);
  server.handleClient();
  ArduinoOTA.handle();

  dataCur = analogRead(ANALOG_PIN);               // запоминаем значение на сенсоре
  findAnalogWindow(dataCur);                      // расширяем окно, если значение выходит за его пределы
  ledStateOld = ledState;                         // сохраняем в буфер старое значение уровня сенсора
  checkLogic(dataCur);                            // оцениваем состояние сенсора и сохраняем его значение в ledState
  if (ledState == !ledStateOld) {                 // ИНДикатор только что изменил своё состояние
    //Serial.print("LED changed");
    if (windowLo < 1000) {
      Serial.print(windowLo);
    } 
    else {
      Serial.print(String((int)windowLo / 1000) + " k ");
    } 
    Serial.print((int) wattage); Serial.print(" w ");
    Serial.print(String(iCalc)); Serial.print(" a ");
    if (windowHi < 1000) {
      Serial.print(windowHi);
    }
    else {
      Serial.print( String((int)windowHi / 1000) + " k ");
    }
  }

  if (ledStateOld && !ledState) {                 // ИНДикатор только что загорелся
    //Serial.println(": ON");
    //                                                 вычисление длины последнего импульса
    microSpent = micros() - microTimer;           //    длина последнего импульса = текущее время - время прошлого перехода
    microTimer = micros();                        //    запоминаем время этого перехода в таймер
    //                                                 вычисление длины последнего импульса
    blincsPerHour = 3600000000000 / microSpent;   //    сколько таких импульсов такой длины поместилось бы в часе
    wattage = (blincsPerHour / impSMDсonst) /100;             //    нагрузка (кВт) = кол-во таких импульсов в часе разделив на 6,4к имп (1кВт*ч) и умножить на 1000
    Serial.print(" Текущая нагрузка W = ");
    Serial.print(wattage);
    Serial.println(" kWt;");
    iCalc = wattage / vData;
    /*if (wattage > ALARM_WT) {                     //  если нагрузка больше сигнального порога
      windowLo = ALARM_WT;                        //    сменить шкалу нагрузки на тревожную
      windowHi = SCALE_TOP;
    } else {                                      //  если нагрузка ниже сигнального порога
      windowLo = 0;                               //    установить шкалу нагрузки от 0 до уровня тревоги
      windowHi = ALARM_WT;
    }*/
  }

  if (!ledStateOld && ledState) {                 // ИНДикатор только что погас
    closeAnalogWindow();                          // ужимаем пороги окна сенсора, чтобы они хронически не росли.
  }

}

void handle_NotFound() {
    server.send(404, "text/plain", "Not found");
}

void handle_OnConnect() {
  server.send(200, "text/html", GetHTML());
}

void initWindow() {
  unsigned long startTimer = millis() + 5000;
  while (millis() < startTimer) {
    dataCur = analogRead(ANALOG_PIN);
    findAnalogWindow(dataCur);
    Serial.print("winLo value:"); Serial.print(winLo);
    Serial.print(", dataCur value:"); Serial.print(dataCur);
    Serial.print(", winHi value:"); Serial.print(winHi);
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
    int winDif = (winHi - winLo) ;                    // вычисляем ширину окна
    winHi = winHi - (winDif / CLOSE_WIN_FACTOR);      // вычитаем 1/10 ширины из верхнего порога
    winLo = winLo + (winDif / CLOSE_WIN_FACTOR);      // прибавляем 1/10 ширины к нижнему порогу
  }
}
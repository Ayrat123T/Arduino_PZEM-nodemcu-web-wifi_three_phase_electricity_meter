#include <Arduino.h>
#include <PZEM004Tv30.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

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
int dataCur;                              // temporary storage of current data
unsigned long microTimer, microSpent;     // stopWatch timer in microSec
boolean ledState, ledStateOld;            // current logic state
float wattage = 0;                        // store current wattage
float blincsPerHour;                      // store how much blinks can fill 1 hour
int windowLo = 0;                         // bottom line of scale window in Wt
int windowHi = 1000;                      // top line of scale window in Wt
float iCalc;                                // here i'll store calculated impedance (I=P/V)
int vData = 220;                          // gere i'll place voltage data or set it manually;

String GetPzemsValues() {
  //main energy meter
  float voltage1 = pzem1.voltage();
  float current1 = pzem1.current();
  float power1 = pzem1.power() / 1000;
  float energy1 = pzem1.energy() / 1000;
  float frequency1 = pzem1.frequency();
  float pf1 = pzem1.pf();
  Serial.print("PZEM1: V1 = ");
  Serial.print(voltage1);
  Serial.print(" B; I1 = ");
  Serial.print(current1);
  Serial.print(" A; W1 = ");
  Serial.print(power1);
  Serial.print(" kWt; P1 = ");
  Serial.print(energy1);
  Serial.print(" kWt*h; F1 = ");
  Serial.print(frequency1);
  Serial.print(" Hz; cos(f)1 =  ");
  Serial.println(pf1);

  String data = "<li>V1 = ";
  data += voltage1;
  data += " B; I1 = ";
  data += current1;
  data += " A; W1 = ";
  data += power1;
  data += " kWt; P1 = ";
  data += energy1;
  data += " kWt*h; F1 = ";
  data += frequency1;
  data += " Hz; cos(f)1 =  ";
  data += pf1;

  //energymeter 2
  float voltage2 = pzem2.voltage();
  float current2 = pzem2.current();
  float power2 = pzem2.power() / 1000;
  float energy2 = pzem2.energy() / 1000;
  float frequency2 = pzem2.frequency();
  float pf2 = pzem2.pf();
  Serial.print("PZEM2: V2 = ");
  Serial.print(voltage2);
  Serial.print(" B; I2 = ");
  Serial.print(current2);
  Serial.print(" A; W2 = ");
  Serial.print(power2);
  Serial.print(" kWt; P2 = ");
  Serial.print(energy2);
  Serial.print(" kWt*h; F2 = ");
  Serial.print(frequency2);
  Serial.print(" Hz; cos(f)2 = ");
  Serial.println(pf2);
  data += "</li><li>V2 = ";
  data += voltage2;
  data += " B; I2 = ";
  data += current2;
  data += " A; W2 = ";
  data += power2;
  data += " kWt; P2 = ";
  data += energy2;
  data += " kWt*h; F2 = ";
  data += frequency2;
  data += " Hz; cos(f)2 =  ";
  data += pf2;

  // energy meter 3
  float voltage3 = pzem3.voltage();
  float current3 = pzem3.current();
  float power3 = pzem3.power() /1000;
  float energy3 = pzem3.energy() /1000;
  float frequency3 = pzem3.frequency();
  float pf3 = pzem3.pf();
  Serial.print("PZEM3: V3 = ");
  Serial.print(voltage3);
  Serial.print(" B; I3 = ");
  Serial.print(current3);
  Serial.print(" A; W3 = ");
  Serial.print(power3);
  Serial.print(" kWt; P3 = ");
  Serial.print(energy3);
  Serial.print(" kWt*h; F3 = ");
  Serial.print(frequency3);
  Serial.print(" Hz; cos(f)3 =  ");
  Serial.println(pf3);
  data += "</li><li>V3 = ";
  data += voltage3;
  data += " B; I3 = ";
  data += current3;
  data += " A; W3 = ";
  data += power3;
  data += " kWt; P3 = ";
  data += energy3;
  data += " kWt*h; F3 = ";
  data += frequency3;
  data += " Hz; cos(f)3 =  ";
  data += pf3;

  float power = power1 + power2 +power3; 

  Serial.print(" I = ");
  Serial.print(current1 + current2 +current3);
  Serial.print(" A; W = ");
  Serial.print(power);
  Serial.print(" kWt; P3 = ");
  Serial.print(energy1 + energy2 + energy3);
  Serial.println(" kWt*h;");
  data += "</li><li> I = ";
  data += current1 + current2 +current3;
  data += " A; W = ";
  data += power;
  data += " kWt; P = ";
  data += energy1 + energy2 + energy3;
  data += " kWt*h; </li>";

  data += "<li> Текущая нагрузка W = ";
  data += wattage;
  data += " kWt; </li>";
  data += "<li> Длина последнего импульса t = ";
  data += double(microSpent) /1000000;
  data += " s; </li>";

  data += "<li> Погрешность = ";
  data += (power - wattage) / power * 100;
  data += " %; </li>";
  
  return data;
}

String SendHTML() {
  String ptr = "<!DOCTYPE html> <html lang=\"ru\">\n";
  ptr +="<head><meta http-equiv=refresh content=30><meta http-equiv=\"Content-type\" content=\"text/html; charset=utf-8\"><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr +="<title>Энергомонитор PZEM</title>\n";
  ptr += "<small>via AP: ";
  ptr += WiFi.SSID();
  ptr += "</small><br>";
  ptr +="<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
  ptr +="body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;} h3 {color: #444444;margin-bottom: 50px;}\n";
  ptr +="p {font-size: 14px;color: #888;margin-bottom: 10px;}\n";
  ptr +="</style>\n";
  ptr +="</head>\n";
  ptr +="<body>\n";
  ptr +="<h1>PZEM Энергомонитор ESP8266 Веб сервер</h1>\n";
  ptr +="<p>";
  ptr += GetPzemsValues();
  ptr +="</p>";
  ptr +="</body>\n";
  ptr +="</html>\n";
  return ptr;
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
	//server.on("/pzem1", handle_pzem1);
  //server.on("/pzem2", handle_pzem2);
  //server.on("/pzem3", handle_pzem3);
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
    wattage = (blincsPerHour / 10000) /100;             //    нагрузка (кВт) = кол-во таких импульсов в часе разделив на 6,4к имп (1кВт*ч) и умножить на 1000
      Serial.print(" Текущая нагрузка W = ");
  Serial.print(wattage);
  Serial.println(" kWt;");
    //digitalWrite(LED_PIN, HIGH);                  // зажигаем индикатор считываемых импульсов
    iCalc = wattage / vData;
    if (wattage > ALARM_WT) {                     //  если нагрузка больше сигнального порога
      //digitalWrite(BUZ_PIN, HIGH);                //    включить пищалку
      windowLo = ALARM_WT;                        //    сменить шкалу нагрузки на тревожную
      windowHi = SCALE_TOP;
    } else {                                      //  если нагрузка ниже сигнального порога
      windowLo = 0;                               //    установить шкалу нагрузки от 0 до уровня тревоги
      windowHi = ALARM_WT;
    }
  }

  if (!ledStateOld && ledState) {                 // ИНДикатор только что погас
    //Serial.println(": OFF");
    //digitalWrite(LED_PIN, LOW);                   // гасим светодиод индикации импульсов
    //digitalWrite(BUZ_PIN, LOW);                   // выключаем пищалку на всякий случай (она дублирует мигание индикатора)
    closeAnalogWindow();                          // ужимаем пороги окна сенсора, чтобы они хронически не росли.
  }

}

void handle_NotFound() {
    server.send(404, "text/plain", "Not found");
}

void handle_OnConnect() {
  server.send(200, "text/html", SendHTML());
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
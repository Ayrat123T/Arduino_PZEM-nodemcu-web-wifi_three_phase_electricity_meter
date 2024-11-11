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

// #include <WiFiClient.h>

ESP8266WiFiMulti wifiMulti;
ESP8266WebServer server(80);

const char* ssid = STASSID;
const char* password = STAPSK;


PZEM004Tv30 pzem1(D1, D2); // (RX,TX)connect to TX,RX of PZEM1
PZEM004Tv30 pzem2(D5, D6);  // (RX,TX) connect to TX,RX of PZEM2
PZEM004Tv30 pzem3(D7, D0);  // (RX,TX) connect to TX,RX of PZEM3
#define ONE_WIRE_BUS D3 // ds18b20

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
	server.on("/pzem", handle_pzem);
	server.onNotFound(handle_NotFound);

	server.begin();
	Serial.println("HTTP server started");

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
}

void handle_NotFound() {
    server.send(404, "text/plain", "Not found");
}

void handle_OnConnect() {
	String index = "<html><meta http-equiv=refresh content=30><title>PZEM station</title>";
	index += "<a href=/temp>TEMP</a><br><small>via AP: ";
  index += WiFi.SSID();
  index += "</small><br>";
	index += "<iframe src=/pzem frameborder=0 height=5% width=100%></iframe>";
  index += "<iframe src=/temp  frameborder=0 height=20% width=100%></iframe>";
	index += "</html>";
    server.send(200, "text/html", index);
}

void handle_pzem() {
  //main energy meter
  float voltage1 = pzem1.voltage();
  float current1 = pzem1.current();
  float power1 = pzem1.power();
  float energy1 = pzem1.energy();
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
  String data1 = "V1 = ";
  data1 += voltage1;
  data1 += " B; I1 = ";
  data1 += current1;
  data1 += " A; W1 = ";
  data1 += power1;
  data1 += " kWt; P1 = ";
  data1 += energy1;
  data1 += " kWt*h; F1 = ";
  data1 += frequency1;
  data1 += " Hz; cos(f)1 =  ";
  data1 += pf1;

  //energymeter 2
  float voltage2 = pzem2.voltage();
  float current2 = pzem2.current();
  float power2 = pzem2.power();
  float energy2 = pzem2.energy();
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
  String data2 = "V2 = ";
  data2 += voltage2;
  data2 += " B; I2 = ";
  data2 += current2;
  data2 += " A; W2 = ";
  data2 += power2;
  data2 += " kWt; P2 = ";
  data2 += energy2;
  data2 += " kWt*h; F2 = ";
  data2 += frequency2;
  data2 += " Hz; cos(f)2 =  ";
  data2 += pf2;

  // energy meter 3
  float voltage3 = pzem3.voltage();
  float current3 = pzem3.current();
  float power3 = pzem3.power();
  float energy3 = pzem3.energy();
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
  String data3 = "V3 = ";
  data3 += voltage3;
  data3 += " B; I3 = ";
  data3 += current3;
  data3 += " A; W3 = ";
  data3 += power3;
  data3 += " kWt; P3 = ";
  data3 += energy3;
  data3 += " kWt*h; F3 = ";
  data3 += frequency3;
  data3 += " Hz; cos(f)3 =  ";
  data3 += pf3;
  server.send(200, "text/html", data1 + data2 + data3);
}

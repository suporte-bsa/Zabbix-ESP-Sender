#include <ESP8266ZabbixSender.h>
#include <ESP8266WiFi.h>
#include <math.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h> 
ESP8266ZabbixSender zSender;

/* WiFi settings */
//String ssid = "Brasilia 211  ";
//String pass = "riodejaneiro";

/* Zabbix server setting */
#define SERVERADDR 192, 168, 0, 56 // Zabbix server Address
#define ZABBIXPORT 10051      // Zabbix erver Port
#define ZABBIXAGHOST "Teste"  // Zabbix item's host name

boolean checkConnection();

void setup() {
  Serial.begin(115200);
//WiFi.mode(WIFI_STA);
//WiFi.disconnect();
//delay(100);
//WiFi.begin(ssid.c_str(), pass.c_str());
  WiFiManager wifiManager;
  wifiManager.autoConnect("AutoConnectAP");
  Serial.println("connected...yeey :)");
  while (!checkConnection()) {
  }
  zSender.Init(IPAddress(SERVERADDR), ZABBIXPORT, ZABBIXAGHOST); // Init zabbix server information
}

void loop() {
  
  static int counter1 = 1;
  static int counter2 = 1;
  double seno = sin(counter1);
  double cosseno = cos(counter2);
  checkConnection();                // Check wifi connection
  zSender.ClearItem();              // Clear ZabbixSender's item list
  zSender.AddItem("counter1", (double)seno); // Exmaple value of zabbix trapper item
  zSender.AddItem("counter2", (double)cosseno); // Exmaple value of zabbix trapper item
  if (zSender.Send() == EXIT_SUCCESS) {     // Send zabbix items
    Serial.println("ZABBIX SEND: OK");
  } else {
    Serial.println("ZABBIX SEND: NG");
  }
  counter1 += 1;
  counter2 += 1;
  delay(1000); // wait 1sec
}

boolean checkConnection() {
  int count = 0;
  Serial.print("Waiting for Wi-Fi connection");
  while (count < 300) {
    if (WiFi.status() == WL_CONNECTED) {
      Serial.println();
      Serial.println("Connected!");
      return (true);
    }
    delay(500);
    Serial.print(".");
    count++;
  }
  Serial.println("Timed out.");
  return false;
}

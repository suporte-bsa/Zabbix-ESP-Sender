#include <ESP8266ZabbixSender.h>
#include <ESP8266WiFi.h>
#include <math.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h> 
ESP8266ZabbixSender zSender;

/* Zabbix server setting */
#define SERVERADDR 192, 168, 0, 56 // Zabbix server Address
#define ZABBIXPORT 10051      // Zabbix erver Port
#define ZABBIXAGHOST "Teste"  // Zabbix item's host name

boolean checkConnection();

void setup() {
  Serial.begin(115200);
  WiFiManager wifiManager;
  wifiManager.autoConnect("AutoConnectAP");
  Serial.println("connected...yeey :)");
  while (!checkConnection()) {
  }
  zSender.Init(IPAddress(SERVERADDR), ZABBIXPORT, ZABBIXAGHOST); // Init zabbix server information
}
  int counter1 = 1;
  int counter2 = 1;
  
void loop() { 
  double seno = sin(counter1);
  double cosseno = cos(counter2);
  checkConnection();                
  zSender.ClearItem();              
  zSender.AddItem("counter1", seno); 
  zSender.AddItem("counter2", cosseno);
  if (zSender.Send() == EXIT_SUCCESS) {     
    Serial.println("ZABBIX SEND: OK");
  } else {
    Serial.println("ZABBIX SEND: NG");
  }
  counter1 += 1;
  counter2 += 1;
  Serial.println(counter1);
  delay(1000); 
}

boolean checkConnection() {
  int count = 0;
  Serial.print("Waiting for Wi-Fi connection");
  while (count < 300) {
    if (WiFi.status() == WL_CONNECTED) {
      Serial.println();
      Serial.println("Connected!");
      return true;
    }
    delay(500);
    Serial.print(".");
    count++;
  }
  Serial.println("Timed out.");
  return false;
}

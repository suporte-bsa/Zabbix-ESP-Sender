#include <ESP8266ZabbixSender.h>
#include <ESP8266WiFi.h>
#include <math.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h> 
#define INTERVAL 1000
ESP8266ZabbixSender zSender;
int counter1 = 1, counter2 = 1;
  
/* Zabbix server setting */
#define SERVERADDR 192, 168, 0, 56 // Zabbix server Address
#define ZABBIXPORT 10051      // Zabbix erver Port
#define ZABBIXAGHOST "Teste"  // Zabbix item's host name

void setup() {
  Serial.begin(115200);
  WiFiManager wm;
  wm.autoConnect("AutoConnectAP");
  Serial.println("connected...yeey :)");
  zSender.Init(IPAddress(SERVERADDR), ZABBIXPORT, ZABBIXAGHOST); // Init zabbix server information
}
  
void sendItems(float seno, float cosseno) {
  zSender.ClearItem();              
  zSender.AddItem("counter1", seno); 
  zSender.AddItem("counter2", cosseno);
  zSender.Send() == EXIT_SUCCESS ? Serial.println("ZABBIX SEND: OK") : Serial.println("ZABBIX SEND: NG");
}

void loop() { 
  double seno = sin(counter1);
  double cosseno = cos(counter2);
  boolean isConnected = checkConnection();
  isConnected ? sendItems(seno, cosseno) : setup();
  counter1 += 1, counter2 += 1;
  delay(INTERVAL); 
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
    delay(INTERVAL/2);
    Serial.print(".");
    count++;
  }
  Serial.println("Timed out.");
  return false;
}

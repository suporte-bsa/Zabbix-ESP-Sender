/* Bicliotecas */
#include <ESP8266ZabbixSender.h>
#include <ESP8266WiFi.h>
#include <DHT.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>

/* DEFS */
#define INTERVAL 1000
ESP8266ZabbixSender zSender;
#define DHTPIN D1
#define DHTTYPE DHT22 
DHT dht(DHTPIN, DHTTYPE);


float umidade = 0;
float temperatura = 0;
long previousMillis = 0;
long interval_sensor = 2000; //Tempo em ms de execução

/* Configuração do Server */
#define SERVERADDR 192, 168, 25, 12 // Zabbix server Address
#define ZABBIXPORT 10051      // Zabbix erver Port
#define ZABBIXAGHOST "ESP01"  // Zabbix item's host name

void setup() {
  Serial.begin(115200);
  Serial.println("DHT11 sensor testing");
  //Setup do DHT
  dht.begin();
  /* Setup WIFI */
  WiFi.disconnect(); //utilizado caso a ESP já tenha conectado em alguma rede antes
  WiFiManager wm;
  wm.autoConnect("AutoConnectAP");
  Serial.println("connected...yeey :)");
  /* Setup Sender */
  zSender.Init(IPAddress(SERVERADDR), ZABBIXPORT, ZABBIXAGHOST); // Init zabbix server information
}
  
  /* Envio para o Server */
void sendItems(float umidade, float temperatura) {
  zSender.ClearItem();              
  zSender.AddItem("umidade", umidade); //key1 
  zSender.AddItem("temperatura", temperatura); //key2
  zSender.Send() == EXIT_SUCCESS ? Serial.println("ZABBIX SEND: OK") : Serial.println("ZABBIX SEND: NG");
}

void loop() { 
  
  //Leitura de tempo atual
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis > interval_sensor){
    previousMillis = currentMillis;
    //Chama as funções de leitura do sensor e envio de dados a cada dois segundos:
    umidade, temperatura = readSensor();
    boolean isConnected = checkConnection();
    isConnected ? sendItems(umidade, temperatura) : setup();
   }
}


float readSensor(){
    // Lê os dados de umidade e temperatura
    umidade = dht.readHumidity();
    temperatura= dht.readTemperature();
    Serial.print("Celsius = ");
    Serial.println(temperatura);
    //Print degree symbol
    Serial.write(176); 
    Serial.println("C");
    Serial.print("Umidade =");
    Serial.println(umidade);
}

/* Check de conexão Wifi */

boolean checkConnection() {
  int count = 0;
  Serial.print("Waiting for Wi-Fi connection");
  while (count < 300) {
    if (WiFi.status() == WL_CONNECTED) {
      Serial.println();
      Serial.println("Connected!");
      Serial.print("IP address: ");
      Serial.println(WiFi.localIP());
      Serial.println();
      return true;
    }
    delay(INTERVAL/2);
    Serial.print(".");
    count++;
  }
  Serial.println("Timed out.");
  return false;
}
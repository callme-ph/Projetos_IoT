/*
Código faz o uso de sensores DHT11 para leitura de umidade e temperatura do ambiente e faz a transmissão dos dados coletados para o Adafruit IO por meio do protocolo MQTT
*/

#include <WiFi.h>
#include <WiFiClient.h>
#include <PubSubClient.h>

#include <Adafruit_Sensor.h>

#include <DHT.h>
#include <DHT_U.h>

#include "DHT.h"

#define LEDWIFI 2
#define DHTPINE 4  // Sensor de entrada
#define DHTPINS 32 // Sensor de saída
#define SOILPIN 27 // Sensor de umidade do solo

#define DHTTYPE DHT11   // DHT 11

//#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
//#define DHTTYPE DHT21   // DHT 21 (AM2301)

DHT dhtEntrada(DHTPINE, DHTTYPE);
DHT dhtSaida(DHTPINS,DHTTYPE);

WiFiClient wifi_client;
PubSubClient mqtt_client(wifi_client);

const char* wifi_ssid = "Redmi9T";
const char* wifi_password = "pedroshow";
int wifi_timeout = 100000;

int umidade_solo, soil_analog;

const char* mqtt_broker = "io.adafruit.com";
const int mqtt_port = 1883;
int mqtt_timeout = 10000;

const char* mqtt_usernameAdafruitIO = "PedroSantos_";
const char* mqtt_keyAdafruitIO = "aio_kXMf78Vwusl7Z8h3N79GYioAIWQu";

int valor = 0;

void setup() {
  Serial.begin(115200);
  
  WiFi.mode(WIFI_STA); //"station mode": permite o ESP32 ser um cliente da rede WiFi
  WiFi.begin(wifi_ssid, wifi_password);
  connectWiFi();
  
  mqtt_client.setServer(mqtt_broker, mqtt_port);

  dhtEntrada.begin();
  dhtSaida.begin();
  
  pinMode(LEDWIFI, OUTPUT);

}

void loop() {

  /* CÓDIGO DO SENSOR DE UMIDADE DO SOLO. NÃO FUNCIONOU :(
  soil_analog = analogRead(SOILPIN);
  Serial.print("Valor lido no sensor:");
  Serial.println(soil_analog);
  umidade_solo = ( 100 - ( (soil_analog/4095.00) * 100 ) );
  Serial.print("Umidade do solo lida = ");
  Serial.print(umidade_solo);  // Print Temperature on the serial window
  Serial.println("%");
  delay(1000);
  */

  float he = dhtEntrada.readHumidity();
  // Read temperature as Celsius (the default)
  float te = dhtEntrada.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float fe = dhtEntrada.readTemperature(true);

  // Check if any reads failed and exit early (to try again).
  if (isnan(he) || isnan(te) || isnan(fe)) {
    Serial.println(F("Erro ao ler o sensor DHT de entrada!"));
    return;
  }
  
  // Compute heat index in Fahrenheit (the default) FAHRENHEIT DA ENTRADA
  float hife = dhtEntrada.computeHeatIndex(fe, he);
  // Compute heat index in Celsius (isFahreheit = false) CELSIUS DA ENTRADA
  float hice = dhtEntrada.computeHeatIndex(te, he, false);



  // Sensor de saída leitura da temperatura final
  float hs = dhtSaida.readHumidity();
  // Read temperature as Celsius (the default)
  float ts = dhtSaida.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float fs = dhtSaida.readTemperature(true); 

  // Check if any reads failed and exit early (to try again).
  if (isnan(hs) || isnan(ts) || isnan(fs)) {
    Serial.println(F("Erro ao ler o sensor DHT de saída!"));
    return;
  }

  // Compute heat index in Fahrenheit (the default) FAHRENHEIT DA SAIDA
  float hifs = dhtSaida.computeHeatIndex(fs, hs);
  // Compute heat index in Celsius (isFahreheit = false) CELSIUS DA SAIDA
  float hics = dhtSaida.computeHeatIndex(ts, hs, false);
  
  /*//Impressão do sensor INICIAL/ENTRADA
  Serial.print(F("Umidade Inicial: "));
  Serial.print(he);
  Serial.print(F("%  Temperatura Inicial: "));
  Serial.print(te);
  Serial.println(F("°C "));*/
  
  /*
  //Impressão do sensor FINAL/SAIDA
  Serial.print(F("Umidade Final : "));
  Serial.print(hs);
  Serial.print(F("%  Temperatura Final: "));
  Serial.print(ts);
  Serial.println(F("°C ")); 
  */
  
  //Calculo da variação da temperatura
  float varTempC = ts-te;
  if (varTempC < 0){
    varTempC = varTempC*-1;
      }
  float varHum = hs-he;
  if (varHum < 0){
    varHum = varHum*-1;
      }

  /*
  //Impressão da VARIAÇÃO DA UMIDADE e TEMPERATURA EM C
  Serial.print(F("Variação de Umidade: "));
  Serial.print(varHum);
  Serial.print(F("%  Variação de Temperatura: "));
  Serial.print(varTempC);
  Serial.println(F("°C "));
  */
  
      
  if (!mqtt_client.connected()) { // Se MQTT não estiver conectado
    connectMQTT();
  }

  if (mqtt_client.connected()) {

    mqtt_client.publish("PedroSantos_/feeds/temperatura-de-entrada", String(te).c_str());
    Serial.println("Publicou a temperatura de entrada: " + String(te));
    
    mqtt_client.publish("PedroSantos_/feeds/ts", String(ts).c_str());
    Serial.println("Publicou a temperatura de saída: " + String(ts));
    
    mqtt_client.publish("PedroSantos_/feeds/he", String(he).c_str());
    Serial.println("Publicou a umidade de entrada: " + String(he));
    
    mqtt_client.publish("PedroSantos_/feeds/hs", String(hs).c_str());
    Serial.println("Publicou a umidade de saída: " + String(hs));

    
    mqtt_client.publish("PedroSantos_/feeds/var_u", String(varHum).c_str());
    Serial.println("Publicou a variação de umidade: " + String(varHum));
      
    mqtt_client.publish("PedroSantos_/feeds/var_t", String(varTempC).c_str());
    Serial.println("Publicou a variação de temperatura: " + String(varTempC));

    /*mqtt_client.publish("PedroSantos_/feeds/soil", String(umidade_solo).c_str());
    Serial.println("Publicou a variação de temperatura: " + String(umidade_solo));*/
    
    delay(60000);
    mqtt_client.loop();
  }
}

void connectWiFi() {
  Serial.print("Conectando à rede WiFi .. ");

  unsigned long tempoInicial = millis();
  while (WiFi.status() != WL_CONNECTED && (millis() - tempoInicial < wifi_timeout)) {
    Serial.print(".");
    delay(100);
  }
  Serial.println();

  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Conexão com WiFi falhou!");
  } else {
    digitalWrite(LEDWIFI, HIGH);
    Serial.print("Conectado com o IP: ");
    Serial.println(WiFi.localIP());
    
  }
}

void connectMQTT() {
  unsigned long tempoInicial = millis();
  while (!mqtt_client.connected() && (millis() - tempoInicial < mqtt_timeout)) {
    if (WiFi.status() != WL_CONNECTED) {
      connectWiFi();
    }
    Serial.print("Conectando ao MQTT Broker..");

    if (mqtt_client.connect("ESP32Client", mqtt_usernameAdafruitIO, mqtt_keyAdafruitIO)) {
      Serial.println();
      Serial.print("Conectado ao broker MQTT!");
    } else {
      Serial.println();
      Serial.print("Conexão com o broker MQTT falhou!");
      delay(500);
    }
  }
  Serial.println();
}

#include <WiFi.h>
#include <WiFiClient.h>
#include <PubSubClient.h>

WiFiClient wifi_client;
PubSubClient mqtt_client(wifi_client);

const char* wifi_ssid = "Redmi9T";
const char* wifi_password = "pedroshow";
int wifi_timeout = 100000;

const char* mqtt_broker = "io.adafruit.com";
const int mqtt_port = 1883;
int mqtt_timeout = 10000;

const char* mqtt_usernameAdafruitIO = "PedroSantos_"; // userid = ID do usário do site io.adafruit.com
const char* mqtt_keyAdafruitIO = "aio_GHYQ90WsVr359txQGt4nRgLXH34V"; // key de acesso do usuário.

int valor = 0;

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA); //"station mode": permite o ESP32 ser um cliente da rede WiFi
  WiFi.begin(wifi_ssid, wifi_password);
  connectWiFi();
  mqtt_client.setServer(mqtt_broker, mqtt_port);
}

void loop() {
  if (!mqtt_client.connected()) { // Se MQTT não estiver conectado
    connectMQTT();
  }

  if (mqtt_client.connected()) {
    valor = random(1, 100);

    mqtt_client.publish("PedroSantos_/feeds/temperatura-de-entrada", String(valor).c_str());
    Serial.println("Publicou a média da temperatura de entrada dos dados armazenados: " + String(valor));
    
    mqtt_client.publish("PedroSantos_/feeds/ts", String(valor).c_str());
    Serial.println("Publicou a média da temperatura de saída dos dados armazenados: " + String(valor));
    
    mqtt_client.publish("PedroSantos_/feeds/he", String(valor).c_str());
    Serial.println("Publicou a méida da umidade de entrada dos dados armazenados: " + String(valor));

    mqtt_client.publish("PedroSantos_/feeds/hs", String(valor).c_str());
    Serial.println("Publicou a média da umidade de saída dos dados armazenados: " + String(valor));

    mqtt_client.publish("PedroSantos_/feeds/var_u", String(valor).c_str());
    Serial.println("Publicou a média da variação da umidade dos dados armazenados: " + String(valor));

    mqtt_client.publish("PedroSantos_/feeds/var_t", String(valor).c_str());
    Serial.println("Publicou a média da variação da temperatura dos dados armazenados: " + String(valor));

    mqtt_client.publish("PedroSantos_/feeds/soil", String(valor).c_str());
    Serial.println("Publicou a média da variação da umidade do solo dos dados armazenados: " + String(valor));
    
    delay(10000);
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

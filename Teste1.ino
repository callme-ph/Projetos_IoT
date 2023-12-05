#include <WiFi.h>
#include <WiFiClient.h>
#include <PubSubClient.h>
#include <Adafruit_Sensor.h>

#include <DHT.h>
#include <DHT_U.h>

#include <FS.h>
#include "SPIFFS.h"

#define LEDWIFI 2
#define DHTPINE 4  // Sensor de entrada
#define DHTPINS 32 // Sensor de saída
#define SOILPIN 35 // Sensor de umidade do solo

#define DHTTYPE DHT11   // DHT 11

DHT dhtEntrada(DHTPINE, DHTTYPE);
DHT dhtSaida(DHTPINS, DHTTYPE);

WiFiClient wifi_client;
PubSubClient mqtt_client(wifi_client);

const char* wifi_ssid = "Redmi9T";
const char* wifi_password = "pedroshow";
int wifi_timeout = 100000;

int umidade_solo, soil_analog;

const char* mqtt_broker = "io.adafruit.com";
const int mqtt_port = 1883;
int mqtt_timeout = 30000; // Ajustado para um valor maior

const char* mqtt_usernameAdafruitIO = "PedroSantos_";
const char* mqtt_keyAdafruitIO = "aio_zEhf00CCBBWfddeys2BonOgHl6PS";

float SomaHE, SomaHS, SomaTS, SomaTE, SomaSoil, SomaVarU, SomaVarT;

int leitura = 0;

String s_str;
String str;
String s;
String values;

int publicarDados(String path) {

  Serial.println("Função publicarDados iniciada.");
  Serial.println("Reading file");

  File rFile = SPIFFS.open(path, "r");//r+ leitura e escrita

  if (!rFile) {
    Serial.println("Erro ao abrir arquivo!");
  }
  else {
    if ( rFile.size() != 0 ) {
      Serial.print("-- Lendo arquivo ");
      Serial.print(path);
      Serial.println("  --");

      while (rFile.position() < rFile.size())
      {
        s_str = rFile.readStringUntil('\n');
        s_str.trim();
        Serial.print("Linha lida pela função publicar dados ( String s_str ): ");
        Serial.println(s_str);

        char str_copy[s_str.length()];
        char value[] = "00.00";
        char resultado[6] = "00.00";

        Serial.print("Vetor value inicial: ");
        Serial.println(value);
        Serial.print("Tamanho do vetor value: ");
        Serial.println(sizeof(value));

        strcpy(str_copy, s_str.c_str()); // Copia os valores da String s_str que é do tipo const char* para str_copy que é um vetor do tipo char.

        Serial.print("Vetor do tipo char formado a partir dos valores recebidos da função readFile: ");
        Serial.println(str_copy);

        Serial.print("length da string s_str: ");
        Serial.println(s_str.length());
        Serial.print("sizeof da string s_str: ");
        Serial.println(sizeof(s_str));
        Serial.print("tamanho da string str_copy: ");
        Serial.println(sizeof(str_copy));

        int cont = 0;

        for (int i = 0; i < s_str.length();) {
          int pos = s_str.indexOf("-", i);

          if (pos == -1) {
            Serial.println("    !!!!  Erro no indexOf da função publicarDados  !!!!");
            break;
          }

          Serial.print("i: ");
          Serial.println(i);
          Serial.print(" pos: ");
          Serial.println(pos);

          sprintf(value, "%.5s", (s_str.substring(i, pos)).c_str());
          Serial.print("value sprintf: ");
          Serial.println(value);
          i = pos + 1;

          float pub = atof(value); // Passando os dados do array value (char) para result (float) usando a função atof
          Serial.print("pub: ");
          Serial.println(pub);

          switch (cont)
          {
            case 0:
              mqtt_client.publish("PedroSantos_/feeds/temperatura-de-entrada", String(pub).c_str());
              Serial.println("Publicou a média da temperatura de entrada dos dados armazenados: " + String(pub));
              cont++;
              delay(1000);
              break;

            case 1:
              mqtt_client.publish("PedroSantos_/feeds/ts", String(pub).c_str());
              Serial.println("Publicou a média da temperatura de saída dos dados armazenados: " + String(pub));
              cont++;
              delay(1000);
              break;


            case 2:
              mqtt_client.publish("PedroSantos_/feeds/he", String(pub).c_str());
              Serial.println("Publicou a méida da umidade de entrada dos dados armazenados: " + String(pub));
              cont++;
              delay(1000);
              break;

            case 3:
              mqtt_client.publish("PedroSantos_/feeds/hs", String(pub).c_str());
              Serial.println("Publicou a média da umidade de saída dos dados armazenados: " + String(pub));
              cont++;
              delay(1000);
              break;

            case 4:
              mqtt_client.publish("PedroSantos_/feeds/var_u", String(pub).c_str());
              Serial.println("Publicou a média da variação da umidade dos dados armazenados: " + String(pub));
              cont++;
              delay(1000);
              break;

            case 5:
              mqtt_client.publish("PedroSantos_/feeds/var_t", String(pub).c_str());
              Serial.println("Publicou a média da variação da temperatura dos dados armazenados: " + String(pub));
              cont++;
              delay(1000);
              break;

            case 6:
              mqtt_client.publish("PedroSantos_/feeds/soil", String(pub).c_str());
              Serial.println("Publicou a média da variação da umidade do solo dos dados armazenados: " + String(pub));
              cont++;
              delay(1000);
              break;
          }
        }

        delay(60000);
        mqtt_client.loop();
      }
      
      Serial.println("Leitura do arquivo finalizada.");
      
    } else {
      
      return 0;
      
    }
    /*Serial.println("Limpando logs.");
    formatFile();
    Serial.println("Arquivo de logs limpo.");*/
  }
}


void writeFile(String state, String path) { //escreve conteúdo em um arquivo
  File rFile = SPIFFS.open(path, "w");//  w para truncar - apagar e reescrever
  if (!rFile) {
    Serial.println("Erro ao abrir arquivo!");
    return;
  }
  else {
    Serial.print("Tamanho atual do aqruivo: ");
    Serial.println(rFile.size());
    rFile.println(state);
    Serial.print("Dados gravados: ");
    Serial.println(state);
  }
  rFile.close();
}

void formatFile() {
  Serial.println("Formantando SPIFFS");
  SPIFFS.format();
  Serial.println("Formatou SPIFFS");
}

void openFS(void) {
  if (!SPIFFS.begin()) {
    Serial.println("\nErro ao abrir o sistema de arquivos");
  }
  else {
    Serial.println("\nSistema de arquivos aberto com sucesso!");
  }
}

void setup() {
  Serial.begin(115200);

  //formatFile();
  openFS();

  WiFi.mode(WIFI_STA);
  WiFi.begin(wifi_ssid, wifi_password);
  connectWiFi();

  mqtt_client.setServer(mqtt_broker, mqtt_port);

  dhtEntrada.begin();
  dhtSaida.begin();

  pinMode(LEDWIFI, OUTPUT);

  SomaHE = SomaHS = SomaTS = SomaTE = SomaSoil = SomaVarU = SomaVarT = 0;
}

void loop() {

  connectMQTT();

  leitura++;
  Serial.print("Leitura: ");
  Serial.println(leitura);

  float he = dhtEntrada.readHumidity();
  float te = dhtEntrada.readTemperature();
  float fe = dhtEntrada.readTemperature(true);

  if (isnan(he) || isnan(te) || isnan(fe)) {
    Serial.println(F("Erro ao ler o sensor DHT de entrada!"));
    return;
  }

  float hs = dhtSaida.readHumidity();
  float ts = dhtSaida.readTemperature();
  float fs = dhtSaida.readTemperature(true);

  if (isnan(hs) || isnan(ts) || isnan(fs)) {
    Serial.println(F("Erro ao ler o sensor DHT de saída!"));
    return;
  }

  float varTempC = ts - te;
  if (varTempC < 0) {
    varTempC = varTempC * -1;
  }
  float varHum = hs - he;
  if (varHum < 0) {
    varHum = varHum * -1;
  }

  // CÓDIGO DO SENSOR DE UMIDADE DO SOLO
  soil_analog = analogRead(SOILPIN);
  Serial.print("Valor lido no sensor de umidade do solo: ");
  Serial.println(soil_analog);
  umidade_solo = 100 - ((soil_analog / 4095.00) * 100);

  if (!mqtt_client.connected()) {

    SomaTE = SomaTE + te;
    SomaTS = SomaTS + ts;
    SomaHE = SomaHE + he;
    SomaHS = SomaHS + hs;
    SomaVarU = SomaVarU + varHum;
    SomaVarT = SomaVarT + varTempC;
    SomaSoil = SomaSoil + umidade_solo;

    str = String(SomaTE / leitura) + "-" + String(SomaTS / leitura) + "-" + String(SomaHE / leitura) + "-" + String(SomaHS / leitura)
          + "-" + String(SomaVarU / leitura) + "-" + String(SomaVarT / leitura) + "-" + String(SomaSoil / leitura) + "-";

    Serial.print("String in Log: ");
    Serial.println(str);

    writeFile(str, "/logsDados.txt");

    delay(20000);

  }

  if (mqtt_client.connected()) {

    publicarDados("/logsDados.txt");
    leitura = 0;
    delay(20000);
    return;
  }

  mqtt_client.loop();
}

void connectWiFi() {
  Serial.print("Conectando à rede WiFi .. ");

  unsigned long tempoInicial = millis();
  while (WiFi.status() != WL_CONNECTED && (millis() - tempoInicial < wifi_timeout)) {
    //Serial.print(".");
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
      Serial.print("Conexão com o broker MQTT falhou! Código de erro: ");
      Serial.println(mqtt_client.state());
      delay(500);
    }
  }
  Serial.println();
}

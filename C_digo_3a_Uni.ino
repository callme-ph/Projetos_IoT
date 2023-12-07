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
const char* mqtt_keyAdafruitIO = "aio_GHYQ90WsVr359txQGt4nRgLXH34V";

float SomaHE, SomaHS, SomaTS, SomaTE, SomaSoil, SomaVarU, SomaVarT;

int leitura = 0;

String s_str;
String str;
String s;
String values;

int separarDados(String path) {

  float pub_te, pub_ts, pub_he, pub_hs, pub_var_t, pub_var_u, pub_soil;
  pub_te = pub_ts = pub_he = pub_hs = pub_var_t = pub_var_u = pub_soil = 0;

  Serial.println("Função separarDados iniciada.");
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
        Serial.print("Linha lida pela função separar dados ( String s_str ): ");
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
          Serial.println("for");

          int pos = s_str.indexOf("-", i);

          if (pos == -1) {
            Serial.println("    !!!!  Erro no indexOf da função separarDados  !!!!");
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
              pub_te = pub;
              cont++;
              //delay(5000);
              break;

            case 1:
              pub_ts = pub;
              cont++;
              //delay(5000);
              break;


            case 2:
   
              pub_he = pub;
              cont++;
              //(5000);
              break;

            case 3:
              pub_hs = pub;
              cont++;
              //delay(5000);
              break;

            case 4:
              pub_var_u = pub;
              cont++;
              //delay(5000);
              break;

            case 5:
              pub_var_t = pub;
              cont++;
              //delay(5000);
              break;

            case 6:
              pub_soil = pub;
              cont++;
              //delay(5000);
              break;
          }
        }
      }

      Serial.println("Separação dos dados finalizada.");
      /*Serial.println("Limpando logs.");
        formatFile();
        Serial.println("Arquivo de logs limpo.");*/

      publicarDados(pub_te, pub_ts, pub_he, pub_hs, pub_var_t, pub_var_u, pub_soil);

    } else {
      return 0;

    }
  }
}

int publicarDados(float pub_te, float pub_ts, float pub_he, float pub_hs, float pub_var_t, float pub_var_u, float pub_soil) {

  //connectMQTT();

  if ( mqtt_client.connected() ) {
    mqtt_client.loop();

    mqtt_client.publish("PedroSantos_/feeds/temperatura-de-entrada", String(pub_te).c_str());
    Serial.println("Publicou a média da temperatura de entrada dos dados armazenados: " + String(pub_te));

    mqtt_client.publish("PedroSantos_/feeds/ts", String(pub_ts).c_str());
    Serial.println("Publicou a média da temperatura de saída dos dados armazenados: " + String(pub_ts));

    mqtt_client.publish("PedroSantos_/feeds/he", String(pub_he).c_str());
    Serial.println("Publicou a méida da umidade de entrada dos dados armazenados: " + String(pub_he));

    mqtt_client.publish("PedroSantos_/feeds/hs", String(pub_hs).c_str());
    Serial.println("Publicou a média da umidade de saída dos dados armazenados: " + String(pub_hs));

    mqtt_client.publish("PedroSantos_/feeds/var_u", String(pub_var_u).c_str());
    Serial.println("Publicou a média da variação da umidade dos dados armazenados: " + String(pub_var_u));

    mqtt_client.publish("PedroSantos_/feeds/var_t", String(pub_var_t).c_str());
    Serial.println("Publicou a média da variação da temperatura dos dados armazenados: " + String(pub_var_t));

    mqtt_client.publish("PedroSantos_/feeds/soil", String(pub_soil).c_str());
    Serial.println("Publicou a média da variação da umidade do solo dos dados armazenados: " + String(pub_soil));

    delay(30000);

  } else {
    connectMQTT();
    publicarDados( pub_te,  pub_ts,  pub_he,  pub_hs,  pub_var_t,  pub_var_u,  pub_soil);
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
  //connectWiFi();

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

    /*if( publicarDados("/logsDados.txt") == 0 ){

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
      publicarDados("/logsDados.txt");
      leitura = 0;
      delay(5000);
      } else {*/

    separarDados("/logsDados.txt");
    leitura = 0;
    delay(5000);

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
      digitalWrite(LEDWIFI, HIGH);
    } else {
      Serial.println();
      Serial.print("Conexão com o broker MQTT falhou! Código de erro: ");
      Serial.println(mqtt_client.state());
      delay(500);
    }
  }
  Serial.println();
}

#include <FS.h>
#include "SPIFFS.h"

#define led 22
#define botao 26

bool estadoBotao_atual;
bool estadoBotao_anterior;
bool estadoLed;

String str;
String s;

void writeFile(String state, String path) { //escreve conteúdo em um arquivo
  File rFile = SPIFFS.open(path, "w+");// w para truncar - apagar e reescrever
  if (!rFile) {
    Serial.println("Erro ao abrir arquivo!");
    return;
  }
  else {
    Serial.print("tamanho");
    Serial.println(rFile.size());
    rFile.println(state);
    Serial.print("Gravou: ");
    Serial.println(state);
  }
  rFile.close();
}

String readFile(String path) {
  Serial.println("Read file");
  File rFile = SPIFFS.open(path, "r");//r+ leitura e escrita
  if (!rFile) {
    Serial.println("Erro ao abrir arquivo!");
  }
  else {
    Serial.print("----------Lendo arquivo ");
    Serial.print(path);
    Serial.println("  ---------");
    while (rFile.position() < rFile.size())
    {
      s = rFile.readStringUntil('\n');
      s.trim();
      Serial.println(s);
    }
    rFile.close();
    return s;
  }
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
  
  pinMode(led, OUTPUT);
  pinMode(botao, INPUT);
  //digitalWrite(led, LOW);
  Serial.begin(115200);
  estadoBotao_atual = 1; 
  estadoBotao_anterior = 1;

  //formatFile();
  openFS(); 
  // Ler o dado armazenado anteriormente.
  Serial.println("ler arquivo");
  if((readFile("/logsAula.txt")) == "1" ){
    estadoLed = 1;
  } else {
    estadoLed = 0;
  }
  
  Serial.print("estadoLed: ");
  Serial.println(estadoLed);
  digitalWrite(led, estadoLed);
  
}

void loop() {
   // this speeds up the simulation
  Serial.println("Início");
  
  delay(100); // Isso acelera a simulação

  estadoBotao_atual = digitalRead(botao);

  Serial.print("Estado do botão atual: ");
  Serial.println(estadoBotao_atual);
  Serial.print("Estado do botão anterior: ");
  Serial.println(estadoBotao_anterior);

  if (estadoBotao_atual != estadoBotao_anterior) {
    if (estadoBotao_atual == HIGH) {
      // Botão pressionado: inverte o estado do LED
      estadoLed = !estadoLed;
      digitalWrite(led, estadoLed ? HIGH : LOW);
      Serial.print("estadoLed: ");
      Serial.println(estadoLed);
    }
    
    str = String(estadoLed);
    writeFile(str , "/logsAula.txt");
    estadoBotao_anterior = estadoBotao_atual;
  }

  delay(100);
  Serial.println("Final");

}

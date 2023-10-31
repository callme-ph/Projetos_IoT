#include <FS.h>
#include "SPIFFS.h"

#define led 22
#define botao 21

int num=0;
String str;
String s;
float led_state;
bool botao_state;
bool estado;


void writeFile(String state, String path) { //escreve conteúdo em um arquivo
  File rFile = SPIFFS.open(path, "a");//a para anexar
  if (!rFile) {
    Serial.println("Erro ao abrir arquivo!");
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
  
  Serial.begin(115200);
  Serial.println("inicio");
  delay(200);
  //formatFile();
  Serial.println("abrir arquivo");
  openFS(); 
  num = random(5000);//gerando numero aleatorio para salvar no arquivo
  str = String(num);//convertendo o int para String
  writeFile(str , "/logsAula.txt");
  Serial.println("ler arquivo");
  String test = readFile("/logsAula.txt");
  Serial.print(test);

}

void loop() {
  delay(500);
  botao_state = digitalRead(botao);
  Serial.print("estado: ");
  Serial.println(estado);
  Serial.print("Botao:");
  Serial.println(botao_state);
  
  if ( (botao_state == 0) && (estado == 1) ){
    digitalWrite(led, LOW);
    estado = false;
  } 

  if ( (botao_state == 0) && (estado == 0)){
    digitalWrite(led, HIGH);
    estado = true;
  } 

}

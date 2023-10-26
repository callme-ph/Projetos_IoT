#include <SoftwareSerial.h>
SoftwareSerial ArduinoMaster(10,11);
char cmd="";
char old_cmd;

#define LED 7
int estadoBotao;
float estadoLuz;

void setup() {
  ArduinoMaster.begin(9600);
  pinMode(LED, OUTPUT);
}

void loop() {
  old_cmd = cmd;

  if(ArduinoMaster.available()){
    estadoBotao = ArduinoMaster.read();
    estadoLuz = ArduinoMaster.read();
    Serial.print("Escravo recebeu: ");
    Serial.print(estadoBotao);
    Serial.print(" e ");
    Serial.println(estadoLuz);
  } else {
    Serial.print("Erro ao se comunicar com o mestre.");
  }

  if(estadoBotao){
    digitalWrite(LED, LOW);
  }else{
    digitalWrite(LED, estadoLuz);
  }
}

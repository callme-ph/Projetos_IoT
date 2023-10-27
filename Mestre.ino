#include <SoftwareSerial.h>
SoftwareSerial ArduinoSlave(10,11);

#define botao 8
#define luz 13

bool estadoBotao;
float estadoLuz;

void setup() {
  Serial.begin(9600);
  ArduinoSlave.begin(9600);

  pinMode(botao, INPUT);
  pinMode(luz, INPUT);

}

void loop() {
  estadoBotao = digitalRead(botao);
  estadoLuz = analogRead(luz);

  if(!ArduinoSlave.available()){
    Serial.print("Comunicação com Arduinoslave falhou.");
  }

  delay(2000);
  Serial.print("Botao: ");
  Serial.println(estadoBotao);
  Serial.print("Luz: ");
  Serial.println(estadoLuz);
  
  ArduinoSlave.write(estadoBotao);
  ArduinoSlave.write(estadoLuz);
  
}

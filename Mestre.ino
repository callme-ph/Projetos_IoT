#include <SoftwareSerial.h>
SoftwareSerial ArduinoSlave(10,11);
char cmd="";
char old_cmd;
char answer="";
char old_answer;

#define botao 8
#define luz 13

bool estadoBotao;
float estadoLuz;

void setup() {
  Serial.begin(9600);
  Serial.println("Digite algo.");
  ArduinoSlave.begin(9600);

  pinMode(botao, INPUT);
  pinMode(luz, INPUT);

}

void loop() {
  old_cmd = cmd;
  old_answer = answer;

  estadoBotao = digitalRead(botao);
  estadoLuz = analogRead(luz);

  if(ArduinoSlave.available()){
    answer = ArduinoSlave.read();
  }else{
    //Serial.print("Arduinoslave não disponível.");
  }

  
   Serial.print("Botao: ");
   Serial.println(estadoBotao);
   Serial.print("Luz: ");
   Serial.println(estadoLuz);
   delay(2000);
    
   ArduinoSlave.write(estadoBotao);
   ArduinoSlave.write(estadoLuz);
  

  if(answer!=old_answer){
    Serial.print("Escravo recebeu: ");
    Serial.println(answer);
  }
}

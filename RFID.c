//Inclui as bibliotecas do SPI e do RC522
#include <SPI.h>
#include <MFRC522.h>

//Definição dos pinos de conexão do projeto 
#define PINO_SS 9
#define PINO_RST 8

//O led_buzzer é o led da placa usado para simular o funcionamento de um
//buzzer para alertar o bloqueio do sistema
#define led_buzzer 13
//O led_n é o led usado para acessos negados, alerta e estado de espera.
#define led_n 7
// O led_a é o led usado para acesso permitido
#define led_a 6
// Contador usado para verificacao de tentativas negadas.
int cont = 0;

//Cria o item para configurar o módulo RC522
MFRC522 mfrc522(PINO_SS, PINO_RST); 

void setup() {
   Serial.begin(9600); // Inicializa a serial
   SPI.begin();// Inicializa a comunicação SPI
   mfrc522.PCD_Init(); // Inicializa o módulo MFRC522
   
   pinMode(led_buzzer, OUTPUT);
   pinMode(led_n, OUTPUT);
   pinMode(led_a, OUTPUT);
   Serial.println("Sistema Inicializado: Aproxime o token");
   
}
void loop() {
   digitalWrite(led_n, HIGH); 
   digitalWrite(led_a, LOW);
   
   if (!mfrc522.PICC_IsNewCardPresent()) return;// Aguarda a aproximação do token
   
   if (!mfrc522.PICC_ReadCardSerial()) return; // Seleciona qual token vai ser utilizado 
   
   Serial.print("UID da tag:"); // Mostra UID do token na serial
   String conteudo= ""; //Cria uma variável vazia, do tipo string para armazenar o UID do token.
   byte letra;

   // Usando os métodos da biblioteca mrfc522 para fazer a leitura e print do UID do token.
   for (byte i = 0; i < mfrc522.uid.size; i++) {
    if(mfrc522.uid.uidByte[i] < 0x10){
      Serial.print(" 0");
    }
    else{
    Serial.print(" ");
    }
    
    Serial.print(mfrc522.uid.uidByte[i], HEX);// Printa a mensagem convertida em hexadecimal
   // Enquanto a leitura e output do UID são feitos, os dados são armazenado na string conteudo.
    if(mfrc522.uid.uidByte[i] < 0x10){
      conteudo.concat(String(" 0"));
    }
    else{
      conteudo.concat(String(" "));
    }
   conteudo.concat(String(mfrc522.uid.uidByte[i], HEX));
   }
   
   Serial.println();
   conteudo.toUpperCase();//Coloca todas as letras da string em maiúscula

   // Verificacao se o UID do token lido -  que foi guardado na string conteudo - é igual a um UID autorizado.
   if (conteudo.substring(1) == "13 4E AA A2") {
      
      digitalWrite(led_n, LOW); // Led para acesso negado ou estado de espera apagado.
      digitalWrite(led_a, HIGH); // Ativacao do led que sinaliza o acesso permitido.
      Serial.println("Acesso Permitido!");
      delay(10000);
      cont = 0; // O contador é zerado assim que um acesso é permitido.
   }else{
      // Em caso de acesso negado, o contador é incrementado e um aviso é mostrado.
      cont = cont + 1;
      Serial.print("Cartão inválido! Tentativa: ");
      Serial.println(cont);

      // Verificacao da quantidade de acessos negados.
      // Se forem feitas 5 tentativas negadas, o sistema é bloqueado por 20 segundos.
      if( cont == 5 ){
        Serial.println("SISTEMA BLOQUEADO");
        digitalWrite(led_buzzer, HIGH); // O led_buzzer é acionado para o alerta sonoro.
        cont = 0; // O contador é zerado.

         // led_n irá piscar 
        for(int j = 0; j < 10; j++){
          digitalWrite(led_n, LOW);
          delay(1000);
          digitalWrite(led_n, HIGH);
          delay(1000);
        }

         digitalWrite(led_buzzer, LOW); // Alerta sonoro é desativado e o codigo retorna para o inicio.
         return;
      }

      // Caso o acesso seja negado mas não seja o quinto acesso negado, o led_n ira piscar por alguns segundos.
      for(int j = 0; j < 3; j++){
        digitalWrite(led_n, LOW);
        delay(1000);
        digitalWrite(led_n, HIGH);
        delay(1000);

      }
      
   }
   
   delay(1000);
}

#define LED_PIN1 26
#define LED_PIN2 25
#define LED_PIN3 32

unsigned long previousMillis = 0;
unsigned long currentMillis = 0;

void setup() {
  
  Serial.begin(115200);
  
  pinMode(LED_PIN1,OUTPUT);
  pinMode(LED_PIN2,OUTPUT);
  pinMode(LED_PIN3,OUTPUT);
  digitalWrite(LED_PIN1, LOW);
  digitalWrite(LED_PIN2, LOW);
  digitalWrite(LED_PIN3, LOW);
}

void loop() {
  digitalWrite(LED_PIN1, LOW);
  digitalWrite(LED_PIN2, LOW);
  digitalWrite(LED_PIN3, LOW);

  Serial.print("Aperte para iniciar");
  
  if (touchRead(13)<50){
    digitalWrite(LED_PIN2, HIGH);
    delay(1000);
    digitalWrite(LED_PIN2, LOW);
    delay(1000);
    digitalWrite(LED_PIN2, HIGH);
    delay(1000);
    digitalWrite(LED_PIN2, LOW);
    delay(1000);
    digitalWrite(LED_PIN2, HIGH);
    delay(1000);
    digitalWrite(LED_PIN2, LOW);
    delay (1000);
    digitalWrite(LED_PIN2, HIGH);

    previousMillis = currentMillis;
    
    Serial.println("Comecem!");
    
     while ((touchRead(12))>65 && (touchRead(14))>65){
       
     }
        
        if ((touchRead(12))<65){
            digitalWrite(LED_PIN3, HIGH);
            digitalWrite(LED_PIN2, LOW);
            currentMillis = millis();
            Serial.print("Jogador 1 ganhou!. Tempo: ");
            Serial.print((currentMillis-previousMillis)/1000);
            Serial.println("s");
            delay(3000);
            return; 
            
        }
        
        if ((touchRead(14))<65){
            digitalWrite(LED_PIN1, HIGH);
            digitalWrite(LED_PIN2, LOW);
            currentMillis = millis();
            Serial.print("Jogador 2 ganhou!. Tempo: ");
            Serial.print((currentMillis-previousMillis)/1000);
            Serial.println("s");
            delay(3000);
            return; 
        }
    }
  }

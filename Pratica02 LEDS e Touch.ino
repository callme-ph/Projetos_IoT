#define LED_PIN1 33
#define LED_PIN2 25
#define LED_PIN3 26
#define BLINK_INTERVAL 250



int ledState= LOW;

unsigned long previousMillis = 0;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  //delay(1000);
 // Serial.println("ESP32 Touch Test");
  pinMode(LED_PIN1,OUTPUT);
  pinMode(LED_PIN2,OUTPUT);
  pinMode(LED_PIN3,OUTPUT);
  delay(3000);
  Serial.print("Aperte para iniciar");
}

void loop() {
  digitalWrite(LED_PIN1, LOW);
  digitalWrite(LED_PIN2, LOW);
  digitalWrite(LED_PIN3, LOW);
  // put your main code here, to run repeatedly:
  unsigned long previousMillis = 0;
  //Serial.println(touchRead(13));
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
    unsigned long currentMillis = millis();
     while ((touchRead(12))>65 && (touchRead(14))>65){
        Serial.print("Comecem!");
        delay (1000);
     }
        
        if ((touchRead(12))<65){
            digitalWrite(LED_PIN3, HIGH);
            Serial.println(currentMillis);
            return; 
            
        }
        
        if ((touchRead(14))<65){
            digitalWrite(LED_PIN1, HIGH);
            Serial.println(currentMillis);
            return; 
        }
    }
  }

  //delay(1000);
  
  /*if (currentMillis - previousMillis >= BLINK_INTERVAL){
      digitalWrite(LED_PIN, !digitalRead(LED_PIN));
      Serial.print("Tempo de processamento do ESP 32 ");
      Serial.println(currentMillis - previousMillis);
      previousMillis = currentMillis;
      
  }
}*/

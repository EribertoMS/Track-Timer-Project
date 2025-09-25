int ledPin = 2;
int pirPin = 4;
int pirValue;

void setup() 
{  
  Serial.begin(115200);

  pinMode(ledPin, OUTPUT);
  pinMode(pirPin, INPUT);
 
  digitalWrite(ledPin, LOW);
}

void loop() 
{

  if(digitalRead(pirPin) == HIGH)
  {
    digitalWrite(ledPin, HIGH);
    Serial.println("LED ON");
    delay(1000);
    digitalWrite(ledPin,LOW);
  }
  else
  {
    digitalWrite(ledPin, LOW);
  }

}

#define LED 2

HardwareSerial MySerial(2);

int button = 4;

void setup()
{
  Serial.begin(9600);
  MySerial.begin(9600,SERIAL_8N1,16,17);

  pinMode(LED,OUTPUT);
  pinMode(button,INPUT_PULLUP);
}

void loop()
{
  if(digitalRead(button) == LOW)
  {
    digitalWrite(LED,HIGH);

    MySerial.println("LED ON");
    Serial.println("Sent ON");
  }
  else if(digitalRead(button) == HIGH)
  {
    digitalWrite(LED,LOW);

    MySerial.println("LED OFF");
    Serial.println("Sent OFF");
  }
}
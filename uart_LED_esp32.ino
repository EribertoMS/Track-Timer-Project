#define LED 2

HardwareSerial MySerial(2);

void setup()
{
  pinMode(LED,OUTPUT);

  Serial.begin(9600);
  MySerial.begin(9600,SERIAL_8N1,16,17);
}

void loop()
{
  digitalWrite(LED,HIGH);
  MySerial.println("LED ON");
  Serial.println("Sent: ON");
  delay(1000);

  digitalWrite(LED,LOW);
  MySerial.println("LED OFF");
  Serial.println("Sent: OFF");
  delay(1000);
}
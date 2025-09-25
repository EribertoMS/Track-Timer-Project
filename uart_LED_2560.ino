#define LED_BUILTIN 13

void setup()
{
  pinMode(LED_BUILTIN,OUTPUT);
  Serial.begin(9600);
  Serial1.begin(9600);
}

void loop()
{
  if(Serial1.available())
  {
    String cmd = Serial1.readStringUntil('\n');
    cmd.trim();
    Serial.print("Command Received: " + cmd + "\n");

    if(cmd == "LED ON")
    {
      digitalWrite(LED_BUILTIN,HIGH);
      Serial.println("LED turned on");
    }
    else if(cmd == "LED OFF")
    {
      digitalWrite(LED_BUILTIN,LOW);
      Serial.println("LED turned OFF");
    }
  }
}
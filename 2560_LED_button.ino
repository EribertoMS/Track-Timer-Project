int pin2 = 2;

void setup()
{
  Serial1.setTimeout(50); // 50ms timeout instead of 1000ms

  pinMode(pin2,OUTPUT);

  Serial.begin(115200);
  Serial1.begin(115200);
}

void loop()
{
  if(Serial1.available())
  {
    String cmd = Serial1.readStringUntil('\n');
    cmd.trim();
    Serial.print("Command Received: " + cmd + '\n');

    if(cmd == "LED ON")
    {
      digitalWrite(pin2,HIGH);
    }
    else if (cmd == "LED OFF")
    {
      digitalWrite(pin2,LOW);
    }
  }
}
int pin1 = 22; // e
int pin2 = 23; // d
int pin3 = 24; // .
int pin4 = 25; // c
int pin5 = 26; // g
int pin7 = 27; // b
int pin10 = 28; // f
int pin11 = 29; // a

void setup()
{
  pinMode(pin1,OUTPUT);
  pinMode(pin2,OUTPUT);
  pinMode(pin3,OUTPUT);
  pinMode(pin4,OUTPUT);
  pinMode(pin5,OUTPUT);
  pinMode(pin7,OUTPUT);
  pinMode(pin10,OUTPUT);
  pinMode(pin11,OUTPUT);

}

void loop() {
  zero(); closeall();
  one(); closeall();
  two(); closeall();
  three(); closeall();
  four(); closeall();
  five(); closeall();
  six(); closeall();
  seven(); closeall();
  eight(); closeall();
  nine(); closeall();
}

void closeall()
{
  digitalWrite(pin1,LOW);
  digitalWrite(pin2,LOW);
  digitalWrite(pin3,LOW);
  digitalWrite(pin4,LOW);
  digitalWrite(pin5,LOW);
  digitalWrite(pin7,LOW);
  digitalWrite(pin10,LOW);
  digitalWrite(pin11,LOW);
}
void zero()
{
  digitalWrite(pin11,HIGH);
  digitalWrite(pin7,HIGH);
  digitalWrite(pin4,HIGH);
  digitalWrite(pin2,HIGH);
  digitalWrite(pin1,HIGH);
  digitalWrite(pin10,HIGH);
  delay(1000);
}

void one()
{
  digitalWrite(pin7,HIGH);
  digitalWrite(pin4,HIGH);
  delay(1000);
}

void two()
{
  digitalWrite(pin11,HIGH);
  digitalWrite(pin7,HIGH);
  digitalWrite(pin5,HIGH);
  digitalWrite(pin1,HIGH);
  digitalWrite(pin2,HIGH);
  delay(1000);
}
void three() 
{
  digitalWrite(pin11,HIGH); // a
  digitalWrite(pin7,HIGH);  // b
  digitalWrite(pin4,HIGH);  // c
  digitalWrite(pin2,HIGH);  // d
  digitalWrite(pin3,HIGH);  // .
  digitalWrite(pin1,LOW);   // e
  digitalWrite(pin5,HIGH);  // g
  delay(1000);
}

void four() 
{
  digitalWrite(pin10,HIGH); // f
  digitalWrite(pin7,HIGH);  // b
  digitalWrite(pin4,HIGH);  // c
  digitalWrite(pin5,HIGH);  // g
  delay(1000);
}

void five() 
{
  digitalWrite(pin11,HIGH); // a
  digitalWrite(pin10,HIGH); // f
  digitalWrite(pin5,HIGH);  // g
  digitalWrite(pin4,HIGH);  // c
  digitalWrite(pin2,HIGH);  // d
  delay(1000);
}

void six() 
{
  digitalWrite(pin11,HIGH); // a
  digitalWrite(pin10,HIGH); // f
  digitalWrite(pin5,HIGH);  // g
  digitalWrite(pin1,HIGH);  // e
  digitalWrite(pin2,HIGH);  // d
  digitalWrite(pin4,HIGH);  // c
  delay(1000);
}

void seven() 
{
  digitalWrite(pin11,HIGH); // a
  digitalWrite(pin7,HIGH);  // b
  digitalWrite(pin4,HIGH);  // c
  delay(1000);
}

void eight() 
{
  digitalWrite(pin11,HIGH); // a
  digitalWrite(pin7,HIGH);  // b
  digitalWrite(pin4,HIGH);  // c
  digitalWrite(pin2,HIGH);  // d
  digitalWrite(pin1,HIGH);  // e
  digitalWrite(pin10,HIGH); // f
  digitalWrite(pin5,HIGH);  // g
  delay(1000);
}

void nine() 
{
  digitalWrite(pin11,HIGH); // a
  digitalWrite(pin7,HIGH);  // b
  digitalWrite(pin4,HIGH);  // c
  digitalWrite(pin2,HIGH);  // d
  digitalWrite(pin10,HIGH); // f
  digitalWrite(pin5,HIGH);  // g
  delay(1000);
}
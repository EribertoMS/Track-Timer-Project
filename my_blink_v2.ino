#include "pitches.h"

int GREEN = 22;
int YELLOW = 23;
int RED = 15;

int button = 13;

int melody[] = 
{
  NOTE_C5, 
  NOTE_D5, 
  NOTE_E5, 
  NOTE_F5, 
  NOTE_G5, 
  NOTE_A5, 
  NOTE_B5, 
  NOTE_C6
};

int duration = 500;

void setup()
{
  Serial.begin(9600);

  pinMode(GREEN,OUTPUT);
  pinMode(YELLOW,OUTPUT);
  pinMode(RED,OUTPUT);

  pinMode(button,INPUT_PULLUP);
}

void loop()
{

  if(digitalRead(button) == LOW)
  {
    digitalWrite(RED,HIGH);
    tone(14,melody[0],duration);
    delay(500);
    digitalWrite(RED,LOW);

    delay(1000);

    digitalWrite(YELLOW,HIGH);
    tone(14,melody[0],duration);
    delay(500);
    digitalWrite(YELLOW,LOW);

    delay(1000);

    digitalWrite(GREEN,HIGH);
    tone(14,melody[7],1000);
    delay(1000);
    digitalWrite(GREEN,LOW);
  }
  
  /*
  for (int thisNote = 0; thisNote < 8; thisNote++) 
  {
    // pin8 output the voice, every scale is 0.5 sencond
    tone(14, melody[thisNote], duration);
     
    // Output the voice after several minutes
    delay(1000);
  }
  */
  
  // restart after two seconds 
  delay(100);
}
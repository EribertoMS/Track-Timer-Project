#include "SevSeg.h"

SevSeg sevseg; //Instantiate a seven segment controller object

#define BUILTIN_LED 2

int button1 = 19;

void setup() 
{
  Serial.begin(115200);
  
  pinMode(BUILTIN_LED,OUTPUT);

  pinMode(button1,INPUT_PULLUP);
  pinMode(button2,INPUT_PULLUP);
  pinMode(button3,INPUT_PULLUP);

  byte numDigits = 4;
  byte digitPins[] = {15, 4, 5, 18};
  byte segmentPins[] = {14, 13, 26, 33, 32, 12, 27, 25}; // A to DP
  bool resistorsOnSegments = false; // 'false' means resistors are on digit pins
  byte hardwareConfig = COMMON_CATHODE; // See README.md for options
  bool updateWithDelays = false; // Default 'false' is Recommended
  bool leadingZeros = true; // Use 'true' if you'd like to keep the leading zeros
  bool disableDecPoint = false; // Use 'true' if your decimal point doesn't exist or isn't connected
  
  sevseg.begin(hardwareConfig, numDigits, digitPins, segmentPins, resistorsOnSegments,
  updateWithDelays, leadingZeros, disableDecPoint);

  sevseg.setBrightness(90);

  digitalWrite(BUILTIN_LED,LOW);
}

void loop() 
{

  static unsigned long timer = millis();
  static int deciSeconds = 0;

  if(digitalRead(button1) == LOW)
  {
    digitalWrite(BUILTIN_LED,LOW);
    if (millis() - timer >= 100) 
    {
      timer += 100;
      deciSeconds++; // 100 milliSeconds is equal to 1 deciSecond
      
      if (deciSeconds == 10000) 
      { // Reset to 0 after counting for 1000 seconds.
        deciSeconds=0;
      }
      sevseg.setNumber(deciSeconds, 1);
    }
  } 
  else
  {
    //Serial.println("LED is ON"); // when this is uncommmented the 7segdisp flickers
    digitalWrite(BUILTIN_LED,HIGH);
    deciSeconds=0;
    sevseg.setNumber(0,1);
  }

  sevseg.refreshDisplay(); // Must run repeatedly

}
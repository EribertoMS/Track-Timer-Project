#include "SevSeg.h"

SevSeg sevseg; // Instantiate a seven segment controller object

#define BUILTIN_LED 2

// Button pins
int buttonStart = 19;    // Button to start counting
int buttonStop = 23;     // Button to stop counting
int buttonReset = 21;    // Button to reset to 0.000

// Timer states
enum TimerState 
{
  STANDBY,      // Timer ready to start (showing 0.000)
  COUNTING,     // Timer is actively counting
  STOPPED       // Timer stopped at current value
};

TimerState timerState = STANDBY;
unsigned long lastUpdateTime = 0;
int deciSeconds = 0;
unsigned long elapsedPausedTime = 0; // Tracks time while paused

void setup() 
{
  Serial.begin(115200);
  
  pinMode(BUILTIN_LED, OUTPUT);
  pinMode(buttonStart, INPUT_PULLUP);
  pinMode(buttonStop, INPUT_PULLUP);
  pinMode(buttonReset, INPUT_PULLUP);

  byte numDigits = 4;
  byte digitPins[] = {15, 4, 5, 18};
  byte segmentPins[] = {14, 13, 26, 33, 32, 12, 27, 25}; // A to DP
  bool resistorsOnSegments = false;
  byte hardwareConfig = COMMON_CATHODE;
  bool updateWithDelays = false;
  bool leadingZeros = true;
  bool disableDecPoint = false;
  
  sevseg.begin(hardwareConfig, numDigits, digitPins, segmentPins, resistorsOnSegments,
               updateWithDelays, leadingZeros, disableDecPoint);

  sevseg.setBrightness(90);
  resetTimer(); // Initialize display to 0.000
}

void loop() 
{
  handleTimer();
  sevseg.refreshDisplay(); // Must run repeatedly
}

void handleTimer() 
{
  // Read button states (using INPUT_PULLUP, so LOW means pressed)
  bool startPressed = (digitalRead(buttonStart) == LOW);
  bool stopPressed = (digitalRead(buttonStop) == LOW);
  bool resetPressed = (digitalRead(buttonReset) == LOW);

  // State machine for timer control
  switch(timerState) 
  {
    case STANDBY:
      digitalWrite(BUILTIN_LED, LOW);
      if (startPressed) 
      {
        // Start button pressed - begin counting
        timerState = COUNTING;
        lastUpdateTime = millis() - elapsedPausedTime; // Account for any paused time
        digitalWrite(BUILTIN_LED, HIGH);
      }
      break;
      
    case COUNTING:
      // Update timer while counting
      if (millis() - lastUpdateTime >= 100) 
      {
        lastUpdateTime += 100;
        deciSeconds++;
        
        if (deciSeconds >= 10000) 
        { // Reset after 1000 seconds (10000 deciSeconds)
          deciSeconds = 0;
        }
        
        sevseg.setNumber(deciSeconds, 1);
      }

      if (stopPressed) 
      {
        // Stop button pressed - pause timer
        timerState = STOPPED;
        elapsedPausedTime = millis() - lastUpdateTime;
        digitalWrite(BUILTIN_LED, LOW);
      }
      break;
      
    case STOPPED:
      if (resetPressed) 
      {
        // Reset button pressed - go back to standby
        resetTimer();
        timerState = STANDBY;
      } 
      else if (startPressed) 
      {
        // Start button pressed - resume counting
        timerState = COUNTING;
        lastUpdateTime = millis() - elapsedPausedTime;
        digitalWrite(BUILTIN_LED, HIGH);
      }
      break;
  }
}

void resetTimer() 
{
  deciSeconds = 0;
  elapsedPausedTime = 0;
  sevseg.setNumber(0, 1); // Display 0.000
  digitalWrite(BUILTIN_LED, LOW);
}
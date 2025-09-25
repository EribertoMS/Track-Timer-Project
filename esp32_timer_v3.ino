#include "SevSeg.h"
#include "ESP32_NOW.h"
#include "WiFi.h"
#include <esp_mac.h>
#include <vector>

SevSeg sevseg; // Instantiate a seven segment controller object

#define ESPNOW_WIFI_CHANNEL 6
#define SLAVE_LED 2

// Button pins
int buttonStart = 19;    // Button to start counting
int buttonStop = 23;     // Button to stop counting
int buttonReset = 21;    // Button to reset to 0.000

typedef struct 
{
  char message[32];
  bool led_state;
} MessageData;

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

// Forward declarations for timer control functions
void startTimer();
void stopTimer();
void resetTimer();

class ESP_NOW_Peer_Class : public ESP_NOW_Peer 
{
public:
  ESP_NOW_Peer_Class(const uint8_t *mac_addr, uint8_t channel, wifi_interface_t iface, const uint8_t *lmk) : 
    ESP_NOW_Peer(mac_addr, channel, iface, lmk) {}

  ~ESP_NOW_Peer_Class() {}

  bool add_peer() 
  {
    if (!add()) 
    {
      log_e("Failed to register the broadcast peer");
      return false;
    }
    return true;
  }

  void onReceive(const uint8_t *data, size_t len, bool broadcast) 
  {
    if (len == sizeof(MessageData)) 
    {
      MessageData* msg = (MessageData*)data;
      Serial.printf("Received message from master " MACSTR ": %s\n", MAC2STR(addr()), msg->message);
      Serial.printf("LED state: %d\n", msg->led_state);
      
      // Control the slave's LED based on master's command
      digitalWrite(SLAVE_LED, msg->led_state ? HIGH : LOW);
      
      // Start or stop timer based on received command
      if (msg->led_state) 
      {
        startTimer();
      } 
    }
  }
};

std::vector<ESP_NOW_Peer_Class> masters;

void setup() 
{
  Serial.begin(115200);
  
  pinMode(SLAVE_LED, OUTPUT);
  digitalWrite(SLAVE_LED, LOW);
  pinMode(buttonStart, INPUT_PULLUP);
  pinMode(buttonStop, INPUT_PULLUP);
  pinMode(buttonReset, INPUT_PULLUP);

  // Initialize 7-segment display
  byte numDigits = 4;
  byte digitPins[] = {15, 4, 5, 18};
  byte segmentPins[] = {14, 13, 26, 33, 32, 12, 27, 25};
  bool resistorsOnSegments = false;
  byte hardwareConfig = COMMON_CATHODE;
  bool updateWithDelays = false;
  bool leadingZeros = true;
  bool disableDecPoint = false;
  
  sevseg.begin(hardwareConfig, numDigits, digitPins, segmentPins,
               resistorsOnSegments, updateWithDelays, leadingZeros, disableDecPoint);
  sevseg.setBrightness(90);
  resetTimer();

  // Initialize WiFi
  WiFi.mode(WIFI_STA);
  WiFi.setChannel(ESPNOW_WIFI_CHANNEL);
  while (!WiFi.STA.started()) 
  {
    delay(100);
  }

  Serial.println("ESP-NOW Timer Slave");
  Serial.println("Wi-Fi parameters:");
  Serial.println("  Mode: STA");
  Serial.println("  MAC Address: " + WiFi.macAddress());
  Serial.printf("  Channel: %d\n", ESPNOW_WIFI_CHANNEL);

  if (!ESP_NOW.begin()) 
  {
    Serial.println("Failed to initialize ESP-NOW");
    Serial.println("Rebooting in 5 seconds...");
    delay(5000);
    ESP.restart();
  }

  ESP_NOW.onNewPeer([](const esp_now_recv_info_t *info, const uint8_t *data, int len, void *arg) 
  {
    if (memcmp(info->des_addr, ESP_NOW.BROADCAST_ADDR, 6) == 0) 
    {
      Serial.printf("New master detected: " MACSTR "\n", MAC2STR(info->src_addr));
      
      for (auto &master : masters) 
      {
        if (memcmp(master.addr(), info->src_addr, 6) == 0) 
        {
          return;
        }
      }
      
      ESP_NOW_Peer_Class new_master(info->src_addr, ESPNOW_WIFI_CHANNEL, WIFI_IF_STA, NULL);
      masters.push_back(new_master);
      
      if (!masters.back().add_peer()) 
      {
        Serial.println("Failed to register the new master");
        masters.pop_back();
      }
    }
  }, NULL);

  Serial.println("Setup complete. Waiting for commands...");
}

void loop() 
{
  handleTimer();
  handleButtons();
  sevseg.refreshDisplay();
}

void handleTimer() 
{
  if (timerState == COUNTING) 
  {
    if (millis() - lastUpdateTime >= 100) 
    {
      lastUpdateTime += 100;
      deciSeconds++;
      
      if (deciSeconds >= 10000) 
      {
        deciSeconds = 0;
      }
      
      sevseg.setNumber(deciSeconds, 1);
    }
  }
}

void handleButtons() 
{
  if (digitalRead(buttonStart) == LOW && timerState != COUNTING) 
  {
    startTimer();
    delay(200); // Debounce
  }
  
  if (digitalRead(buttonStop) == LOW && timerState == COUNTING) 
  {
    stopTimer();
    delay(200); // Debounce
  }
  
  if (digitalRead(buttonReset) == LOW && timerState != COUNTING) 
  {
    resetTimer();
    delay(200); // Debounce
  }
}

void startTimer() 
{
  if (timerState != COUNTING) 
  {
    timerState = COUNTING;
    lastUpdateTime = millis() - elapsedPausedTime;
    digitalWrite(SLAVE_LED, HIGH);
    Serial.println("Timer started");
  }
}

void stopTimer() 
{
  if (timerState == COUNTING) 
  {
    timerState = STOPPED;
    elapsedPausedTime = millis() - lastUpdateTime;
    digitalWrite(SLAVE_LED, LOW);
    Serial.println("Timer stopped");
  }
}

void resetTimer() 
{
  timerState = STANDBY;
  deciSeconds = 0;
  elapsedPausedTime = 0;
  sevseg.setNumber(0, 1);
  digitalWrite(SLAVE_LED, LOW);
  Serial.println("Timer reset");
}
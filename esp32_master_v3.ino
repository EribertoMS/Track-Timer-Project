#include "ESP32_NOW.h"
#include "WiFi.h"
#include <esp_mac.h>

#define ESPNOW_WIFI_CHANNEL 6
#define BUILTIN_LED 2
#define BUTTON_PIN 15

// Structure for message data
typedef struct 
{
  char message[32];
  bool led_state;
} MessageData;

class ESP_NOW_Broadcast_Peer : public ESP_NOW_Peer 
{
public:
  ESP_NOW_Broadcast_Peer(uint8_t channel, wifi_interface_t iface, const uint8_t *lmk) : 
    ESP_NOW_Peer(ESP_NOW.BROADCAST_ADDR, channel, iface, lmk) {}

  ~ESP_NOW_Broadcast_Peer() 
  {
    remove();
  }

  bool begin() {
    if (!ESP_NOW.begin() || !add()) 
    {
      log_e("Failed to initialize ESP-NOW or register the broadcast peer");
      return false;
    }
    return true;
  }

  bool send_message(const uint8_t *data, size_t len) 
  {
    if (!send(data, len)) 
    {
      log_e("Failed to broadcast message");
      return false;
    }
    return true;
  }
};

ESP_NOW_Broadcast_Peer broadcast_peer(ESPNOW_WIFI_CHANNEL, WIFI_IF_STA, NULL);
unsigned long previousMillis = 0;
const long interval = 2000; // 2 second interval
bool ledState = LOW;

void setup() {
  Serial.begin(115200);
  while (!Serial) 
  {
    delay(10);
  }

  pinMode(BUILTIN_LED, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  // Initialize WiFi
  WiFi.mode(WIFI_STA);
  WiFi.setChannel(ESPNOW_WIFI_CHANNEL);
  while (!WiFi.STA.started()) 
  {
    delay(100);
  }

  Serial.println("ESP-NOW Example - Broadcast Master");
  Serial.println("Wi-Fi parameters:");
  Serial.println("  Mode: STA");
  Serial.println("  MAC Address: " + WiFi.macAddress());
  Serial.printf("  Channel: %d\n", ESPNOW_WIFI_CHANNEL);

  if (!broadcast_peer.begin()) 
  {
    Serial.println("Failed to initialize broadcast peer");
    Serial.println("Rebooting in 5 seconds...");
    delay(5000);
    ESP.restart();
  }

  Serial.println("Setup complete. Broadcasting LED commands every 2 seconds...");
}

void loop() 
{
  unsigned long currentMillis = millis();
  
  // Check if 2 seconds have passed
  if (currentMillis - previousMillis >= interval) 
  {
    previousMillis = currentMillis;
    
    // Toggle LED state
    ledState = !ledState;
    digitalWrite(BUILTIN_LED, ledState);
    
    // Prepare and send message to slave
    MessageData data;
    snprintf(data.message, sizeof(data.message), "LED command");
    data.led_state = ledState;
    
    Serial.printf("Broadcasting LED state: %d\n", ledState);
    
    if (!broadcast_peer.send_message((uint8_t *)&data, sizeof(data))) 
    {
      Serial.println("Failed to broadcast message");
    }
  }
  
  // Optional: Still allow button control if needed
  if (digitalRead(BUTTON_PIN) == LOW) 
  {
    digitalWrite(BUILTIN_LED, HIGH);
    // You could add additional button logic here if needed
  }
}
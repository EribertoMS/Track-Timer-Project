#include "ESP32_NOW.h"
#include "WiFi.h"
#include <esp_mac.h>
#include <vector>

#define ESPNOW_WIFI_CHANNEL 6
#define SLAVE_LED 2 // Assuming slave also has onboard LED on pin 2

// Structure for message data (must match master)
typedef struct 
{
  char message[32];
  bool led_state;
} MessageData;

class ESP_NOW_Peer_Class : public ESP_NOW_Peer 
{
public:
  ESP_NOW_Peer_Class(const uint8_t *mac_addr, uint8_t channel, wifi_interface_t iface, const uint8_t *lmk) : ESP_NOW_Peer(mac_addr, channel, iface, lmk) {}

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
      
      // Control the slave's LED based on master's button
      digitalWrite(SLAVE_LED, msg->led_state ? HIGH : LOW);
    }
  }
};

std::vector<ESP_NOW_Peer_Class> masters;

void register_new_master(const esp_now_recv_info_t *info, const uint8_t *data, int len, void *arg) 
{
  if (memcmp(info->des_addr, ESP_NOW.BROADCAST_ADDR, 6) == 0) 
  {
    Serial.printf("New master detected: " MACSTR "\n", MAC2STR(info->src_addr));
    
    // Check if this master is already registered
    for (auto &master : masters) 
    {
      if (memcmp(master.addr(), info->src_addr, 6) == 0) 
      {
        return; // Master already registered
      }
    }
    
    // Register new master
    ESP_NOW_Peer_Class new_master(info->src_addr, ESPNOW_WIFI_CHANNEL, WIFI_IF_STA, NULL);
    masters.push_back(new_master);
    
    if (!masters.back().add_peer()) 
    {
      Serial.println("Failed to register the new master");
      masters.pop_back();
    }
  }
}

void setup() 
{
  Serial.begin(115200);
  while (!Serial) 
  {
    delay(10);
  }

  // Initialize LED pin
  pinMode(SLAVE_LED, OUTPUT);
  digitalWrite(SLAVE_LED, LOW);

  // Initialize WiFi
  WiFi.mode(WIFI_STA);
  WiFi.setChannel(ESPNOW_WIFI_CHANNEL);
  while (!WiFi.STA.started()) 
  {
    delay(100);
  }

  Serial.println("ESP-NOW Example - Broadcast Slave");
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

  ESP_NOW.onNewPeer(register_new_master, NULL);
  Serial.println("Setup complete. Waiting for messages...");
}

void loop() 
{
  delay(100);
}
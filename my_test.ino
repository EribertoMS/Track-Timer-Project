// ESP32: Connect TX = GPIO 17, RX = GPIO 16 to Mega 2560

HardwareSerial MySerial(2);  // Use UART2

void setup() {
  Serial.begin(115200);       // Debug monitor
  MySerial.begin(9600, SERIAL_8N1, 16, 17); // UART2 with RX = 16, TX = 17
}

void loop() {
  MySerial.println("Hello from ESP32");
  Serial.println("Sent: Hello from ESP32");
  delay(2000);
}

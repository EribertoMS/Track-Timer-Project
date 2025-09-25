void setup() {
  Serial.begin(9600);    // For Serial Monitor
  Serial1.begin(9600);   // RX1 (19), TX1 (18)
}

void loop() {
  if (Serial1.available()) {
    String received = Serial1.readStringUntil('\n');
    Serial.print("Received: ");
    Serial.println(received);
  }
}

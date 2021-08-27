// Project: Speedo Pulse Test 1 - 100Hz Pin 10
// Author: Ben Ford
// Use: Simulate vehicle speedo.
// Arduino Pro Micro.

int freq = 1;
byte outpin = 10;

void setup() {
  Serial.begin(9600);
}

void loop() {
  while(freq < 101){
    Serial.println(freq);
    tone(10,freq);
    freq++;
    delay(500);
  }
  freq = 1;
}

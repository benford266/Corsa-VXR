// Project: GPS to Speedo Pulse v2 Works
// Author: Ben Ford
// Use: Simulate vehicle speed pulse using GPS speed.
// Arduino Mega2650 / Pro Micro and NEO-6M GPS Breakout

#include <SoftwareSerial.h>
#include <TinyGPS.h>


byte outPin = 10; // Pulse Output Pin
int freq = 30; // 30Hz default


// Setup modules
TinyGPS gps;
//SoftwareSerial ss(64, 65); // RX / TX << pins to NEO-6M Arduino Mega2650
SoftwareSerial ss(8, 9); // RX / TX << pins to NEO-6M Arduino ProMicro


void setup(){
  Serial.begin(9600); // Debug serial uncomment if needed
  ss.begin(9600); // Begin serial comms to GPS 9600 baud
  Serial.println();
  pinMode (outPin, OUTPUT); // Setup pin for pulse output
}


void loop(){
  bool newData = false;

  for (unsigned long start = millis(); millis() - start < 500;) // 500ms to get current gps speed 
    {
      while (ss.available())
      {
        char c = ss.read();
        //Serial.write(c); // uncomment this line if you want to see the GPS data flowing
        if (gps.encode(c)) // Did a new valid sentence come in?
          newData = true;
      }
    }
    // Pulse Generator
    // Checks if speed is below 1mph if so no pulse is generated.
  if (gps.f_speed_kmph() < 2){
    //Serial.println("No Pulse"); // Uncomment Debug
    freq = int(0);
    noTone(outPin); // Stop pulse
  }
  else { // Above 1mph pulse is generated and 1Hz per KPH / 3600 pulses per Kilometer
    freq  = int(gps.f_speed_kmph()); 
    //Serial.println(gps.f_speed_kmph()); // Uncomment Debug
    tone(outPin, freq); 
    //Serial.println(freq); // Uncomment Debug
  }
 }

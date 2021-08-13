// Project: GPS to Speedo Pulse
// Author: Ben Ford
// Use: Simulate vehicle speed pulse using GPS speed.
// Arduino Mega2650 / Pro Micro and NEO-6M GPS Breakout


#include <SoftwareSerial.h>
#include <TinyGPS.h>

// Setup modules
TinyGPS gps;
//SoftwareSerial ss(64, 65); // RX / TX << pins to NEO-6M Arduino Mega2650
SoftwareSerial ss(8, 9); // RX / TX << pins to NEO-6M Arduino ProMicro

unsigned long cycleStart, period = 100000UL, onTime = period / 2;
const byte outPin = 13; // Output Pin to ECU / Speedo

void setup()
{
  Serial.begin(9600); // Debug serial uncomment if needed
  ss.begin(9600); // Begin serial comms to GPS 9600 baud
  Serial.println();
  pinMode(outPin, OUTPUT);
}

void loop()
{
  //GPS Stuff
  bool newData = false;
  unsigned long chars;
  unsigned short sentences, failed;

  // For 500ms we parse GPS data and report some key values
  for (unsigned long start = millis(); millis() - start < 500;)
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
  if (gps.f_speed_mph() < 1){
    Serial.println("No Pulse");
  }
  else { // Above 1mph pulse is generated and 1Hz per MPH / 3600 pulses per Mile
  digitalWrite(outPin, micros() - cycleStart < onTime);
  if(micros() - cycleStart > period) // restart timer
    cycleStart += period; 
     int freq  = int(gps.f_speed_mph());
     Serial.println(gps.f_speed_mph());
     freq = constrain(freq, 1, 200);
     period = 1000000 / freq; // 1 million / freq
     onTime = period / 2; // duty cycle (100 / 2 = 50)
     cycleStart = micros();
     Serial.println(freq);
  }
    
}

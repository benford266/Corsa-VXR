#include <SoftwareSerial.h>
#include <TinyGPS.h>


byte outPin = 10; // output pin
byte level = 0; // flag to track high/low level
unsigned long currentTime;  // could use micros also for more precise timing
unsigned long previousTime;
int freq = 30;
unsigned long halfPeriod = 500; // milliseconds, or use 50000 for microseconds
unsigned long elapsedTime;
unsigned long cycleStart, period = 100000UL, onTime = period / 2;


// Setup modules
TinyGPS gps;
//SoftwareSerial ss(64, 65); // RX / TX << pins to NEO-6M Arduino Mega2650
SoftwareSerial ss(8, 9); // RX / TX << pins to NEO-6M Arduino ProMicro


void setup(){
  Serial.begin(9600); // Debug serial uncomment if needed
  ss.begin(9600); // Begin serial comms to GPS 9600 baud
  Serial.println();
  pinMode (outPin, OUTPUT);
}


void loop(){
  bool newData = false;
  unsigned long chars;
  unsigned short sentences, failed;

  for (unsigned long start = millis(); millis() - start < 10;)
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
    Serial.println("No Pulse");
    freq = int(0);
  }
  else { // Above 1mph pulse is generated and 1Hz per MPH / 3600 pulses per Mile
    freq  = int(gps.f_speed_kmph());
    Serial.println(gps.f_speed_kmph());
    freq = constrain(freq, 1, 200);
    Serial.println(freq);
  }
  halfPeriod = (1000 / freq) / 2;
  currentTime = millis(); // capture the current time
  elapsedTime = currentTime - previousTime; // how much time elapsed since last pass thru loop
  Serial.println(halfPeriod);
  if (elapsedTime >= halfPeriod) { // time to change levels?
    previousTime = previousTime + halfPeriod; // set up for next level change
    level = 1 - level; // will change 1,0,1,0,1,0 ...
    digitalWrite (outPin, level);
  } // end time check

  
}

//GPS to Speedo Pulse
#include <SoftwareSerial.h>
#include <TinyGPS.h>

// Setup modules
TinyGPS gps;
SoftwareSerial ss(8, 9); // RX / TX <<

unsigned long cycleStart, period = 100000UL, onTime = period / 2;
const byte outPin = 13; // Output Pin to ECU / Speedo

void setup()
{
  Serial.begin(9600);
  ss.begin(9600); // Begin serial comms 9600 baud
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
      // Serial.write(c); // uncomment this line if you want to see the GPS data flowing
      if (gps.encode(c)) // Did a new valid sentence come in?
        newData = true;
    }
  }

  // Pulse Generator
  digitalWrite(outPin, micros() - cycleStart < onTime);
  if(micros() - cycleStart > period) // restart timer
    cycleStart += period; 
  
    int freq  = int(gps.f_speed_mph());
    freq = constrain(freq, 1, 200);
    period = 1000000 / freq; // 1 million / freq
    onTime = period / 2; // duty cycle (100 / 2 = 50)
    cycleStart = micros();
    Serial.println(freq);
}

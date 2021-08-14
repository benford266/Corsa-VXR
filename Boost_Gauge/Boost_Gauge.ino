// Project: Basic Boost Logger
// Author: Ben Ford
// Use: Log Boost pressure to serial console
// Arduino Pro Micro and MPX2650AP

//Define Pins used for Function
const int MapPin = A0; // Map sesnor pin


//Set other global variables
double MapKPA;// Initialise other PID variables
float CurKPA = MapKPA;
float PSI = CurKPA;
float Atmo = 0;

void setup() {
  Serial.begin(9600); //Open Serial Port
  Atmo = analogRead(MapPin) * (315.0 / 1023.0); //Read Atmospheric pressure and convert to KPA
}

void loop(){
  Boost();
  Output();
}

void Boost() {
  MapKPA = analogRead(MapPin) * (315.0 / 1023.0);  //Read map Pin and convert to KPA
}

void Output(){
    PSI = (MapKPA - Atmo) / 6.8;
    Serial.println(PSI); // Write PSI to serial 
    delay(100); // How often to poll
  
}

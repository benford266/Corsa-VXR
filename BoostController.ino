#include <PID_v1.h>
#include <TM1638plus.h>
//Define Pins used for Control
const int MapPin = A0; // Map sesnor pin
const int MacPin = 3; // Mac valve pin
const int WBPin = A5; // Wideband pin

// Define screen
#define  STROBE_TM 4 // strobe = GPIO connected to strobe line of module
#define  CLOCK_TM 6  // clock = GPIO connected to clock line of module
#define  DIO_TM 7 // data = GPIO connected to data line of module
bool high_freq = false; //default false, If using a high freq CPU > ~100 MHZ set to true. 
bool swap_nibbles = false; //Optional , Default is false if left out, see note in readme at URL
TM1638plus tm(STROBE_TM, CLOCK_TM , DIO_TM, high_freq);



//Set desired boost level and define Pid variables
double TargetBoost = 230; // 250kpa is target boost level
double MapKPA, PidPWM;// Initialise other PID variables
const long sampleRate = 15;// Pid sample rate

// Pid tuning parameters
const double Kp = 7; //Pid Proporional Gain. Initial ramp up i.e Spool, Lower if over boost
const double Ki = 20; //Pid Integral Gain. Overall change while near Target Boost, higher value means less change, possible boost spikes
const double Kd = 0; //Pid Derivative Gain. Not Sure what to do with it.....

//Load PID controller
PID myPID(&MapKPA, &PidPWM, &TargetBoost, Kp, Ki, Kd, DIRECT); //Initialize PID with our variables

//Set other global variables
unsigned long LeanTime; 
float Lean = 0;
float Cut = 0;
float AF = 0;
float Atmo = 0;
float Target = TargetBoost;
float CurKPA = MapKPA;

void setup() {
  //Serial.begin(9600); //debug uncomment to debug
  Atmo = analogRead(MapPin) * (315.0 / 1023.0); //Read Atmospheric pressure and convert to KPA
  //Pid Stuffs
  myPID.SetMode(AUTOMATIC);
  myPID.SetOutputLimits(0, 180); //Mac valve lower duty cycle limits 180 Closed, 0 Off
  myPID.SetSampleTime(sampleRate); //PID's sample rate in milliseconds
  TCCR2B = TCCR2B & B11111000 | B00000111; //Set pin 3 and 11 PWM frequency to 31hz
  pinMode(13, OUTPUT);
  // Start Screen
  tm.displayBegin(); 
}

void loop(){
  Function();
  Control();
}

void Function() {


  
  MapKPA = analogRead(MapPin) * (315.0 / 1023.0);  //Read map Pin and convert to KPA
  
  AF = ((analogRead(WBPin) * (5.0 / 1023.0)/0.625) + 10.0);//Read wideband and convert to a/f ratio


  if ((MapKPA < 100) || (Cut>0)) {//Keep valve from clicking when not in boost or cut boost if too lean
    PidPWM = 0;//Dont pulse if not building boost or wideband is lean
    //Serial.print("Boost Cut");//debug
    digitalWrite(13, HIGH);
   }
  else {
    myPID.Compute(); //Execute Pid and produce a pulse width if above 100kpa and not lean
    digitalWrite(13, LOW);
  }
  analogWrite(MacPin, PidPWM); //Energize mac valve

}

void Control(){

    // Define Buttons
  uint8_t buttons = tm.readButtons();
    // Display MAP
  tm.DisplayDecNumNibble(MapKPA, TargetBoost, true);

  // Buttons
  if ( buttons == 0x80 ){
    TargetBoost = TargetBoost + 10;
  }
  if ( buttons == 0x40 ){
    TargetBoost = TargetBoost - 10;
  } 

  
}


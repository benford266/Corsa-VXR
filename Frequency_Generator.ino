#include <TimerOne.h>

unsigned long t=1000,f,k=512;// default 1000 μs (1000 Hz), meander, pulse 

byte k1,kn,kn1,kn2;
int drive,drive0;

void setup()
{ 
  pinMode(9, OUTPUT);

}
void loop()
{
  Timer1.initialize(t); // period    
  Timer1.pwm(9, k); // k - fill factor 0-1023. 

 
  if(t==0 || t>300000){ // limiting the pulse duration to the minimum, if 


    t=1;
  }
  if(t>200000 && t<300000){ // limiting the pulse duration to the 




    t=200000;
  } 
  f=1000000/t; // calculate the frequency 
  k1=k*100/1024; // calculate% fill factor

  if(kn2==HIGH){// button for adjusting the fill factor (in a circle from 


    k=k+16;// step 16 out of 1024 (you can do 8 for smoother adjustment)
  }
  if(k==1024){
    k=0;
  }

}

/*
  Blink
  The basic Energia example.
  Turns on an LED on for one second, then off for one second, repeatedly.
  Change the LED define to blink other LEDs.
  
  Hardware Required:
  * LaunchPad with an LED
  
  This example code is in the public domain.
*/

// most launchpads have a red LED
#define SW 19

//see pins_energia.h for more LED definitions
//#define LED GREEN_LED
  
// the setup routine runs once when you press reset:
void setup() {                
  // initialize the digital pin as an output.
  pinMode(SW, OUTPUT);   
  digitalWrite(SW,LOW) ; 
}

// the loop routine runs over and over again forever:
void loop() {
  //digitalWrite(SW, HIGH);   // turn the LED on (HIGH is the voltage level)
  //delay(1000);               // wait for a second
  //digitalWrite(SW, LOW);    // turn the LED off by making the voltage LOW
  //delay(1000);               // wait for a second
}

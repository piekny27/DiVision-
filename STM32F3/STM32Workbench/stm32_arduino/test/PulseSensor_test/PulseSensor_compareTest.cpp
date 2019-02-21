#include <Arduino.h>
#include <Wire.h>
#include "MAX30100/src/MAX30100_PulseOximeter.h"
#define OKRES_WYSWIETLANIA 20
#define MAX_DIFF 10
#define USE_ARDUINO_INTERRUPTS true    // Set-up low-level interrupts for most acurate BPM math.
#include "PulseSensor/src/PulseSensorPlayground.h"     // Includes the PulseSensorPlayground Library.
const int PulseWire = A0;       // PulseSensor PURPLE WIRE connected to ANALOG PIN 0
const int LED13 = 13;          // The on-board Arduino LED, close to PIN 13.
int Threshold = 550;           // Determine which Signal to "count as a beat" and which to ignore.
uint8_t saturacja;
float tetno;
PulseOximeter max30100;
uint32_t poprzedniOdczyt = 0;
PulseSensorPlayground pulseSensor;
int inByte = 0;
void setup() {
  Serial.begin(115200); 
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  max30100.begin();
    if (pulseSensor.begin()) {
    Serial.println("We created a pulseSensor Object !");  //This prints one time at Arduino power-up,  or on Arduino reset.  
  }
}

void loop() {
  
    max30100.update();
    int myBPM = pulseSensor.getBeatsPerMinute();
    saturacja = max30100.getSpO2();
    tetno = max30100.getHeartRate();
    
    Serial.print(tetno);
   Serial.print("\t");
   Serial.print(myBPM);
   Serial.print("\t");
   Serial.println((myBPM+tetno)/2);
  // if((myBPM-tetno)>MAX_DIFF||(myBPM-tetno<-MAX_DIFF)){
  // Serial.print("Bledny odczyt");
  // Serial.println("");
  // Serial.print(tetno);
  // Serial.print("\t");
  // Serial.print(myBPM);
  // Serial.print("\t");
  // Serial.print((myBPM+tetno)/2);
   //Serial.println();
   //}
   //else{
   // Serial.print(tetno);
   // Serial.print("\t");
   // Serial.print(myBPM);
   // Serial.print("\t");
   // Serial.print((myBPM+tetno)/2);
   // Serial.println();
    
  }


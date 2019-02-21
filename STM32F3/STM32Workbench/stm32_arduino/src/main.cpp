#include <Arduino.h>
#include <Wire.h>
#include "MAX30100_main.h"                // modul pulsometru MAX30100



#define CzestotliwoscWysylania     500


//Zmienne ############################################################################

uint32_t ostatniCzas = 0;

//pulsometr MAX30100
PulseOximeter pox; //klasa modul pulseoximeter
MAX30100 sensor;   //klasa modul max30100
bool pulse1Status = false;
String pulse1Name = "0x57";
int pulse1Mode=2;

// FUNKCJE #############################################################################

// pulsometr MAX30100

void pulsometr1_init(){

  if (!pox.begin()) {

      Serial.println("Modul MAX30100... FAILED");
      pulse1Status = false;
    } else {
      Serial.println("Modul MAX30100... SUCCESS");
      pulse1Status = true;
  }
}

void pulsometr1_send() 
{
  if(pulse1Status){

    uint32_t sampStart;

    Serial.print(pulse1Name);
    Serial.print("|");
    Serial.print("success");
    Serial.print("|");

    switch (pulse1Mode){
      case 1: //pomiar bpm oraz SpO2

        sampStart = millis();

        Serial.print("mode1");
        Serial.print("|");
        Serial.print(pox.getHeartRate());  //Heart rate: x bpm
        Serial.print("|");
        Serial.println(pox.getSpO2());       //SpO2: x %

        while(!sensor.isTemperatureReady()) { //gdy ERROR

          if (millis() - sampStart > 1000) {
            //Serial.println("ERROR: PulseSensor1");
            pulse1Status = false;
            break;
          }
        }
        break;
      case 2: //pomiar temperatury

        sampStart = millis();

        Serial.print("mode2");
        Serial.print("|");
        
        sensor.startTemperatureSampling();

        while(!sensor.isTemperatureReady()) {   //gdy ERROR

          if (millis() - sampStart > 1000) {
            //Serial.println("ERROR: PulseSensor1");
            pulse1Status = false;
            break;
          }
        }

        float temperature = sensor.retrieveTemperature();
        if(pulse1Status){
          Serial.println(temperature);    //temp x C
        }else{
          Serial.println("0");            //temp error 
        }
        
        break;
    }

  }else{
    pulsometr1_init();
  }
}


// GLOWNE ###############################################################################

void setup()
{
    Serial.begin(115200);
    Serial.println("Inicjacja sterownika STM...");
    Serial.println("Inicjacja modulow...");

    pulsometr1_init();  //modul pulsometru MAX30100
      

}
void loop()
{
  pox.update(); //odswierzanie pulsometru MAX30100

    if (millis() - ostatniCzas > CzestotliwoscWysylania) {
        
        
        pulsometr1_send();    //pulsometr MAX30100


        //zapis czasu
        ostatniCzas = millis();
    }
}



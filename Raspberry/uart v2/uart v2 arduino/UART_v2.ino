/*
  Serial Event example

  When new serial data arrives, this sketch adds it to a String.
  When a newline is received, the loop prints the string and clears it.

  A good test for this is to try it with a GPS receiver that sends out
  NMEA 0183 sentences.

  NOTE: The serialEvent() feature is not available on the Leonardo, Micro, or
  other ATmega32U4 based boards.

  created 9 May 2011
  by Tom Igoe

  This example code is in the public domain.

  http://www.arduino.cc/en/Tutorial/SerialEvent
*/
String Sensor1 = "23C";
String Sensor2 = "60";
String Sensor3 = "96%";
String inputString = "";         // a String to hold incoming data
boolean stringComplete = false;  // whether the string is complete
String toMalina = "";
int packCont[6];
void setup() {
  // initialize serial:
    SerialUSB.begin(9600);
  Serial2.begin(9600);
  // reserve 200 bytes for the inputString:
  inputString.reserve(200);
}

void loop() {
  // print the string when a newline arrives:
  if (stringComplete) {
    SerialUSB.println(inputString);
    toMalina="111,"+Sensor1+","+Sensor2+","+Sensor3;
   
    // clear the string:
    inputString = "";
    if(toMalina.length()>15) Serial2.print(toMalina.length(),HEX);
    else{
      Serial2.print(0);
      Serial2.print(toMalina.length(),HEX);
      }
    
    Serial2.print(toMalina);
   SerialUSB.print(toMalina);
    stringComplete = false;
  }
}

/*
  SerialEvent occurs whenever a new data comes in the hardware serial RX. This
  routine is run between each time loop() runs, so using delay inside loop can
  delay response. Multiple bytes of data may be available.
*/
void serialEvent2() {
  while (Serial2.available()) {
    // get the new byte:
    char inChar = (char)Serial2.read();
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set a flag so the main loop can
    // do something about it:
    if (inChar == '\n') {
      stringComplete = true;
    }
  }
}

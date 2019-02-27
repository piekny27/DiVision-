#include <TinyGPS++.h>
static const uint32_t GPSBaud = 9600;
String Sensor1 = "23C";
String Sensor2 = "60";
String Sensor3 = "96%";
String inputString = "";         // a String to hold incoming data
String GPSString="";
boolean stringComplete = false;  // whether the string is complete
String toMalina = "";
int packCont[6];
static volatile unsigned long timestamp = 0;
TinyGPSPlus gps;  //inicjalizacja obiektu gps

void setup() {
  // initialize serial:
    SerialUSB.begin(9600);
    Serial1.begin(GPSBaud); //UART od gps
    Serial2.begin(9600);      // UART do RasPi
  // reserve 200 bytes for the inputString:
  inputString.reserve(200);
}

void loop() {
  // print the string when a newline arrives:
  if(millis()-timestamp>1000){
  while (Serial1.available())
      gps.encode(Serial1.read());
  timestamp=millis();
  }
  
  if (stringComplete) {
    SerialUSB.println(inputString); //wyświetlenie komunikatu z RasPi
    // clear the string:
    inputString = "";
    
    
    
    toMalina="1111,"+Sensor1+","+Sensor2+","+Sensor3+",";
    printInt(gps.satellites.value(), gps.satellites.isValid(), 6); //ilość połączonych satelitów
    printFloat(gps.hdop.hdop(), gps.hdop.isValid(), 6, 1);         //jakość połączenia <1 - ideal, 2-3 znakomity, 4-6 dobry, 7-8 umiarkowany, 9-20 słaby, >20 zły
    printFloat(gps.location.lat(), gps.location.isValid(), 11, 6); //szerokość
    printFloat(gps.location.lng(), gps.location.isValid(), 12, 6); //długość
   // printInt(gps.location.age(), gps.location.isValid(), 5);
    printDateTime(gps.date, gps.time);                             //czas GMT
    printFloat(gps.altitude.meters(), gps.altitude.isValid(), 7, 2);//wysokość m npm
    printFloat(gps.course.deg(), gps.course.isValid(), 7, 2);       //kierunek
    printFloat(gps.speed.kmph(), gps.speed.isValid(), 6, 2);        //prędkość kmph
   // printStr(gps.course.isValid() ? TinyGPSPlus::cardinal(gps.course.deg()) : "*** ", 6); //kierunek N/S/E/W
   // printInt(gps.failedChecksum(), true, 9); //suma kontrolna

    //wysyłanie długości ramki, max 256 bajtów
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

//funkcje dla GPS
static void smartDelay(unsigned long ms)
{
  unsigned long start = millis();
  do 
  {
    while (Serial1.available())
      gps.encode(Serial1.read());
  } while (millis() - start < ms);
}

static void printFloat(float val, bool valid, int len, int prec)
{
  char sz[32];
  if (!valid)
  {
    toMalina+="blad ";
  }
  else
  {
    sprintf(sz, "%f",val);
    toMalina+=sz;
  //  int vi = abs((int)val);
  //  int flen = prec + (val < 0.0 ? 2 : 1); // . and -
  //  flen += vi >= 1000 ? 4 : vi >= 100 ? 3 : vi >= 10 ? 2 : 1;
   // for (int i=flen; i<len; ++i)
     toMalina+=" ";
  }
  smartDelay(0);
}

static void printInt(unsigned long val, bool valid, int len)
{
  char sz[32];
  if (valid)
    sprintf(sz, "%ld", val);
  sz[len] = 0;
  for (int i=strlen(sz); i<len; ++i)
    sz[i] = ' ';
  if (len > 0) 
    sz[len-1] = ' ';
 toMalina+=sz;
  smartDelay(0);
}

static void printDateTime(TinyGPSDate &d, TinyGPSTime &t)
{
  if (!d.isValid())
  {
   // Serial2.print(F("********** "));
    toMalina+="BladDaty";
  }
  else
  {
    char sz[32];
    sprintf(sz, "%02d/%02d/%02d ", d.month(), d.day(), d.year());
   toMalina+=sz;
  }
  
  if (!t.isValid())
  {
    //Serial2.print(F("******** "));
    toMalina+="BladCzasu";
  }
  else
  {
    char sz[32];
    sprintf(sz, "%02d:%02d:%02d ", t.hour(), t.minute(), t.second());
    toMalina+=sz;
  }

 // printInt(d.age(), d.isValid(), 5);
  smartDelay(0);
}

static void printStr(const char *str, int len)
{
  int slen = strlen(str);
  for (int i=0; i<len; ++i)
    toMalina+=(i<slen ? str[i] : ' ');
  smartDelay(0);
}


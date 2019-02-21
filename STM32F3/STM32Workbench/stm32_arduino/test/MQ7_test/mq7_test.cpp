#include <Arduino.h>
#include "MQ7/MQ7.h"


MQ7 mq7(A0,5.0);

void setup() {
    Serial.begin(115200);
}

void loop() {
    Serial.println(mq7.getPPM());
    delay(1000);
}

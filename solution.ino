#include "SD_Helper.h"


void setup()
{
  Serial.begin(115200);
  ss.begin(GPSBaud);

  initTEM();
  initSD();
}

void loop()
{

    TemToSD();

    gpsToSD();
  

    smartDelay(1000);

    if (millis() > 5000 && gps.charsProcessed() < 10)
        DSerial::println(("No GPS data received: check wiring"));
}



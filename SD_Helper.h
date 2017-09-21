#ifndef __SD_Helper_h
#define __SD_Helper_h

#include "utils.h"
#include <SD.h>

#include "KAmodTEM.h"
#include "TWI.h"


const int chipSelect = 53;

unsigned int temperature = 0;

char fract[] = {'0','1','1','2','3','3','4','4','5','6','6','7','8','8','9','9'};
char str[8];

File logFile;

filePrinter logPrinter;

void initSD()
{
  
  DSerial::println("Beginning SD init");
  if(!SD.begin(chipSelect))
  {
    DSerial::println("Error initializing SD");
  }
  DSerial::println("Init finished");

  DSerial::println("Creating log file");

  if(SD.exists("log.txt"))
  {
    DSerial::println("File exists");
    SD.remove("log.txt");
  }
  else
  {
    DSerial::println("File doesn't exist, creating");
  }
  logFile = SD.open("log.txt", FILE_WRITE);

  logPrinter = filePrinter(&logFile);
}

void exitSD()
{
  DSerial::println("Closing log file");
  logFile.close();
}

/*void setup() {
  // put your setup code here, to run once:
  KAmodTEM_Init();  // 
  KAmodTEM_WriteReg(0, CONFIGURATION, 0b01100000); // 12-bit resolution
  Serial.begin(9600);

  initSD();
}*/

void initTEM()
{
  KAmodTEM_Init();
  KAmodTEM_WriteReg(0, CONFIGURATION, 0b01100000); // 12-bit resolution
}
//to-do: work out how temperature conversion works and change flushing
void TemToSD()
{
  temperature = KAmodTEM_ReadReg(0, TEMPERATURE);
  logFile.println("Temperature:");
  logFile.print(itoa(temperature >> 8, str, 10));
  logFile.print('.');
  temperature >>= 4 ;
  temperature &= 0x0F;
  logFile.print(fract[temperature]);
  logFile.print("C\n");
  logFile.flush();
}

void gpsToSD()
{
    logFile.println("TIME: ");
    logPrinter.printDateTime(gps.date, gps.time);
     logFile.print("\n");

    logFile.println("ALTITUDE: ");
    logPrinter.printFloat(gps.altitude.meters(), gps.altitude.isValid(), 7, 2);
     logFile.print("\n");

    logFile.println("COORDINATES: ");
    logPrinter.printFloat(gps.location.lng(), gps.location.isValid(), 12, 6);
    logFile.print(" ");
    logPrinter.printFloat(gps.location.lat(), gps.location.isValid(), 11, 6);
     logFile.print("\n");
}

#endif

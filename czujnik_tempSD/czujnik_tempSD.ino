#include <Narcoleptic.h>

#include <SD.h>

#include <KAmodTEM.h>
#include <TWI.h>


const int chipSelect = 53;

unsigned int temperature = 0;

unsigned int TLE=1; //time in minutes
unsigned int current_time = 0;
char fract[] = {'0','1','1','2','3','3','4','4','5','6','6','7','8','8','9','9'};
char str[8];

File logFile;

void initSD()
{
  Serial.println("Beginning SD init");
  if(!SD.begin(chipSelect))
  {
    Serial.println("Error initializing SD");
  }
  Serial.println("Init finished");

  Serial.println("Creating log file");

  if(SD.exists("log.txt"))
  {
    Serial.println("File exists");
    SD.remove("log.txt");
  }
  else
  {
    Serial.println("File doesn't exist, creating");
    
  }
  logFile = SD.open("log.txt", FILE_WRITE);
}

void exitSD()
{
  Serial.println("Closing log file");
  logFile.close();
}

void setup() {
  // put your setup code here, to run once:
  KAmodTEM_Init();  // 
  KAmodTEM_WriteReg(0, CONFIGURATION, 0b01100000); // 12-bit resolution
  Serial.begin(9600);

  initSD();
}

void loop() {
  // put your main code here, to run repeatedly:
  temperature = KAmodTEM_ReadReg(0, TEMPERATURE);
  logFile.print(itoa(temperature >> 8, str, 10));
  logFile.print('.');
  temperature >>= 4 ;
  temperature &= 0x0F;
  logFile.print(fract[temperature]);
  logFile.print("C\n");
  logFile.flush();
  /*if(current_time == TLE * 60000){
    exitSD();
    while(1);
  }*/
  Narcoleptic.delay(5000);
  current_time += 5000;
}

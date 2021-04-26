#include <Arduino.h>
#include <Wire.h>
#include <TFLI2C.h>
#include <stdint.h>

TFLI2C tfli2C;

static int16_t tfDist;                    // Distance in centimeters
static int16_t tfTemp;                    // Temperature in degrees Centigrade 
static int16_t tfFlux;                    // Confidence value 
static int16_t tfAddr = TFL_DEFAULT_ADDR; // Default I2C address
static uint16_t tfFrame;                  // Frame Rate

void setup()
{
   Wire.begin(); // Initialize the Wire library
   delay(1000);  // Wait for serial port monitor to come up. 
   Serial.begin(115200);
   Serial.println("TF-Luna Test  and I2C scanner");
   byte error, address;
   int nDevices = 0;
   Serial.println("\nScanning....");
   for (address = 1; address < 127; address++)
   {
      Wire.beginTransmission(address);
      error = Wire.endTransmission();
      if (error == 0)
      {
         Serial.print("I2C device found at address 0x");
         if (address < 16)
         {
            Serial.println("0");
         }
         Serial.println(address, HEX);
         nDevices++;
      }
      else if (error == 4)
      {
         Serial.print("Unknown error at address 0x");
         if (address < 16)
         {
            Serial.print("0");
         }
         Serial.println(address, HEX);
      }
   }
   if (nDevices == 0)
   {
      Serial.println("No I2C devices found\n");
   }
   else
   {
      Serial.println("done\n");
   }
   if (tfli2C.Get_Frame_Rate(tfFrame, tfAddr)) 
   {
      Serial.print("Frame rate: ");
      Serial.println(tfFrame);
   }
}

void loop()
{
   if (tfli2C.getData(tfDist, tfFlux, tfTemp, tfAddr)) // If read okay
   {
      char buffer[80];
      uint16_t tempwhole = tfTemp / 100;
      uint16_t tempfrac = tfTemp % 100;
      sprintf(buffer, "Distance: %4d, Flux: %4d, Temp: %2d.%02d\n", tfDist, tfFlux, tempwhole, tempfrac);
      Serial.print(buffer);
   } else {
      Serial.println("TF-luna getData error");
   }
   delay(500);
}
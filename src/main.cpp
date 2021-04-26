#include <Arduino.h>
#include <Wire.h>
#include <TFLI2C.h>
#include <stdint.h>

TFLI2C tfli2C;

static int16_t tfAddr = TFL_DEFAULT_ADDR; // Default I2C address

/* Setup the NodeMCU to enable the i2c and Serial port
*/

void setup()
{
   Wire.begin();           // Initialize the Wire library
   delay(1000);             // Wait for the serial port monitor to come up
   Serial.begin(115200);   // Start serial port with 115200 baud
}

void find_I2C()
{
   Serial.println("Find the I2C adresses");
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
         tfAddr = address;
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
}

void program_luna()
{
   char buffer[80];
   Serial.println("Program the I2C address.");
   Serial.println("Enter the new address. Hex numbers allowed. 0x08 .. 0x77");
   Serial.setTimeout(3000);
   Serial.readBytesUntil('\n', buffer, 5);
   uint16_t new_address = (uint16_t)strtol(buffer, NULL, 0);
   sprintf(buffer, "Read number: 0x%x", new_address);
   Serial.println(new_address);
   // Reset the serial timeout back to 1000 msec.
   Serial.setTimeout(1000);
   bool address_ok = true;
   if (new_address < 0x08) {
      Serial.println("The address is too low.");
      address_ok = false;
   }
   if (new_address > 0x77) {
      Serial.println("The address is too high");
      address_ok = false;
   }
   if (address_ok == true) {
      tfli2C.Set_I2C_Addr((uint8_t) new_address, tfAddr);
      tfli2C.Save_Settings(tfAddr);
      delay(200);
      tfli2C.Soft_Reset(tfAddr);
      tfAddr = new_address;
   }
}

void read_luna()
{
static uint16_t tfFrame;                  // Frame Rate
static int16_t tfDist;                    // Distance in centimeters
static int16_t tfTemp;                    // Temperature in degrees Centigrade 
static int16_t tfFlux;                    // Confidence value 
   if (tfli2C.Get_Frame_Rate(tfFrame, tfAddr)) 
   {
      Serial.print("Frame rate: ");
      Serial.println(tfFrame);
   }

   int index = 20;
   while(index > 0 ){
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
      index--;
   }
}

void luna_hard_reset(){
   Serial.println("Hard reset the TF-luna.");
   tfli2C.Hard_Reset(tfAddr);
}

void test_string()
{
   char buffer[80]; 
   sprintf(buffer, "The I2C address is 0x%xh\n", tfAddr);
   Serial.println(" Test the menu");
   Serial.println(buffer);
}

/* 
   Main program with a menu via the serial port
*/

void loop()
{
   Serial.println("----------------------------------------------------------------");
   Serial.println("The TF-luna configuration program");
   Serial.println("Choose one of the following options:");
   Serial.println(" 1: Find the connected i2c devices.");
   Serial.println(" 2: Check the measured distance and temperature for 10 seconds.");
   Serial.println(" 3: Reprogram the found device to a different address.");
   Serial.println(" 4: Hard reset the TF-Luna.");
   Serial.println(" 5: Simple test.");
   Serial.println("Select item:");

   while(!Serial.available()){}
   switch(Serial.read()) {
      case '1': find_I2C() ; break;
      case '2': read_luna(); break;
      case '3': program_luna(); break;
      case '4': luna_hard_reset(); break;
      case '5': test_string(); break;
      default: Serial.println(" - Invalid command. Try again.");
   }
}

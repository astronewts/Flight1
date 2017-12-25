/* MPU9250 Basic Example Code
 by: Kris Winer
 date: April 1, 2014
 license: Beerware - Use this code however you'd like. If you
 find it useful you can buy me a beer some time.
 Modified by Brent Wilkins July 19, 2016

 Demonstrate basic MPU-9250 functionality including parameterizing the register
 addresses, initializing the sensor, getting properly scaled accelerometer,
 gyroscope, and magnetometer data out. Added display functions to allow display
 to on breadboard monitor. Addition of 9 DoF sensor fusion using open source
 Madgwick and Mahony filter algorithms. Sketch runs on the 3.3 V 8 MHz Pro Mini
 and the Teensy 3.1.

 SDA and SCL should have external pull-up resistors (to 3.3V).
 10k resistors are on the EMSENSR-9250 breakout board.

 Hardware setup:
 MPU9250 Breakout --------- Arduino
 VDD ---------------------- 3.3V
 VDDI --------------------- 3.3V
 SDA ----------------------- A4
 SCL ----------------------- A5
 GND ---------------------- GND
 */

//#include "quaternionFilters.h"
//#include "MPU9250.h"
#include <Wire.h>

extern TwoWire Wire;

#define MPU9250_ADDRESS          0x68
#define MPU9250_ADDRESS_AD0_HIGH 0x69
#define MAG_ADDRESS              0x0C

#define GYRO_FULL_SCALE_250_DPS  0x00
#define GYRO_FULL_SCALE_500_DPS  0x08
#define GYRO_FULL_SCALE_1000_DPS 0x10
#define GYRO_FULL_SCALE_2000_DPS 0x18

#define ACC_FULL_SCALE_2_G       0x00
#define ACC_FULL_SCALE_4_G       0x08
#define ACC_FULL_SCALE_8_G       0x10
#define ACC_FULL_SCALE_16_G      0x18

#define AK8963_RA_HXL                   0x03
#define AK8963_RA_CNTL1                 0x0A
#define AK8963_RA_ASAX                  0x10

#define AK8963_MODE_POWERDOWN           0x0
#define AK8963_MODE_SINGLE              0x1
#define AK8963_MODE_CONTINUOUS_8HZ      0x2
#define AK8963_MODE_EXTERNAL            0x4
#define AK8963_MODE_CONTINUOUS_100HZ    0x6
#define AK8963_MODE_SELFTEST            0x8
#define AK8963_MODE_FUSEROM             0xF

#define AHRS false         // Set to false for basic data read
#define SerialDebug true  // Set to true to get Serial output for debugging

// Pin definitions
int intPin = 12;  // These can be changed, 2 and 3 are the Arduinos ext int pins
int myLed  = 13;  // Set up pin 13 led for toggling
byte c = 0;
byte d = 0;


void setup()
{
  Wire.begin();
  // TWBR = 12;  // 400 kbit/sec I2C speed
  Serial.begin(115200);

  // Set up the interrupt pin, its set as active high, push-pull
  pinMode(intPin, INPUT);
  digitalWrite(intPin, LOW);
  pinMode(myLed, OUTPUT);
  digitalWrite(myLed, HIGH);

  I2Cscan();

  // Read the WHO_AM_I register, this is a good test of communication
  Wire.requestFrom(MPU9250_ADDRESS, 1);
  if(Wire.available())    // slave may send less than requested
  { 
    c = Wire.read();    // receive a byte as character
    Serial.print(c);         // print the character
  }
  Serial.print(F("MPU9250 I AM 0x"));
  Serial.print(c, HEX);
  Serial.print(F(" I should be 0x"));
  Serial.println(0x71, HEX);
  delay(2000);

  if (c == 0x71) // WHO_AM_I should always be 0x71
  {
    Serial.println(F("MPU9250 is online..."));
  }
  
  // Read the WHO_AM_I register of the magnetometer, this is a good test of
  // communication
  Wire.requestFrom(MAG_ADDRESS, 1);
  if(Wire.available())    // slave may send less than requested
  { 
    d = Wire.read();    // receive a byte as character
    Serial.print(d);         // print the character
  }

  Serial.print("AK8963 ");
  Serial.print("I AM 0x");
  Serial.print(d, HEX);
  Serial.print(" I should be 0x");
  Serial.println(0x48, HEX);

  delay(2000);

  if (d != 0x48)
  {
    // Communication failed, stop here
    Serial.println(F("Communication failed, abort!"));
    Serial.flush();
    abort();
  }
  else
  {
    Serial.println("It maybe worked!!!");
  }

  
}

void I2Cscan()
 {
// scan for i2c devices
 byte error, address;
 int nDevices;

 Serial.println("Scanning...");

 nDevices = 0;
 for(address = 1; address < 127; address++ ) 
{
  // The i2c_scanner uses the return value of
   // the Write.endTransmisstion to see if
  // a device did acknowledge to the address.
  Wire.beginTransmission(address);
  error = Wire.endTransmission();

  if (error == 0)
  {
      Serial.print("I2C device found at address 0x");
    if (address<16) 
      Serial.print("0");
    Serial.print(address,HEX);
    Serial.println("  !");

    nDevices++;
 }
  else if (error==4) 
{
   Serial.print("Unknow error at address 0x");
   if (address<16) 
    Serial.print("0");
   Serial.println(address,HEX);
 }    
 }
  if (nDevices == 0)
  Serial.println("No I2C devices found\n");
  else
  Serial.println("done\n");

}

void loop()
{

}

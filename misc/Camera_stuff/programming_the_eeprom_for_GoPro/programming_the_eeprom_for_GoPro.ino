#include <Wire.h>

/* 
    https://forum.sparkfun.com/viewtopic.php?f=32&t=25140
    
    Wire Hookup for 24LC01B 1 K-bit (128 byte) EEPROM
    Don't forget to read the datasheet!
    http://ww1.microchip.com/downloads/en/devicedoc/21711c.pdf
    1,2,3 are not internally connected.
    It's okay to leave them open.
    
              -----
            -|1   8|- Arduino +5V
            -|2   7|- Jumper to +5V for Write Protect / <LC: GND TO WRITE TO IT>
            -|3   6|- Arduino Pin: SCL
Arduino Gnd -|4   5|- Arduino Pin: SDA
              -----
              
    I put a 2.2K pull-up resistor between pin 5 and 8
    because the datasheet suggested it, but I'm not entirely
    sure it's necessary as I think the Wire library enables
    the Arduino's internal pull-ups.  Don't quote me on that,
    though. <LC NOTE: I did this but with a 1k>

    You're supposed to be able to write 8 bytes at
    a time, but I had trouble with that.  Try 4 at
    a time with a delay and then push it up until
    your results degrade.
    
    It seems like you can read as many as you like.
    
    We're going to write Hello World!
    
Data       |H|e|l|l|o| |W|o|r|l|d|!|
           |-|-|-|-|-|-|-|-|-|-|-|-|
Address    |0|1|2|3|4|5|6|7|8|9|1|1|
           | | | | | | | | | | |0|1|

<LC: Astronewts Edits for GoPro>
*/

void setup()
{
  Wire.begin(); // join i2c bus (address optional for master)
  Serial.begin(9600);
}

void loop()
{
  //Serial.println("Writing...");
  
  //Wire.beginTransmission(0x50); // This is the 24LC01B device address
  //Wire.write(0x0);               // Start writing at address 0
  //Wire.write(0x09);              // Send 4 bytes of '0x09' to show its GoPro Compatible
  //Wire.endTransmission();       
  //delay(10);                    // Without a short delay, the EEPROM is still
                                // writing when you start to write the next block
                                // Feel free to experiment with the delay length

  Serial.println("Reading...");
  Wire.beginTransmission(0x50); // Now we're going to read it back
  Wire.write(0x0);               // Sending address 0, so it knows where we'll want
  //Wire.read(0x0);
  Wire.endTransmission();       // to read from

  Wire.requestFrom(0x50,4);     // Start new transmission and keep reading for 12 bytes
  if(Wire.available())    
  { 
    //char c = ;    // Read a byte and write it out to the Serial port
    Serial.print(Wire.read(), DEC);         
  } 
  Serial.println();

  delay(5000);
}

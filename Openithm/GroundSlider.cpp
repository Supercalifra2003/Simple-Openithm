#include "Arduino.h"
#include <Wire.h>
#include "GroundSlider.h"

int pinRequest = 0x00;
int16_t GroundSlider::getKeysData(int address)
{
  //mpr1fff21 address (0x5a or 0x5b)
  Wire.beginTransmission(address);
  Wire.write(pinRequest); //might change to read only 8 pins due to 8+8 layout
  Wire.endTransmission();
  Wire.requestFrom(address, 2, true);
  /*request data from 0x5a, 2 consecutive bytes, and stop message,
  release the bus for the master device have multiple transmission*/  
  
  return (Wire.read() | Wire.read() << 8) & 0b0000111111111111;
  /*Wire.read(): read the next byte transmitted; two times
  second byte shift to the left 8 times
  use bitwise AND to mask the unwanted bits*/
}

int setting1=0x80;
int setting2=0x63;
int setting3=0x5E;
int setting4=0x0C;
void GroundSlider::setupI2C(int address)
{
  Wire.beginTransmission(address); //start sending data bits to address 0x5b and 0x5a
  Wire.write(setting1); //send/write 0x80 and 0x63 as the settings for mpr121
  Wire.write(setting2);
  Wire.endTransmission(); //end sending data bits

  Wire.beginTransmission(address);
  Wire.write(setting3);
  Wire.write(setting4);
  Wire.endTransmission();
}

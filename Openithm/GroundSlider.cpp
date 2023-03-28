#include "GroundSlider.h"
#include "Arduino.h"
#include <Wire.h>

int pinRequest = 0x00;
int16_t GroundSlider::getKeysData(int address) {
  Wire.beginTransmission(address);
  Wire.write(pinRequest); 
  Wire.endTransmission();
  Wire.requestFrom(address, 1, true);

  return (Wire.read());
}

void GroundSlider::MPR121_begin(int address, int electrodesNumber){
  I2CWrite(address, MPR121_SOFTRESET, 0x63);
  
  I2CWrite(address, MPR121_ECR, 0x0);

  setSens(address, electrodesNumber, 1, 6); //default 12 6
  
  settingsI2C(address);

  I2CWrite(address, MPR121_ECR, electrodesNumber);
}

void GroundSlider::I2CWrite(int address, int command, int data){
  Wire.beginTransmission(address);
  Wire.write(command);
  Wire.write(data);
  Wire.endTransmission();
}
void GroundSlider::setSens(int address, int electrodesNumber, int touch, int release){
  for (int i = 0; i < electrodesNumber; i++) {
     I2CWrite(address, 0x41 + 2 * i, touch);
     I2CWrite(address, 0x42 + 2 * i, release);
  }
}
void GroundSlider::settingsI2C(int address){
  //sucreto setting is describe the the comments, might need to change to it
  I2CWrite(address, MPR121_MHDR, 0x01); 
  I2CWrite(address, MPR121_NHDR, 0x01);  
  I2CWrite(address, MPR121_NCLR, 0x0E); //0x10
  I2CWrite(address, MPR121_FDLR, 0x00);
     
  I2CWrite(address, MPR121_MHDF, 0x01);
  I2CWrite(address, MPR121_NHDF, 0x05); //0x01
  I2CWrite(address, MPR121_NCLF, 0x01); //0x08
  I2CWrite(address, MPR121_FDLF, 0x00); //0x08

  I2CWrite(address, MPR121_NHDT, 0x00);
  I2CWrite(address, MPR121_NCLT, 0x00);
  I2CWrite(address, MPR121_FDLT, 0x00);

  I2CWrite(address, MPR121_DEBOUNCE, 0);
  I2CWrite(address, MPR121_CONFIG1, 0x10);
  I2CWrite(address, MPR121_CONFIG2, 0x20); //0x20 ~32
}


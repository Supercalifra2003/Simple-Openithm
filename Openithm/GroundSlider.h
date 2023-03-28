/*for ground slider*/
#ifndef GroundSlider_h
#define GroundSlider_h

#include "Arduino.h"
#include <Wire.h>
#include "enums.h"

class GroundSlider
{
    public:
      int16_t getKeysData(int address);
      void MPR121_begin(int address, int electrodesNumber);
    private:
      void I2CWrite(int address, int command, int data);
      void setSens(int address, int electrodesNumber, int touch, int release);
      void settingsI2C(int address);
};

#endif

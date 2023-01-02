/*for ground slider*/
#ifndef GroundSlider_h
#define GroundSlider_h

#include "Arduino.h"
#include <Wire.h>

class GroundSlider
{
    public:
        int16_t getKeysData(int address);
        void setupI2C(int address);
    private:
};

#endif

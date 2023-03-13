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
        int pinRequest;
        int setting1; //todo describe each setting, for now no need
        int setting2; 
        int setting3;
        int setting4;
};

#endif

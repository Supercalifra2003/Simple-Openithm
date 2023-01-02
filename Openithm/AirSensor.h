#ifndef AirSensor_h
#define AirSensor_h

#include "Arduino.h"
#include "PinConfig.h"

class AirSensor
{
    public:
        bool getAirStates(int sensor);
        bool airStates[6];
        void calibrateLED();
    private:
        byte photoDiode[6];
        int _fluctuationValue;
        int _calibrateValue[6];
        void controlLED(byte LED);
};

#endif

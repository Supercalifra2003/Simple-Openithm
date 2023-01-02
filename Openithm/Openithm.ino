#include <Keyboard.h>
#include <Wire.h>

#include "AirSensor.h"
#include "GroundSlider.h"

GroundSlider slider;
AirSensor sensor;


int airKeys[6] = { '.', '/', ';', '\'', '[', ']'};
char groundKeys[16] = {
  'a', 'z', 's', 'x', 'd', 'c', 'f', 'v',
  'g', 'b', 'h', 'n', 'j', 'm', 'k', ','
};
int16_t data1;
int16_t data2;


void setup() {
    Wire.begin();
    slider.setupI2C(0x5A);
    slider.setupI2C(0x5B);
    delay(10);//ensure mpr121 booted
    Keyboard.begin();

}

void loop() {
    data1 = slider.getKeysData(0x5A);
    data2 = slider.getKeysData(0x5B);
    for (int i=0; i<8; i++)
    {
        byte bit1 = bitRead(data1, i);
        byte bit2 = bitRead(data2, i);
        (bit1 = 1) ? Keyboard.press(groundKeys[i]) : Keyboard.release(groundKeys[i]);
        (bit2 = 1) ? Keyboard.press(groundKeys[i]) : Keyboard.release(groundKeys[i]);
    }

    for (int i=0; i<6; i++)
    {
        if (sensor.getAirStates(i)){
            Keyboard.press(airKeys[i]);
        }
        else{
            Keyboard.release(airKeys[i]);
        }
    }
}

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
int16_t data1 = 0;
int16_t data2 = 0;
int electrodesNumber = 8;


void setup() {
  Wire.begin();
  Wire.setClock(400000L); //run clock at 400kHz
  Keyboard.begin();
  slider.MPR121_begin(0x5A, electrodesNumber);
  slider.MPR121_begin(0x5C, electrodesNumber);
  sensor.setupLED();
  delay(3000); //wait for complete setup and upload time incase error
}

void loop() {
  data1 = slider.getKeysData(0x5A);
  data2 = slider.getKeysData(0x5C);

  for (int i = 0; i < 8; i++) {
    (bitRead(data1, i) == 1) ? Keyboard.press(groundKeys[i]) : Keyboard.release(groundKeys[i]);
    (bitRead(data2, i) == 1) ? Keyboard.press(groundKeys[i+8]) : Keyboard.release(groundKeys[i+8]);
  }
  
  for (int i = 0; i < 6; i++){
    (sensor.getAirStates(i)) ? Keyboard.press(airKeys[i]) : Keyboard.release(airKeys[i]);
  }
}

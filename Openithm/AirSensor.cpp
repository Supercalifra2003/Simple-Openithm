#include "Arduino.h"
#include "AirSensor.h"
#include "PinConfig.h"

int _fluctuationValue = 30;
int _calibrateValue[6];
byte photoDiode[6] = {diode_0, diode_1, diode_2, diode_3, diode_4, diode_5};
bool airStates[6];

bool AirSensor::getAirStates(int sensor){
    controlLED(sensor);
    delay(3);
    if ((analogRead(photoDiode[sensor])) < (_calibrateValue[sensor] - _fluctuationValue)) {
        airStates[sensor] = true;
    }
    else {
        airStates[sensor] = false;
    }
    return airStates[sensor];
}

void AirSensor::calibrateLED(){
  for(int i=0; i<6; i++){
    controlLED(i);
    delay(200);
    _calibrateValue[i]=analogRead(photoDiode[i]);
  }
}

void AirSensor::controlLED(byte LED){
    switch (LED)
  {
    case 0:
      pinMode(LED_0, OUTPUT);
      pinMode(LED_1, OUTPUT);
      pinMode(LED_2, INPUT);

      digitalWrite(LED_0, HIGH);
      digitalWrite(LED_1, LOW);
      digitalWrite(LED_2, LOW);
      break;
    case 1:
      pinMode(LED_0, OUTPUT);
      pinMode(LED_1, OUTPUT);
      pinMode(LED_2, INPUT);

      digitalWrite(LED_0, LOW);
      digitalWrite(LED_1, HIGH);
      digitalWrite(LED_2, LOW);
      break;
    case 2:
      pinMode(LED_0, INPUT);
      pinMode(LED_1, OUTPUT);
      pinMode(LED_2, OUTPUT);

      digitalWrite(LED_0, LOW);
      digitalWrite(LED_1, HIGH);
      digitalWrite(LED_2, LOW);
      break;
    case 3:
      pinMode(LED_0, INPUT);
      pinMode(LED_1, OUTPUT);
      pinMode(LED_2, OUTPUT);

      digitalWrite(LED_0, LOW);
      digitalWrite(LED_1, LOW);
      digitalWrite(LED_2, HIGH);
      break;
    case 4:
      pinMode(LED_0, OUTPUT);
      pinMode(LED_1, INPUT);
      pinMode(LED_2, OUTPUT);

      digitalWrite(LED_0, HIGH);
      digitalWrite(LED_1, LOW);
      digitalWrite(LED_2, LOW);
      break;
    case 5:
      pinMode(LED_0, OUTPUT);
      pinMode(LED_1, INPUT);
      pinMode(LED_2, OUTPUT);

      digitalWrite(LED_0, LOW);
      digitalWrite(LED_1, LOW);
      digitalWrite(LED_2, HIGH);
      break;
  }
}

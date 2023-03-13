#include <Wire.h>

int16_t data1 = 0;
int16_t data2 = 0;

char keys[16] = {
  'a', 'z', 's', 'x', 'd', 'c', 'f', 'v',
  'g', 'b', 'h', 'n', 'j', 'm', 'k', ','
};


void setup() {
  Wire.begin();
  Serial.begin(9600);

  setupI2C(0x5A);
}

void loop() {
  data1 = getKeysData(0x5a);
  Serial.println(data1, BIN);
}

void setupI2C (int address){
    Wire.beginTransmission(address);
    Wire.write(0x80);
    Wire.write(0x63);
    Wire.endTransmission();

    Wire.beginTransmission(address);
    Wire.write(0x5E);
    Wire.write(0x0C);
    Wire.endTransmission();
  }

int getKeysData (int address){
    Wire.beginTransmission(address);
    Wire.write(0x9);
    Wire.endTransmission();
    Wire.requestFrom(address, 1, true);

    return (Wire.read());
  }

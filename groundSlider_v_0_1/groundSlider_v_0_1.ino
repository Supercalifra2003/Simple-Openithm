#include <Wire.h>
#include <Keyboard.h>
int16_t data1 = 0; //output data of mpr121(0x5a)
int16_t data2 = 0; //output data of mpr121(0x5b)
char keys[16] = {
  'a', 'z', 's', 'x', 'd', 'c', 'f', 'v',
  'g', 'b', 'h', 'n', 'j', 'm', 'k', ','
};


void setup() {
  Wire.begin(); //initialize Wire.h lib
  Serial.begin(9600); //initialize serial with 9600 baud rate
  
  setupI2C(0x5A);
  setupI2C(0x5B);
  delay(3000); //wait for complete setup
  Serial.println("SETUP COMPLETE"); 
}

void loop() {
  //mpr121 0x5A
  data1 = getKeysData(0x5A);
  Serial.println(data1, BIN);
  //print data1 as binary

  //mpr121 0x5B
  data2 = getKeysData(0x5B);
  Serial.println(data2, BIN);
  //print data2 as binary

  /*data1 and data 2 to keyboard
  use for loop to loop through the digits of binary
  press(keys[i]) for 1, release ONLY when 0
  Or use bitRead
  to do: ADD RAW HID*/
  
//  for(int i=0; i<8; i++)
//  {
//    (bitRead(data1, i) == 1) ? Keyboard.press(keys[i]) : Keyboard.release(keys[i]);
//    (bitRead(data2, i) == 1) ? Keyboard.press(keys[i+8]) : Keyboard.release(keys[i+8]);
//  }

  delay(5);
}

/*int returnKey(){
    int i = 0;
  do {    
    (bitRead(data1, i) == 1) ? Keyboard.press(keys[i]) : Keyboard.release(keys[i]);
    (bitRead(data2, i) == 1) ? Keyboard.press(keys[i+8]) : Keyboard.release(keys[i+8]);
  }
  while (i < 8);
  return i;
}
*/

void setupI2C (int address){
  Wire.beginTransmission(address); //start sending data bits to address 0x5b and 0x5a
  Wire.write(0x80); //send/write 0x80 and 0x63 as the settings for mpr121
  Wire.write(0x63);
  Wire.endTransmission(); //end sending data bits

  Wire.beginTransmission(address);
  Wire.write(0x5E);
  Wire.write(0x0C);
  Wire.endTransmission();
}

int getKeysData (int address){
  //mpr1fff21 address (0x5a or 0x5b)
  Wire.beginTransmission(address);
  Wire.write(0x00); //might change to read only 8 pins due to 8+8 layout
  Wire.endTransmission();
  Wire.requestFrom(address, 2, true);
  /*request data from 0x5a, 2 consecutive bytes, and stop message,
  release the bus for the master device have multiple transmission*/  
  
  return (Wire.read() | Wire.read() << 8) & 0b0000111111111111;
  /*Wire.read(): read the next byte transmitted; two times
  second byte shift to the left 8 times
  use bitwise AND to mask the unwanted bits*/
}

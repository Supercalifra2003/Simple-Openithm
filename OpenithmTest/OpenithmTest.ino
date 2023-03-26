#include <Wire.h>
#include <Keyboard.h>

enum MPR121Register {
  MPR121_SOFTRESET = 0x80,
  MPR121_TOUCHSTATUS_L = 0x00,
  MPR121_TOUCHSTATUS_H = 0x01,
  MPR121_FILTDATA_0L = 0x04,
  MPR121_FILTDATA_0H = 0x05,
  MPR121_BASELINE_0 = 0x1E,
  MPR121_MHDR = 0x2B,
  MPR121_NHDR = 0x2C,
  MPR121_NCLR = 0x2D,
  MPR121_FDLR = 0x2E,
  MPR121_MHDF = 0x2F,
  MPR121_NHDF = 0x30,
  MPR121_NCLF = 0x31,
  MPR121_FDLF = 0x32,
  MPR121_NHDT = 0x33,
  MPR121_NCLT = 0x34,
  MPR121_FDLT = 0x35,

  MPR121_TOUCHTH_0 = 0x41,
  MPR121_RELEASETH_0 = 0x42,
  MPR121_DEBOUNCE = 0x5B,
  MPR121_CONFIG1 = 0x5C,
  MPR121_CONFIG2 = 0x5D,
  MPR121_CHARGECURR_0 = 0x5F,
  MPR121_CHARGETIME_1 = 0x6C,
  MPR121_ECR = 0x5E,
  MPR121_AUTOCONFIG0 = 0x7B,
  MPR121_AUTOCONFIG1 = 0x7C,
  MPR121_UPLIMIT = 0x7D,
  MPR121_LOWLIMIT = 0x7E,
  MPR121_TARGETLIMIT = 0x7F,
};

int electrodesNumber = 0x08; // 8 electrodes
//0x0C for default 12 electrodesNumber 
//change this for 8 electodes per MPR121


int16_t data1 = 0; //output data of mpr121(0x5a)
int16_t data2 = 0; //output data of mpr121(0x5b)

char keys[16] = {
  'a', 'z', 's', 'x', 'd', 'c', 'f', 'v',
  'g', 'b', 'h', 'n', 'j', 'm', 'k', ','
};

byte photoDiode[6] = {A0, A1, A2, A3, A6, A7};
const byte LED_0 = 7;
const byte LED_1 = 8;
const byte LED_2 = 9;

int fluctuationValue = 50;
int calibrateValue[6];
char airKeys[6] = { '.', '/', ';', '\'', '[', ']'};

void setup() {
  Wire.begin(); //initialize Wire.h lib
  Wire.setClock(400000L); //run clock at 400kHz
//  Serial.begin(115200); //initialize serial with 9600 baud rate
  Keyboard.begin();
  for (int i = 0; i < 6; i++){
      pinMode(photoDiode[i], INPUT);
      calibrateValue[i] = calibrateLED(i);
    }
//  setupI2C(0x5A, electrodesNumber);
//  setupI2C(0x5B, electrodesNumber);
  MPR121_begin(0x5A, electrodesNumber);
  MPR121_begin(0x5B, electrodesNumber);
  delay(3000); //wait for complete setup
  Serial.println("SETUP COMPLETE"); 
}

void loop() {
  //mpr121 0x5A
  data1 = getKeysData(0x5A);
  //Serial.println(data1, BIN);
  //print data1 as binary

  //mpr121 0x5B
  data2 = getKeysData(0x5B);
  //Serial.println(data2, BIN);
  //print data2 as binary

  /*data1 and data 2 to keyboard
  use for loop to loop through the digits of binary
  press(keys[i]) for 1, release ONLY when 0
  Or use bitRead
  to do: ADD RAW HID*/
  
  for(int i=0; i<8; i++)
  {
    (bitRead(data1, i) == 1) ? Keyboard.press(keys[i]) : Keyboard.release(keys[i]);
    (bitRead(data2, i) == 1) ? Keyboard.press(keys[i+8]) : Keyboard.release(keys[i+8]);
  }

  for (int i = 0; i < 6; i++) {
    controlLED(i); //turn on corresponding IRLED
    delay(2); //to preven read before turn on (as describe in other Openithm)
    int diodeValue = analogRead(photoDiode[i]);
//      Serial.println("IR number");
//      Serial.print(i);
//      Serial.print(" is ");
//      Serial.print("|");
//      Serial.print(diodeValue);
//      Serial.print("   ");
    if (diodeValue < (calibrateValue[i] - fluctuationValue)) {
      Keyboard.press(airKeys[i]);
//          Serial.print(airKeys[i]); 
//          Serial.print(" press    |");
        }
     else {
      Keyboard.release(airKeys[i]);
//          Serial.print(airKeys[i]);
//          Serial.print(" release  |");
        }
  }
//  Serial.println();  
//  delay(5);
}


/*void setupI2C (int address, int electrodesNumber){
//  Wire.beginTransmission(address); //start sending data bits to address 0x5b and 0x5a
//  Wire.write(0x80); //send/write 0x80 and 0x63 as the settings for mpr121
//  Wire.write(0x63); //0x80 and 0x63 is soft reset 
//  Wire.endTransmission(); //end sending data bits
  I2CWrite(address, 0x80, 0x63);

//  mpr121AutoConfig(address); //for auto config
  settingsI2C(address); //for calibrate profile 

//  Wire.beginTransmission(address);
//  Wire.write(0x5E); // ECR mode, refer to section 11 
//  Wire.write(electrodesSetting); // setting 1100
//  Wire.endTransmission();
  I2CWrite(address, MPR121_ECR, electrodesNumber);
}
*/
void mpr121AutoConfig(int address){
  //set auto config, works for now but might need to fine tune for better uses
  I2CWrite(address, MPR121_AUTOCONFIG0, 0x0B); 
  //correct value for Vdd=3.3 according to Adafruit mpr121 library
  //set upper limit
  I2CWrite(address, MPR121_UPLIMIT, 200); //202
  
  //set tartget limit
  I2CWrite(address, MPR121_TARGETLIMIT, 180); //182
  
  //set low limit
  I2CWrite(address, MPR121_LOWLIMIT, 130); //131
  }

void settingsI2C(int address){
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

void MPR121_begin(int address, int electrodesNumber){
  I2CWrite(address, MPR121_SOFTRESET, 0x63);
  
  I2CWrite(address, MPR121_ECR, 0x0);

  setSensitivity(address, 12, 6); //default touch and release threshold
  
  settingsI2C(address);

  I2CWrite(address, MPR121_ECR, electrodesNumber);
  
}

void I2CWrite(int address, int command, int data){
  Wire.beginTransmission(address);
  Wire.write(command);
  Wire.write(data);
  Wire.endTransmission();
  }

void setSensitivity(int address, int touch, int release){
  for (int i = 0; i < electrodesNumber; i++) {
       I2CWrite(address, 0x41 + 2 * i, touch);
       I2CWrite(address, 0x42 + 2 * i, release);
       }
}


int getKeysData (int address){
  //mpr1fff21 address (0x5a or 0x5b)
  Wire.beginTransmission(address);
  Wire.write(0x00); //read the 0x00 byte 
  Wire.endTransmission();
  Wire.requestFrom(address, 1, true); //change the second parameter to 2 if more than 8 electrodesNumber
  /*request data from 0x5a, 2 consecutive bytes, and stop message,
  release the bus for the master device have multiple transmission*/  
  
//  return (Wire.read() | Wire.read() << 8) & 0b0000111111111111;
  /*Wire.read(): read the next byte transmitted; two times
  second byte shift to the left 8 times
  use bitwise AND to mask the unwanted bits*/
  //use this if more than 8 electrodesNumber
  return (Wire.read());
}

int calibrateLED (int LED) {
  controlLED(LED);
  delay(200);
//  Serial.println("calibrate value ");
//  Serial.print(LED);
//  Serial.print(analogRead(photoDiode[LED]));
  return analogRead(photoDiode[LED]);
}

//turn on turn off each LED
//todo: direct port manipulation for much faster (but less safe) manipulation
/* example on atmega328
   pinMode:
  DDRD |= _BV (2); // pinMode (2, OUTPUT);
  DDRD |= _BV (3); // pinMode (3, OUTPUT);
  DDRD |= _BV (4); // pinMode (4, OUTPUT);

  DDRD &= ~_BV (2); // pinMode (2, INPUT);
  DDRD &= ~_BV (3); // pinMode (3, INPUT);
  DDRD &= ~_BV (4); // pinMode (4, INPUT);

    digitalWrite
  PORTD &= ~_BV (2); // digitalWrite (2, LOW);
  PORTD &= ~_BV (3); // digitalWrite (3, LOW);
  PORTD &= ~_BV (4); // digitalWrite (4, LOW);

  PORTD |= _BV (2); // digitalWrite (2, HIGH);
  PORTD |= _BV (3); // digitalWrite (3, HIGH);
  PORTD |= _BV (4); // digitalWrite (4, HIGH);
*/
void controlLED (byte LED) {
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

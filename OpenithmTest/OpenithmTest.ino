#include <Wire.h>
#include <Keyboard.h>

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
  setupI2C(0x5A, electrodesNumber);
  setupI2C(0x5B, electrodesNumber);
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
  
  
 // delay(5);
}


void setupI2C (int address, int electrodesSetting){
  Wire.beginTransmission(address); //start sending data bits to address 0x5b and 0x5a
  Wire.write(0x80); //send/write 0x80 and 0x63 as the settings for mpr121
  Wire.write(0x63); //0x80 and 0x63 is soft reset 
  Wire.endTransmission(); //end sending data bits
  
  //set auto config, works for now but might need to fine tune for better uses
  Wire.beginTransmission(address);
  Wire.write(0x7B);
  Wire.write(0x0B);
  Wire.endTransmission();
 
  //correct value for Vdd=3.3 according to Adafruit mpr121 library
  //set upper limit
  Wire.beginTransmission(address);
  Wire.write(0x7D);
  Wire.write(200);
  Wire.endTransmission();
  
  //set tartget limit
  Wire.beginTransmission(address);
  Wire.write(0x7F);
  Wire.write(180);
  Wire.endTransmission();
  
  //set low limit
  Wire.beginTransmission(address);
  Wire.write(0x7E);
  Wire.write(130);
  Wire.endTransmission();

  Wire.beginTransmission(address);
  Wire.write(0x5E); // ECR mode, refer to section 11 
  Wire.write(electrodesSetting); // setting 1100
  Wire.endTransmission();
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
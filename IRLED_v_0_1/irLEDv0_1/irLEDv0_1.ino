#include <Keyboard.h>

//photodiode input pins
byte photoDiode[6] = {A0, A1, A2, A3, A6, A9}; //from bottom up

//charliplexed IR transmiter output pins
const byte LED_0 = 6;
const byte LED_1 = 7;
const byte LED_2 = 8;
//
//const byte delayButton =  14;
//const byte keyboardStop = 16;

/*todo: the value of IR fluctuate alot,
   need to find way to fluctuation range to be pressed properly
   or may result in random keyboard tapping
*/
int fluctuationValue=50;
int calibrateValue[6];
int airKeys[6] = { 'a', 'b', 'c', 'd', 'e', 'f'};

void setup() {
//  while (digitalRead(delayButton) == LOW) { //to finish calibrate and know that calibrate value before continue
    for (int i = 0; i < 6; i++) {
      pinMode(photoDiode[i], INPUT);
      calibrateValue[i] = calibrateLED(i);
      Serial.println("calibrate value");
      Serial.print(i);
      Serial.print(calibrateValue[i]);
    }
//  }
  Serial.begin(9600);
  Keyboard.begin();
  Serial.println("1    2     3    4    5   6");

}

void loop() {
  //turn on each led, then press key according to the turn on LED
  /* example:
      loop through each LED {
      charliplexed LED_0 ON, LED_(2, 3, 4, 5, 6) off
      irValue = analogRead(A0)
      state = (irValue < calibrateValue) ? Pressed : notPressed
      return state
      }

      pressKey[1]
      wait until the next cycle loop
      if (state = notPressed)
      releaseKey[1] (only release when receive new state, not when scanning through next LED)
  */

  /* calibrateValue {
      loop through all LED and read all
  */
  for (int i = 0; i < 6; i++) {
    controlLED(i); //turn on corresponding IRLED
    delay(5); //to preven read before turn on (as describe in other Openithm)
    int diodeValue = analogRead(photoDiode[i]);
    //    Serial.println("IR number");
    //    Serial.print(i);
    //    Serial.print(" is ");
    Serial.print(diodeValue);
    Serial.print("   ");
    delay (5);
        if (diodeValue < (calibrateValue[i] - fluctuationValue)) {
          Keyboard.press(airKeys[i]);
        }
        else {
          Keyboard.release(airKeys[i]);
        }
    //this need to change to other way of writing in order to do Keyboard.release
  }
  Serial.println();
}


//use pointer to return whole array
/*int *calibrateLED (int LED) {
  //array of calubrate value for each photodiode
  //static to keep array pointer
  static int calibrateValue[6];

  //loop through each sensor and put in calibrate value
  for (int i = 0; i < 6; i++) {
    controlLED(i);
    delay(200); //delay so that value is stable to calibration
    calibrateValue[i] = analogRead(photoDiode[i]);
    Serial.println("calibrate value ");
    Serial.print(i);
    Serial.print(calibrateValue[i]);
  }
  }*/

int calibrateLED (int LED) {
  controlLED(LED);
  delay(200);
  Serial.println("calibrate value ");
  Serial.print(LED);
  Serial.print(analogRead(photoDiode[LED]));
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

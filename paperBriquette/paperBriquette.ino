#include "HX711.h"
HX711 scale;
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 20, 4);
#include <Keypad.h>

const int LOADCELL_DOUT_PIN = 8;
const int LOADCELL_SCK_PIN = 9;
float calibration_factor = 773675.83;
bool isTargetWeightReached = false;

#define RELAY_OFF HIGH
#define RELAY_ON LOW

#define MOTOR_DOWN 3 //brown wire
#define MOTOR_UP 2  //black wire
#define ACT1_UP 6// SCALE UP
#define ACT1_DOWN 7// SCALE DOWN
#define ACT2_UP 5
#define ACT2_DOWN 4
#define PUMP 24
#define GRINDER 22
#define BINDER 26

#define LMS_UP 29
#define LMS_DOWN 27


// Keypad configuration
const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {42, 40, 38, 36};
byte colPins[COLS] = {34, 32, 30, 28};
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

enum State { ENTER_PASSWORD, SELECT_MODE, AUTOMATIC, MANUAL, CALIBRATE, WASH };
State currentState = ENTER_PASSWORD;

const String correctPassword = "1234"; // You can change this
String inputPassword = "";
unsigned long lastKeyPressTime = 0;
const unsigned long passwordTimeout = 10000; // 10 seconds timeout

void setup() {
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();

  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  scale.set_scale(calibration_factor);
  scale.tare();

  pinMode(ACT1_UP, OUTPUT); digitalWrite(ACT1_UP, RELAY_OFF);
  pinMode(ACT1_DOWN, OUTPUT); digitalWrite(ACT1_DOWN, RELAY_OFF);
  pinMode(ACT2_UP, OUTPUT); digitalWrite(ACT2_UP, RELAY_OFF);
  pinMode(ACT2_DOWN, OUTPUT); digitalWrite(ACT2_DOWN, RELAY_OFF);
  pinMode(MOTOR_DOWN, OUTPUT); digitalWrite(MOTOR_DOWN, RELAY_OFF);
  pinMode(MOTOR_UP, OUTPUT); digitalWrite(MOTOR_UP, RELAY_OFF);
  pinMode(PUMP, OUTPUT); digitalWrite(PUMP, RELAY_OFF);
  pinMode(GRINDER, OUTPUT); digitalWrite(GRINDER, RELAY_OFF);
  pinMode(BINDER, OUTPUT); digitalWrite(BINDER, RELAY_OFF);
 
  pinMode(LMS_UP, INPUT_PULLUP);
  pinMode(LMS_DOWN, INPUT_PULLUP);

  Serial.println("===READY===");
}

void loop() {
  serialCommand();
  // readWeight();

  switch (currentState) {
    case ENTER_PASSWORD:
      handlePasswordInput();
      break;
    case SELECT_MODE:
      showSelectMenu();
      break;
    case AUTOMATIC:
      automaticMenu();
      break;
    case MANUAL:
      showManualMenu();  // show the manual tasks
      break;
    case CALIBRATE:
      calibrateMenu();
      break;
    case WASH:
      washMenu(); 
      break;
  }
  // Serial.print(digitalRead(LMS_UP));
  // Serial.print("||");
  // Serial.println(digitalRead(LMS_DOWN));
  // delay(500);
}

#include "HX711.h"
HX711 scale;
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 20, 4);
#include <Keypad.h>

// LOAD CELL
const int LOADCELL_DOUT_PIN = 13;
const int LOADCELL_SCK_PIN = 12;
float calibration_factor = 428.55;
bool isTargetWeightReached = false;

// RELAYS
// #define DISPENSER 2
#define PUMP 3
#define ACT_UP 4
#define ACT_DOWN 5
#define BACKWARD 6
#define FORWARD 7
#define LOAD_UP 8 //CLOSE
#define LOAD_DOWN 9 //OPEN
#define GRINDER 22
#define BUZZER 11
#define HEATER 10

// LIMIT SWITCH 
#define LMS_UP 46 //CLOSE LOAD
#define LMS_DOWN 44 //OPEN LOAD
#define LMS_FWD 48
#define LMS_BWD 50 

#define RELAY_OFF HIGH
#define RELAY_ON LOW

// Keypad configuration
const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {37, 35, 33, 31};
byte colPins[COLS] = {29, 27, 25, 23};
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

String currentStatus = "Idle";
String screenMode = "Idle"; 

unsigned long grindingDurationMs = 0;

void setup() {
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();

  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  scale.set_scale(calibration_factor);
  scale.tare();

  pinMode(GRINDER, OUTPUT); digitalWrite(GRINDER, RELAY_ON);
  // pinMode(DISPENSER, OUTPUT); digitalWrite(DISPENSER, RELAY_OFF);
  pinMode(HEATER, OUTPUT); digitalWrite(HEATER, RELAY_OFF);
  pinMode(ACT_UP, OUTPUT); digitalWrite(ACT_UP, RELAY_OFF);
  pinMode(ACT_DOWN, OUTPUT); digitalWrite(ACT_DOWN, RELAY_OFF);
  pinMode(FORWARD, OUTPUT); digitalWrite(FORWARD, RELAY_OFF);
  pinMode(BACKWARD, OUTPUT); digitalWrite(BACKWARD, RELAY_OFF);
  pinMode(LOAD_DOWN, OUTPUT); digitalWrite(LOAD_DOWN, RELAY_OFF);
  pinMode(LOAD_UP, OUTPUT); digitalWrite(LOAD_UP, RELAY_OFF);
  pinMode(PUMP, OUTPUT); digitalWrite(PUMP, RELAY_OFF);
  pinMode(BUZZER, OUTPUT); digitalWrite(BUZZER, RELAY_ON);

  pinMode(LMS_FWD, INPUT_PULLUP);
  pinMode(LMS_BWD, INPUT_PULLUP);
  pinMode(LMS_UP, INPUT_PULLUP);
  pinMode(LMS_DOWN, INPUT_PULLUP);

  Serial.println("===READY===");
  mainMenu();
}

void loop() {
  serialCommand();
  displayStatus(currentStatus);
  if (screenMode != "settings") readWeight();
  handleInput();
  delay(500);
}
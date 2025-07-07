#include <AccelStepper.h>
// #include "IRremote.hpp"
// AGROSPRAYTECH PINOUT
#define LEFT_PUL 22
#define LEFT_DIR 24
#define LEFT_ENA 26
#define RIGHT_PUL 28
#define RIGHT_DIR 30
#define RIGHT_ENA 32
#define echoPin 34
#define trigPin 36
#define PUMP_1 8
#define PUMP_2 9
#define BUZZER 10
#define CLOSE 7 //close
#define OPEN 6 //opeN
#define LMS_CLOSE 38
#define LMS_OPEN 40
#define MANUAL_FWD     2
#define MANUAL_BWD     3
#define MANUAL_LEFT    5
#define MANUAL_RIGHT   4


// === Constants ===
#define RELAY_ON LOW
#define RELAY_OFF HIGH

// === Stepper Motors ===
AccelStepper leftMotor(AccelStepper::DRIVER, LEFT_PUL, LEFT_DIR);
AccelStepper rightMotor(AccelStepper::DRIVER, RIGHT_PUL, RIGHT_DIR);

int motorSpeed = 2500;
int acceleration = 2500;
bool isRunning = false;

long duration;
int distance;

bool remoteModeEnabled = false;
int fwdPressCount = 0;
int bwdPressCount = 0;
unsigned long lastRemotePressTime = 0;
const unsigned long debounceTime = 1000;  // 1 second max interval between presses

void setup() {
  Serial.begin(9600);

  // Setup ultrasonic pins
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  // Setup motor enable pins
  pinMode(LEFT_ENA, OUTPUT);
  pinMode(RIGHT_ENA, OUTPUT);

  pinMode(PUMP_1, OUTPUT); digitalWrite(PUMP_1, RELAY_OFF);
  pinMode(PUMP_2, OUTPUT); digitalWrite(PUMP_2, RELAY_OFF);
  pinMode(CLOSE, OUTPUT); digitalWrite(CLOSE, RELAY_OFF);
  pinMode(OPEN, OUTPUT); digitalWrite(OPEN, RELAY_OFF);
  pinMode(BUZZER, OUTPUT); digitalWrite(BUZZER, RELAY_ON);

  pinMode(LMS_CLOSE, INPUT_PULLUP);
  pinMode(LMS_OPEN, INPUT_PULLUP);

  // Motor configuration
  leftMotor.setMaxSpeed(motorSpeed);
  leftMotor.setAcceleration(acceleration);
  rightMotor.setMaxSpeed(motorSpeed);
  rightMotor.setAcceleration(acceleration);
  leftMotor.enableOutputs();
  rightMotor.enableOutputs();

  pinMode(MANUAL_FWD, INPUT_PULLUP);
  pinMode(MANUAL_BWD, INPUT_PULLUP);
  pinMode(MANUAL_LEFT, INPUT_PULLUP);
  pinMode(MANUAL_RIGHT, INPUT_PULLUP);


  Serial.println("=== READY ===");
}


void loop() {
  if (Serial.available() > 0) {
    serialCommand();
  }
  
  handleRFRemote();  // handle remote first

  // Always run motors if running
  if (isRunning) {
    leftMotor.runSpeed();
    rightMotor.runSpeed();
  }
}


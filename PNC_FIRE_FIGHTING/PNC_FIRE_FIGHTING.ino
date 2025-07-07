#include <AccelStepper.h> // Library for controlling stepper motors
#define tfMiniSerial Serial1  // Use Serial1 for the TFmini-S LiDAR

// Stepper motor pin definitions
#define LEFT_PUL 52   // Left motor step pulse
#define LEFT_DIR 50   // Left motor direction
#define LEFT_ENA 48   // Left motor enable

#define RIGHT_PUL 46  // Right motor step pulse
#define RIGHT_DIR 44  // Right motor direction
#define RIGHT_ENA 42  // Right motor enable

// Setup stepper motors (driver mode using STEP/DIR pins)
AccelStepper leftMotor(AccelStepper::DRIVER, LEFT_PUL, LEFT_DIR);
AccelStepper rightMotor(AccelStepper::DRIVER, RIGHT_PUL, RIGHT_DIR);

int motorSpeed = 1500;  // Motor speed
int acceleration = 1000;  // Motor acceleration
bool isRunning = false; // Flag to control motor running

// Actuator control pins
#define ACT_UP 38 // Extinguisher release // OFF EXTINGUISHER
#define ACT_DOWN 40 // Extinguisher press // ON EXTINGUISHER

// Nozzle movement control
#define NOZZLE_UP 34 
#define NOZZLE_DOWN 36 

// Limit switches
#define LMS_UP 13 
#define LMS_DOWN 32 

// Relay logic
#define RELAY_ON LOW
#define RELAY_OFF HIGH

void setup() {
  Serial.begin(9600);   // Serial monitor
  Serial1.begin(115200);  // LiDAR sensor
 
 // Set actuator and nozzle pins as outputs and turn them off
  pinMode(ACT_UP, OUTPUT); digitalWrite(ACT_UP, RELAY_OFF);
  pinMode(ACT_DOWN, OUTPUT); digitalWrite(ACT_DOWN, RELAY_OFF);
  pinMode(NOZZLE_UP, OUTPUT); digitalWrite(NOZZLE_UP, RELAY_OFF);
  pinMode(NOZZLE_DOWN, OUTPUT); digitalWrite(NOZZLE_DOWN, RELAY_OFF);

// Set limit switches as inputs with pull-up resistors
  pinMode(LMS_UP, INPUT_PULLUP);
  pinMode(LMS_DOWN, INPUT_PULLUP);

  // Motor enable and control pins
  digitalWrite(LEFT_ENA, LOW);  
  digitalWrite(RIGHT_ENA, LOW);  
  pinMode(LEFT_PUL, OUTPUT);
  pinMode(LEFT_DIR, OUTPUT);
  pinMode(LEFT_ENA, OUTPUT);
  pinMode(RIGHT_PUL, OUTPUT);
  pinMode(RIGHT_DIR, OUTPUT);
  pinMode(RIGHT_ENA, OUTPUT);

  // Enable motors and set speed/acceleration
  leftMotor.enableOutputs();
  rightMotor.enableOutputs();
  leftMotor.setMaxSpeed(motorSpeed);
  leftMotor.setAcceleration(acceleration);
  rightMotor.setMaxSpeed(motorSpeed);
  rightMotor.setAcceleration(acceleration);

  Serial.println("===READY===");
}

void loop() {
  serialCommand(); // Check for serial commands
  
  // If motors should run, move them at set speed
  if (isRunning) {
    leftMotor.runSpeed();
    rightMotor.runSpeed();
  }
}

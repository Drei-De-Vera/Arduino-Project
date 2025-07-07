#include "HX711.h"

#define DT A1
#define SCK A0
HX711 scale;

#define MOTOR 32
#define SCALE_DOWN 10
#define SCALE_UP  11
#define CONVEYOR 12
#define BIN1_OPEN 4
#define BIN1_CLOSE 5
#define BIN2_OPEN 6
#define BIN2_CLOSE 7
#define BIN3_OPEN 8
#define BIN3_CLOSE 9
#define BUZZER 13

#define LMS_DOWN  22
#define LMS_UP  24
#define LMS_BIN1 30
#define LMS_BIN2 28
#define LMS_BIN3 26

#define ir1 2

#define RELAY_ON LOW
#define RELAY_OFF HIGH

const float noiseThreshold = 10.0;   // Ignore readings below this value (grams)
const int samples = 20;             // Number of samples to average

void setup() {
  Serial.begin(9600);
  scale.begin(DT, SCK);
  loadCell();

  pinMode(MOTOR, OUTPUT); digitalWrite(MOTOR, RELAY_OFF);
  pinMode(SCALE_DOWN, OUTPUT); digitalWrite(SCALE_DOWN, RELAY_OFF);
  pinMode(SCALE_UP, OUTPUT); digitalWrite(SCALE_UP, RELAY_OFF);
  pinMode(CONVEYOR, OUTPUT); digitalWrite(CONVEYOR, RELAY_OFF);
  pinMode(BIN1_OPEN, OUTPUT); digitalWrite(BIN1_OPEN, RELAY_OFF);
  pinMode(BIN1_CLOSE, OUTPUT); digitalWrite(BIN1_CLOSE, RELAY_OFF);
  pinMode(BIN2_OPEN, OUTPUT); digitalWrite(BIN2_OPEN, RELAY_OFF);
  pinMode(BIN2_CLOSE, OUTPUT); digitalWrite(BIN2_CLOSE, RELAY_OFF);
  pinMode(BIN3_OPEN, OUTPUT); digitalWrite(BIN3_OPEN, RELAY_OFF);
  pinMode(BIN3_CLOSE, OUTPUT); digitalWrite(BIN3_CLOSE, RELAY_OFF);
  pinMode(BUZZER, OUTPUT); digitalWrite(BUZZER, RELAY_ON);

  pinMode(LMS_DOWN, INPUT_PULLUP); 
  pinMode(LMS_UP, INPUT_PULLUP); 
  pinMode(LMS_BIN1, INPUT_PULLUP); 
  pinMode(LMS_BIN2, INPUT_PULLUP);
  pinMode(LMS_BIN3, INPUT_PULLUP);
  pinMode(ir1, INPUT);


  Serial.println("===SYSTEM READY===");
}

void loop() {
  serialCommand();

  // === TESTING OF ULTRASONIC SENSOR AND IR SENSOR === 
  // digitalWrite(CONVEYOR, RELAY_ON);
  // irProcess();
  // bin1();
  // bin2();
  // bin3();

  // Serial.print(digitalRead(ir1));
  // delay(500);

  // Serial.print(digitalRead(LMS_DOWN));
  // Serial.print("||");
  // Serial.print(digitalRead(LMS_UP));
  // Serial.print("||");
  // Serial.print(digitalRead(LMS_BIN1));
  // Serial.print("||");
  // Serial.print(digitalRead(LMS_BIN2));
  // Serial.print("||");
  // Serial.println(digitalRead(LMS_BIN3));
  // delay(500);

}


void loadCell() {
  scale.begin(DT, SCK);
  Serial.println("Initializing scale...");

  while (!scale.is_ready()) {
    Serial.println("Waiting for HX711...");
    delay(500);
  }

  delay(1000); // Settling time

  scale.set_scale(21.61);  // Replace with actual scale factor
  scale.tare();               // Set zero offset
  Serial.println("Tare complete.");

  // Double-check tare
  float zeroCheck = scale.get_units(samples);
  if (abs(zeroCheck) > noiseThreshold) {
    Serial.print("⚠️ Tare offset may be off! Initial reading: ");
    Serial.println(zeroCheck, 1);
  }

  Serial.println("System ready. Place weight.");
}

void loadCellProcess() {
  float weight = getStableWeight(samples);

  // First, check for noise threshold
  if (abs(weight) < noiseThreshold) {
    weight = 0.0;
  }
  
  // Then, ensure no negative readings
  if (weight < 0) {
    weight = 0.0;
  }

  Serial.print("Weight (g): ");
  Serial.println(weight, 1);

  delay(1000);

  // Uncomment this if you want auto-reset after each weighing
  scale.tare();
}


float getStableWeight(int sampleCount) {
  float total = 0.0;
  for (int i = 0; i < sampleCount; i++) {
    total += scale.get_units(1);
    delay(5);  // Small delay between samples
  }
  return total / sampleCount;
}
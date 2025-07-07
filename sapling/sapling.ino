// Include necessary libraries
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 20, 4);
#include "HX711.h"

bool isTargetWeightReached = false;
const float ZERO_THRESHOLD = 0.2;  // +/- 2 grams

unsigned long mixStartTime;
bool mixRunning = false;
float totalWeight = 0.0;
bool shredderRunning = false;
unsigned long shredderStartTime = 0;
static bool processComplete = false;
bool shreddingEnabled = false;  // NEW: Track shredding state

unsigned long heaterLastToggleTime = 0;
bool heaterOn = false;



// dry mat
#define SHREDDER 40  // plastic
#define COFFEE 44
#define STARCH 48  // BINDING AGENT
#define AGAR 46    // BINDING AGENT
// wet mat
#define GLYCERIN 25
#define VINEGAR 27
#define WATER 23

#define MIXER 38
#define SCALE_IN 36
#define SCALE_OUT 34
#define MOLD_IN 24
#define MOLD_OUT 26
#define ACT_OPEN 30
#define ACT_CLOSE 28
#define MOLDING 42

#define HEATER1 22
#define FAN 8

#define BTN1 4
#define BTN2 5
// #define BTN3 6
#define LMS_MOLD 51

HX711 scale;

void setup() {
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  while (!Serial)
    ;  // Wait for Serial Monitor
  scale.begin(A1, A0);
  delay(2000);                  // let the system settle
  scale.set_scale(204.483062);  // your calibrated scale
  scale.tare();                 // tare AFTER scale is stable and loaded

  Serial.println("Scale initialized. Ready to read weight...");

  pinMode(SHREDDER, OUTPUT);
  digitalWrite(SHREDDER, 1);
  pinMode(COFFEE, OUTPUT);
  digitalWrite(COFFEE, 1);
  pinMode(STARCH, OUTPUT);
  digitalWrite(STARCH, 1);
  pinMode(AGAR, OUTPUT);
  digitalWrite(AGAR, 1);
  pinMode(MIXER, OUTPUT);
  digitalWrite(MIXER, 1);
  pinMode(GLYCERIN, OUTPUT);
  digitalWrite(GLYCERIN, 1);
  pinMode(VINEGAR, OUTPUT);
  digitalWrite(VINEGAR, 1);
  pinMode(WATER, OUTPUT);
  digitalWrite(WATER, 1);
  pinMode(SCALE_IN, OUTPUT);
  digitalWrite(SCALE_IN, 1);
  pinMode(SCALE_OUT, OUTPUT);
  digitalWrite(SCALE_OUT, 1);
  pinMode(MOLD_IN, OUTPUT);
  digitalWrite(MOLD_IN, 1);
  pinMode(MOLD_OUT, OUTPUT);
  digitalWrite(MOLD_OUT, 1);
  pinMode(ACT_OPEN, OUTPUT);
  digitalWrite(ACT_OPEN, 1);
  pinMode(ACT_CLOSE, OUTPUT);
  digitalWrite(ACT_CLOSE, 1);
  pinMode(MOLDING, OUTPUT);
  digitalWrite(MOLDING, 1);
  pinMode(HEATER1, OUTPUT);
  digitalWrite(HEATER1, 1);
  pinMode(FAN, OUTPUT);
  digitalWrite(FAN, 0);

  pinMode(BTN1, INPUT_PULLUP);
  pinMode(BTN2, INPUT_PULLUP);
  // pinMode(BTN3, INPUT_PULLUP);
  pinMode(LMS_MOLD, INPUT_PULLUP);

  resetDisplay();
  resetProcess();
  display1();
  display2();
  Serial.println("===SYSTEM READY===");
}

void loop() {
  serialCommand();
  // readWeight();
  // delay(1000);

  // Call shredding continuously if enabled
  if (shreddingEnabled) {
    shredding();
  }

  static unsigned long button1PressStart = 0;
  static bool button1Pressed = false;
  static unsigned long button2PressStart = 0;
  static bool button2Pressed = false;

  byte buttonState1 = digitalRead(BTN1);
  byte buttonState2 = digitalRead(BTN2);

  // Button 1 - Start
  if (buttonState1 == LOW && !button1Pressed) {
    button1PressStart = millis();
    button1Pressed = true;
  }
  if (buttonState1 == HIGH && button1Pressed) {
    unsigned long pressDuration = millis() - button1PressStart;
    if (pressDuration >= 2000) {
      Serial.println("Button 1 Pressed");
      Serial.println("Starting process 3... 2... 1...");
      plasticDisplay1();
      shreddingEnabled = true;  // 🔥 Start shredding via loop()
    }

    button1Pressed = false;
  }
  // Heater ON 15s, OFF 30s cycle during mixing
  if (mixRunning) {
    unsigned long currentMillis = millis();
    if (heaterOn && currentMillis - heaterLastToggleTime >= 10000) {
      digitalWrite(HEATER1, 1);
      heaterOn = false;
      heaterLastToggleTime = currentMillis;
      Serial.println("HEATER1 OFF");
    } else if (!heaterOn && currentMillis - heaterLastToggleTime >= 20000) {
      digitalWrite(HEATER1, 0);
      heaterOn = true;
      heaterLastToggleTime = currentMillis;
      Serial.println("HEATER1 ON");
    }
  }

  if (mixRunning && millis() - mixStartTime >= 600000) {
    mixingDisplay2();
    Serial.println("===OPENING ACTUATOR===");
    digitalWrite(ACT_OPEN, 0);
    delay(120000);
    digitalWrite(ACT_OPEN, 1);
    Serial.println("===OPEN ACTUATOR===");
    moldingTest();
    mixRunning = false;
    
    digitalWrite(HEATER1, 1);
    heaterOn = false;
    Serial.println("Heater forced OFF");
  }
  // Button 2 - molding out
  if (buttonState2 == LOW && !button2Pressed) {
    button2PressStart = millis();
    button2Pressed = true;
  }
  if (buttonState2 == HIGH && button2Pressed) {
    unsigned long pressDuration = millis() - button2PressStart;
    if (pressDuration >= 100) {
      Serial.println("Button 2 Pressed");
      moldDisplay3();
      digitalWrite(MOLD_OUT, 0);
      delay(60000);
      digitalWrite(MOLD_OUT, 1);
    }
    button2Pressed = false;
  }
}

float readWeight() {
  if (!scale.is_ready()) {
    Serial.println("HX711 not ready.");
    return 0.0;
  }

  float sum = 0.0;
  int validReads = 0;

  for (int i = 0; i < 10; i++) {
    float val = scale.get_units();
    if (abs(val) > 0.005) {  // ignore very tiny spikes
      sum += val;
      validReads++;
    }
    delay(20);
  }

  float avg = (validReads > 0) ? (sum / validReads) : 0.0;

  // Clamp small noise to zero
  if (abs(avg) < ZERO_THRESHOLD) {
    avg = 0.0;
  }

  if (avg > 0.0) {
    Serial.print("Reading: ");
    Serial.print(avg, 3);
    Serial.println(" grams");
  } else {
    Serial.println("Reading: 0.000 grams");
  }

  return avg;
}

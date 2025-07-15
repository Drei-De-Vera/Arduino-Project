// Include necessary libraries
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 20, 4);
// LCD buffer for flicker-free updates
String lcdBuffer[4] = { "", "", "", "" };

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
bool processStarted = false;
unsigned long lastMixUpdate = 0;

// Blinking status symbol on LCD (to show system is alive)
static unsigned long lastBlinkTime = 0;
static bool blinkOn = false;
const unsigned long blinkInterval = 500; // ms


#define SHREDDER 40  
#define COFFEE 44
#define STARCH 48  
#define AGAR 46    
#define GLYCERIN 23
#define VINEGAR 27
#define WATER 25
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
#define BTN1 5
#define BTN2 6
#define LMS_MOLD 51

// CHANGE THE VALUE DEPENDING ON THE RATIO
#define SHRED_TIME 45000            //  45secs Shredding Plastic
#define MIX_TIME 420000             //  7mins Mixing 
#define PLASTIC_WEIGHT 7.0          //  7grams of shredded plastic
#define COFFEE_WEIGHT 500.0         //  500grams of coffee
#define STARCH_WEIGHT 15.0          //  15grams of starch
#define AGAR_WEIGHT 15.0            //  15grams of agar
#define GLY_DURATION 3000           //  18secs - for (int = 0; i < 6; i++)
#define VIN_DURATION 100            //  5secs - for (int = 0; i < 50; i++)
#define WATER_DURATION 100          //  14secs - for (int = 0; i <50; i++)

HX711 scale;

void setup() {
  Serial.begin(9600);
  Serial.println("STARTING");
  lcd.init();
  lcd.backlight();
  Serial.println("LCD is okay");

  while (!Serial)
    ;  // Wait for Serial Monitor
  scale.begin(A1, A0);
  delay(2000);                  // let the system settle
  scale.set_scale(204.483062);  // your calibrated scale
  scale.tare();                 // tare AFTER scale is stable and loaded

  Serial.println("Scale initialized. Ready to read weight...");

  pinMode(SHREDDER, OUTPUT); digitalWrite(SHREDDER, 1);
  pinMode(COFFEE, OUTPUT); digitalWrite(COFFEE, 1);
  pinMode(STARCH, OUTPUT); digitalWrite(STARCH, 1);
  pinMode(AGAR, OUTPUT); digitalWrite(AGAR, 1);
  pinMode(MIXER, OUTPUT); digitalWrite(MIXER, 1);
  pinMode(GLYCERIN, OUTPUT); digitalWrite(GLYCERIN, 1);
  pinMode(VINEGAR, OUTPUT); digitalWrite(VINEGAR, 1);
  pinMode(WATER, OUTPUT); digitalWrite(WATER, 1);
  pinMode(SCALE_IN, OUTPUT); digitalWrite(SCALE_IN, 1);
  pinMode(SCALE_OUT, OUTPUT); digitalWrite(SCALE_OUT, 1);
  pinMode(MOLD_IN, OUTPUT); digitalWrite(MOLD_IN, 1);
  pinMode(MOLD_OUT, OUTPUT); digitalWrite(MOLD_OUT, 1);
  pinMode(ACT_OPEN, OUTPUT); digitalWrite(ACT_OPEN, 1);
  pinMode(ACT_CLOSE, OUTPUT); digitalWrite(ACT_CLOSE, 1);
  pinMode(MOLDING, OUTPUT); digitalWrite(MOLDING, 1);
  pinMode(HEATER1, OUTPUT); digitalWrite(HEATER1, 1);
  pinMode(FAN, OUTPUT); digitalWrite(FAN, 0);

  pinMode(BTN1, INPUT_PULLUP);
  pinMode(BTN2, INPUT_PULLUP);
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
  // delay(50);

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
      delay(3000);
      plasticDisplay1();
      shreddingEnabled = true;
      processStarted = true;  // <=== mark that Button 1 was pressed
      processComplete = false;
    }
    button1Pressed = false;
  }
  if (mixRunning) {
    unsigned long currentMillis = millis();

    // Heater ON 15s, OFF 30s cycle
    if (heaterOn && currentMillis - heaterLastToggleTime >= 8000) {
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

    // Live time update every 1 sec
    if (currentMillis - lastMixUpdate >= 1000) {
      updateMixingStatus();
      lastMixUpdate = currentMillis;
    }

    // Mixing done after 30 sec
    if (currentMillis - mixStartTime >= MIX_TIME) {
      updateMixingStatus();  // Final update
      mixingDisplay2();
      digitalWrite(HEATER1, 1);
      heaterOn = false;
      Serial.println("Heater forced OFF");
      Serial.println("===OPENING ACTUATOR===");
      digitalWrite(ACT_OPEN, 0);
      delay(120000);
      digitalWrite(ACT_OPEN, 1);
      Serial.println("===OPEN ACTUATOR===");
      moldingTest();
      mixRunning = false;
    }
  }
  // Button 2 - MOLD ACTUATOR OUT
  if (buttonState2 == LOW && !button2Pressed) {
    button2PressStart = millis();
    button2Pressed = true;
  }
  if (buttonState2 == HIGH && button2Pressed) {
    unsigned long pressDuration = millis() - button2PressStart;
    if (pressDuration >= 100) {
      if (processStarted) {
        Serial.println("Button 2 Pressed");
        moldDisplay3();
        digitalWrite(MOLD_OUT, 0);
        delay(60000);
        digitalWrite(MOLD_OUT, 1);
        processComplete = true;
      } else {
        Serial.println("Button 2 ignored: process not started yet.");
        btnDisplay();
        display2();
      }
    }

    button2Pressed = false;
  }
  updateBlinkingStatus();

}

float readWeight() {
  // if (!scale.is_ready()) {
  //   Serial.println("HX711 not ready.");
  //   return 0.0;
  // }

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

void updateBlinkingStatus() {
  if (!processStarted || processComplete) return;  // Only blink if running

  unsigned long now = millis();
  if (now - lastBlinkTime >= blinkInterval) {
    lastBlinkTime = now;
    blinkOn = !blinkOn;

    // You can choose any symbol, or cycle through a few
    String symbol = blinkOn ? "*" : " ";
    lcd.setCursor(19, 3);  // Upper-right corner of 20x4 LCD
    lcd.print(symbol);
  }
}


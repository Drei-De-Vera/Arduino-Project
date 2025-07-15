void shredding() {
  if (processComplete) {
    Serial.println("=== PROCESS COMPLETE ===");
    shreddingEnabled = false;
    return;
  }

  static bool localShredderRunning = false;
  static unsigned long localShredderStartTime = 0;
  static int cycleCount = 1;

  if (!localShredderRunning) {
    Serial.print("=== SHREDDER START: CYCLE ");
    Serial.print(cycleCount);
    Serial.println(" ===");
    digitalWrite(SHREDDER, 0);  // ON
    plasticDisplay1();
    localShredderStartTime = millis();
    localShredderRunning = true;
  }

  if (localShredderRunning && millis() - localShredderStartTime >= SHRED_TIME) {
    digitalWrite(SHREDDER, 1);  // OFF
    localShredderRunning = false;
    Serial.println("=== SHREDDER STOP ===");

    delay(2000);  // Let scale settle

    float reading = readWeight();
    float cycleWeight = (reading >= ZERO_THRESHOLD) ? reading : 0.0;

    if (cycleWeight > 0.0) {
      Serial.print("Cycle Weight: ");
      Serial.print(cycleWeight, 3);
      Serial.println(" g");
      totalWeight += cycleWeight;
    } else {
      Serial.println("Cycle Weight: 0.000 g (noise ignored)");
    }

    Serial.print("Total Weight: ");
    Serial.print(totalWeight, 3);
    Serial.println(" g");

    // Display shredding progress
    plasticProgressDisplay(cycleCount, cycleWeight, totalWeight);
    delay(3000);

    if (totalWeight >= PLASTIC_WEIGHT) {
      // plasticDisplay2(true);
      delay(3000);
      Serial.println("=== TARGET REACHED: 10g ===");
      processComplete = true;
      shreddingEnabled = false;

      Serial.println("Taring scale before main process...");
      scale.tare();
      delay(3000);

      mainProcess();
    } else {
      Serial.println("Taring scale for next cycle...");
      scale.tare();
      delay(3000);
      cycleCount++;
    }
  }
}

void mainProcess() {
  scaleProcess();

  scale.tare();
  delay(3000);

  coffeeProcess();

  scale.tare();
  delay(3000);

  starchProcess();

  delay(3000);
  wetMaterials();

  delay(3000);
}

void dispenseUntilWeight(int pin, float targetWeight, const char* name) {
  Serial.print("===");
  Serial.print(name);
  Serial.println(" DISPENSING STARTED===");

  digitalWrite(pin, 0);  // Start dispensing

  // LCD Setup
  clearLCD();
  updateLCDLine(0, String("Disp: ") + name);
  updateLCDLine(1, "Target: " + String(targetWeight, 1) + "g");

  bool targetReached = false;

  while (true) {
    float weight = readWeight();
    if (weight < 0.0) weight = 0.0;

    if (weight >= targetWeight) {
      if (!targetReached) {
        digitalWrite(pin, 1);  // Stop dispensing
        updateLCDLine(2, "Current: " + String(targetWeight, 1) + "g");
        updateLCDLine(3, "Done!");
        Serial.print("Dispensed ");
        Serial.print(targetWeight);
        Serial.println(" grams");
        Serial.print("===");
        Serial.print(name);
        Serial.println(" DONE===");
        targetReached = true;
      }
      delay(2000);  // Let user see "Done!" message
      break;
    }

    // Live display only until target reached
    updateLCDLine(2, "Current: " + String(weight, 1) + "g");

    delay(100);
  }


  scale.tare();
  delay(3000);
}

void coffeeProcess() {
  dispenseUntilWeight(COFFEE, COFFEE_WEIGHT, "COFFEE");
  scaleProcess();
}

void starchProcess() {
  scale.tare();
  delay(3000);
  dispenseUntilWeight(STARCH, STARCH_WEIGHT, "STARCH");
  scale.tare();
  delay(3000);
  dispenseUntilWeight(AGAR, AGAR_WEIGHT, "AGAR");
  scaleProcess();
}

void wetMaterials() {
  Serial.println("===WET MATERIALS DISPENSING===");
  Serial.println("===MIXING AND HEATING===");
  clearLCD();
  updateLCDLine(0, "Mixing & Heating");
  setWetMaterialStatus(false);
  updateLCDLine(2, "Time: 00:00");
  updateLCDLine(3, "Heater: OFF");
  digitalWrite(FAN, 1);

  if (!mixRunning) {
    Serial.println("===MIXER START===");
    digitalWrite(MIXER, 0);  // ON
    mixStartTime = millis();
    heaterLastToggleTime = mixStartTime;  // reset heater cycle
    heaterOn = false;                     // initial heater state
    mixRunning = true;
  }

  wetDispenser();
  setWetMaterialStatus(true);  // change line after dispensing
}


void moldingTest() {
  moldDisplay1();
  digitalWrite(MIXER, 1);
  digitalWrite(FAN, 0);
  digitalWrite(ACT_CLOSE, 0);
  digitalWrite(MOLD_IN, 0);
  delay(40000);
  digitalWrite(MOLD_IN, 1);
  if (digitalRead(LMS_MOLD) == HIGH) {
    Serial.println("===MOLDING===");
    digitalWrite(MOLDING, 0);
    digitalWrite(MOLD_IN, 0);
    while (digitalRead(LMS_MOLD) == HIGH) {
      delay(10);
    }
    delay(50);
    digitalWrite(MOLDING, 1);
    digitalWrite(MOLD_IN, 1);
    digitalWrite(ACT_CLOSE, 1);
    Serial.println("===STOP MOLDING===");
    moldDisplay2();
  }
}

void scaleProcess() {

  Serial.println("===SCALE_OUT===");
  digitalWrite(SCALE_OUT, 0);
  delay(25000);
  digitalWrite(SCALE_OUT, 1);
  Serial.println("===SCALE_OUT DONE===");

  Serial.println("===SCALE_IN===");
  digitalWrite(SCALE_IN, 0);
  delay(25000);
  digitalWrite(SCALE_IN, 1);
  Serial.println("===SCALE_IN DONE===");
}

void wetDispenser() {
  // Repeating Glycerin process
  for (int i = 0; i < 6; i++) {
    Serial.print("===GLYCERIN PROCESS CYCLE===");
    Serial.print(i + 1);
    digitalWrite(GLYCERIN, 0);
    delay(GLY_DURATION);
    digitalWrite(GLYCERIN, 1);
    Serial.println("===GLYCERIN DONE===");
  }

  // Repeating vinegar process
  for (int i = 0; i < 50; i++) {
    Serial.print("===VINERGAR PROCESS CYCLE===");
    Serial.print(i + 1);
    digitalWrite(VINEGAR, 0);
    delay(VIN_DURATION);
    digitalWrite(VINEGAR, 1);
    delay(50);
    Serial.println("===VINEGAR DONE===");
  }

  // Repeating water process 140 times = 14secs
  for (int i = 0; i < 140; i++) {
    Serial.print("===WATERING PROCESS CYCLE===");
    Serial.print(i + 1);
    digitalWrite(WATER, 0);
    delay(WATER_DURATION);
    digitalWrite(WATER, 1);
    delay(50);
    Serial.println("===WATER DONE===");
  }
}

void resetProcess() {
  Serial.println("===RESETTING===");

  digitalWrite(MOLD_OUT, 0);
  digitalWrite(SCALE_IN, 0);
  digitalWrite(ACT_CLOSE, 0);
  delay(35000);
  digitalWrite(SCALE_IN, 1);
  digitalWrite(ACT_CLOSE, 1);
  digitalWrite(MOLD_OUT, 1);

  Serial.println("===DONE RESET===");
}
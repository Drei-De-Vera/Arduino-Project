void shredding() {
  if (processComplete) {
    Serial.println("=== PROCESS COMPLETE ===");
    shreddingEnabled = false;  // 🔴 Stop future checks
    return;
  }

  static bool localShredderRunning = false;
  static unsigned long localShredderStartTime = 0;

  if (!localShredderRunning) {
    Serial.println("=== SHREDDER START ===");
    digitalWrite(SHREDDER, 0);  // ON
    plasticDisplay1();
    localShredderStartTime = millis();
    localShredderRunning = true;
  }

  if (localShredderRunning && millis() - localShredderStartTime >= 60000) {
    digitalWrite(SHREDDER, 1);  // OFF
    localShredderRunning = false;
    Serial.println("=== SHREDDER STOP ===");

    delay(10000);

    float reading = readWeight();

    if (reading >= ZERO_THRESHOLD) {
      Serial.print("Cycle Weight: ");
      Serial.print(reading, 3);
      Serial.println(" g");
      totalWeight += reading;
    } else {
      Serial.println("Cycle Weight: 0.000 g (noise ignored)");
    }


    totalWeight += reading;

    Serial.print("Total Weight: ");
    Serial.print(totalWeight, 3);
    Serial.println(" g");

    if (totalWeight >= 50.0) {
      Serial.println("=== TARGET REACHED: 50g ===");
      plasticDisplay2();
      processComplete = true;
      shreddingEnabled = false;
      Serial.println("Taring scale before main process...");
      scale.tare();
      delay(3000);  // Let it settle
      mainProcess();
    } else {
      Serial.println("Taring scale for next cycle...");
      scale.tare();
      delay(3000);
    }
  }
}

void dispenseUntilWeight(int pin, float targetWeight, const char* name) {
  Serial.print("===");
  Serial.print(name);
  Serial.println(" DISPENSING STARTED===");

  digitalWrite(pin, 0);  // Start dispensing

  while (true) {
    float weight = readWeight();

    // Clamp negative weight to zero
    if (weight < 0.0) weight = 0.0;

    Serial.print("Reading: ");
    Serial.print(weight, 3);
    Serial.println(" grams");

    if (weight >= targetWeight) {
      digitalWrite(pin, 1);  // Stop dispensing
      Serial.print("Dispensed ");
      Serial.print(targetWeight);
      Serial.println(" grams");
      Serial.print("===");
      Serial.print(name);
      Serial.println(" DONE===");
      break;
    }

    delay(100);
  }

  scale.tare();
  delay(3000);
}

void coffeeProcess() {
  coffeeDisplay1();
  scale.tare();
  delay(3000);
  dispenseUntilWeight(COFFEE, 400.0, "COFFEE");
  coffeeDisplay2();
  scaleProcess();
}

void starchProcess() {
  starchDisplay1();
  scale.tare();
  delay(5000);
  dispenseUntilWeight(STARCH, 12.5, "STARCH");
  starchDisplay2();
  scale.tare();
  delay(5000);
  agarDisplay1();
  dispenseUntilWeight(AGAR, 12.5, "AGAR");
  agarDisplay2();
  scaleProcess();
}

void wetMaterials() {
  Serial.println("===WET MATERIALS DISPENSING===");
  Serial.println("===MIXING AND HEATING===");
  mixingDisplay1();
  digitalWrite(FAN, 1);

  if (!mixRunning) {
    Serial.println("===MIXER START===");
    digitalWrite(MIXER, 0);  // ON
    mixStartTime = millis();
    mixRunning = true;
  }
  wetDispenser();
}

void moldingTest() {
  moldDisplay1();
  digitalWrite(MIXER, 1);
  digitalWrite(FAN, 0);
  digitalWrite(ACT_CLOSE, 0);
  digitalWrite(MOLD_IN, 0);
  delay(35000);
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

void mainProcess() {

  scaleProcess();

  delay(5000);

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

void scaleProcess() {

  Serial.println("===SCALE_OUT===");
  digitalWrite(SCALE_OUT, 0);
  delay(30000);
  digitalWrite(SCALE_OUT, 1);
  Serial.println("===SCALE_OUT DONE===");

  Serial.println("===SCALE_IN===");
  digitalWrite(SCALE_IN, 0);
  delay(30000);
  digitalWrite(SCALE_IN, 1);
  Serial.println("===SCALE_IN DONE===");
}

void wetDispenser() {
  Serial.println("===GLYCERIN===");
  digitalWrite(GLYCERIN, 0);
  delay(150);
  digitalWrite(GLYCERIN, 1);
  Serial.println("===GLYCERIN DONE===");

  Serial.println("===VINEGAR===");
  digitalWrite(VINEGAR, 0);
  delay(150);
  digitalWrite(VINEGAR, 1);
  Serial.println("===VINEGAR DONE===");

  Serial.println("===WATER===");
  digitalWrite(WATER, 0);
  delay(1000);
  digitalWrite(WATER, 1);
  Serial.println("===WATER DONE===");
}
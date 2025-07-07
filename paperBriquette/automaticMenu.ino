void performGrinding(int cycles = 5, unsigned long onTime = 10000, unsigned long offTime = 5000) {
  Serial.println("Grinding...");
  for (int i = 0; i < cycles; i++) {
    digitalWrite(GRINDER, RELAY_ON);
    delay(onTime);
    digitalWrite(GRINDER, RELAY_OFF);
    delay(offTime);
  }
}

void activateRelay(int pin, unsigned long duration, const String& actionName = "") {
  if (actionName != "") {
    Serial.println(actionName);
  }
  digitalWrite(pin, RELAY_ON);
  delay(duration);
  digitalWrite(pin, RELAY_OFF);
}

void automaticMenu() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Automatic Mode");

int cycle = 1;
while (cycle <= 2) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Automatic Mode");

  Serial.print("Starting Cycle ");
  Serial.println(cycle);

  // === WEIGHING ===
  float weight_g = scale.get_units(10) * 1000.0;
  if (weight_g < 50.0) {
    lcd.setCursor(0, 1);
    lcd.print("Add more paper!");
    Serial.println("Add more paper to reach 50g.");
    delay(5000);
    continue; // Retry same cycle number
  } else if (weight_g > 60.0) {
  lcd.setCursor(0, 1);
  lcd.print("Remove paper!    ");
  Serial.println("Weight exceeds 50g. Please remove some paper.");
  delay(5000);
  continue; // Retry same cycle number
  }
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Cycle ");
    lcd.print(cycle);
    lcd.print(" Start");

    // === LIFT PLATFORM ===
    Serial.println("Lifting platform (ACT1_UP)");
    digitalWrite(ACT1_UP, RELAY_ON);
    delay(25000); // Adjust as needed
    digitalWrite(ACT1_UP, RELAY_OFF);
    delay(1000);

    // === RETURN PLATFORM ===
    Serial.println("Returning platform (ACT1_DOWN)");
    digitalWrite(ACT1_DOWN, RELAY_ON);
    delay(30000);
    digitalWrite(ACT1_DOWN, RELAY_OFF);
    Serial.println("Platform returned");

    // === DISPENSE WATER & BINDER ===
    Serial.println("Dispensing water and binder");

    unsigned long startTime = millis();
    unsigned long binderDuration = 50000;   // 50 seconds
    unsigned long pumpDuration = 120000;    // 2 MINS

    digitalWrite(BINDER, RELAY_ON);
    digitalWrite(PUMP, RELAY_ON);

    // Wait while either BINDER or PUMP is still dispensing
    while (millis() - startTime < pumpDuration) {
      if (millis() - startTime >= binderDuration && digitalRead(BINDER) == RELAY_ON) {
        digitalWrite(BINDER, RELAY_OFF);
        Serial.println("Binder OFF");
      }

      // Optional: add a tiny delay to avoid locking the CPU
      delay(10);
    }

    digitalWrite(PUMP, RELAY_OFF);
    Serial.println("Pump OFF");

    delay(3000);

    performGrinding();  // defaults to 6 cycles of 15s ON / 5s OFF

    delay(10000);

  // MOTOR_DOWN for 10s
    if (digitalRead(LMS_DOWN) == HIGH) {
      Serial.println("DROP PAPER");
      digitalWrite(MOTOR_DOWN, RELAY_ON);
      while (digitalRead(LMS_DOWN) == HIGH) {
        delay(10);
      }
      delay(50);
      digitalWrite(MOTOR_DOWN, RELAY_OFF);
      Serial.println("TRANSFERRING PAPER");
    }

  delay(10000);

    if (digitalRead(LMS_UP) == HIGH) {
      Serial.println("READY TO GRIND");
      digitalWrite(MOTOR_UP, RELAY_ON);
      while (digitalRead(LMS_UP) == HIGH) {
        delay(1000);
      }
      delay(50);
      digitalWrite(MOTOR_UP, RELAY_OFF);
      Serial.println("STOP");
    }
  cycle++; // Only increase cycle count if everything went well
}

// === WAIT FOR USER CONFIRMATION BEFORE FINAL STEP ===
lcd.clear();
lcd.setCursor(0, 0);
lcd.print("Press 'A' to cont.");
lcd.setCursor(0, 1);
lcd.print("Spreading...");

Serial.println("Waiting for 'A' key to continue...");

while (true) {
  char key = keypad.getKey();
  if (key == 'A') {
    break;
  }
  delay(100); // small delay to prevent busy loop
}

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("MOLDING...");

  // FIRST CYCLE
  digitalWrite(ACT2_DOWN, RELAY_ON);
  delay(45000);
  digitalWrite(ACT2_DOWN, RELAY_OFF);
  // delay(10000);
  digitalWrite(ACT2_UP, RELAY_ON);
  delay(40000);
  digitalWrite(ACT2_UP, RELAY_OFF);

  //SECOND CYCLE
  digitalWrite(ACT2_DOWN, RELAY_ON);
  delay(45000);
  digitalWrite(ACT2_DOWN, RELAY_OFF);
  // delay(10000);
  digitalWrite(ACT2_UP, RELAY_ON);
  delay(40000);
  digitalWrite(ACT2_UP, RELAY_OFF);

    //THIRD CYCLE
  digitalWrite(ACT2_DOWN, RELAY_ON);
  delay(45000);
  digitalWrite(ACT2_DOWN, RELAY_OFF);
  delay(10000);
  digitalWrite(ACT2_UP, RELAY_ON);
  delay(40000);
  digitalWrite(ACT2_UP, RELAY_OFF);

  // FOURTH CYCLE
  digitalWrite(ACT2_DOWN, RELAY_ON);
  delay(45000);
  digitalWrite(ACT2_DOWN, RELAY_OFF);
  delay(10000);
  digitalWrite(ACT2_UP, RELAY_ON);
  delay(40000);
  digitalWrite(ACT2_UP, RELAY_OFF);

  lcd.clear();
  lcd.print("Auto Process Done!");
  Serial.println("Automatic process complete.");
  delay(5000);

  currentState = SELECT_MODE; // Go back to menu
}




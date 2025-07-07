// Helper function to get clean, filtered weight in KG
float getFilteredWeightKg() {
  float w = scale.get_units(10) / 1000.0; // Convert grams to kilograms
  if (w > -0.05 && w < 0.05) w = 0.0;     // Force near-zero values to exactly 0.0
  return w;
}
void startGrinding() {
  displayStatus("Grinding");
  digitalWrite(GRINDER, RELAY_OFF); // Grinder ON

  unsigned long startMillis = millis();
  unsigned long lastUpdate = 0;

  while (millis() - startMillis < grindingDurationMs) {
    unsigned long now = millis();
    if (now - lastUpdate >= 500) {
      lastUpdate = now;
      unsigned long remaining = (grindingDurationMs - (now - startMillis)) / 1000;
      int minutes = remaining / 60;
      int seconds = remaining % 60;
      lcd.setCursor(0, 3);
      lcd.print("Grind: ");
      if (minutes < 10) lcd.print("0");
      lcd.print(minutes);
      lcd.print(":");
      if (seconds < 10) lcd.print("0");
      lcd.print(seconds);
      lcd.print("      ");
    }

    // Optional: Read key or sensor here without blocking
  }

  digitalWrite(GRINDER, RELAY_ON); // Grinder OFF
  delay(5000);
  displayStatus("WEIGHING");
  delay(10000); // Consider replacing with non-blocking if needed
  
  // displayStatus("DISPENSING");
  // digitalWrite(DISPENSER, RELAY_ON);
  // delay(30000);
  // digitalWrite(DISPENSER, RELAY_OFF);
  // displayStatus("WEIGHING");
  // delay(15000);
}

void shredMaterial() {
  const float targetWeightKg = 0.5;
  const float weightMargin = 0.01; // Allow +/- 10 grams
  float currentWeight = 0.0;
  isTargetWeightReached = false;

  displayStatus("Preparing");
  lcd.setCursor(0, 3);
  lcd.print("Starting in 3s...");
  delay(3000);

  scale.tare();
  delay(500);

  bool grindingComplete = false;

  while (!isTargetWeightReached) {
    if (!grindingComplete) {
      startGrinding();   // Perform grinding once
      grindingComplete = true;
    }

    currentWeight = getFilteredWeightKg();

    lcd.setCursor(0, 2);
    lcd.print("Weight: ");
    lcd.print(currentWeight, 3);
    lcd.print(" kg    ");

    lcd.setCursor(0, 3);
    lcd.print("A = Regrind");

    // If weight OK, proceed to next step
    if (currentWeight >= (targetWeightKg - weightMargin) && currentWeight <= (targetWeightKg + weightMargin)) {
      isTargetWeightReached = true;
      displayStatus("Weight OK");
      delay(3000);
      break; // Exit while loop to continue process
    }

    // If user presses A, allow regrind
    char key = keypad.getKey();
    if (key == 'A') {
      grindingDurationMs = getGrindingTimeFromUser(); // Ask user new grind time
      grindingComplete = false; // Allow regrinding again
    }

    delay(300); // Small delay to avoid overwhelming the LCD/keypad
  }

  dropMaterial(); // Proceed with next steps
}


void dropMaterial() {
  if (!isTargetWeightReached) return;

  displayStatus("Dropping");

  if (digitalRead(LMS_DOWN) == HIGH) {
    digitalWrite(LOAD_DOWN, RELAY_ON);
    while (digitalRead(LMS_DOWN) == HIGH) delay(10);
    digitalWrite(LOAD_DOWN, RELAY_OFF);
    delay(10000);

    displayStatus("Closing");
    digitalWrite(LOAD_UP, RELAY_ON);
    while (digitalRead(LMS_UP) == HIGH) delay(10);
    digitalWrite(LOAD_UP, RELAY_OFF);
    delay(5000);
  }

  if (digitalRead(LMS_FWD) == HIGH) {
    displayStatus("Pushing");
    digitalWrite(FORWARD, RELAY_ON);
    while (digitalRead(LMS_FWD) == HIGH) delay(10);
    digitalWrite(FORWARD, RELAY_OFF);
    delay(500);

    displayStatus("Retracting");
    digitalWrite(BACKWARD, RELAY_ON);
    while (digitalRead(LMS_BWD) == HIGH) delay(10);
    digitalWrite(BACKWARD, RELAY_OFF);
  }
  delay(3000);
  moldMaterial();
}

void moldMaterial() {
  displayStatus("HEATING ON");
  digitalWrite(HEATER, RELAY_ON);

  displayStatus("Pumping");
  digitalWrite(PUMP, RELAY_ON);
  delay(10000);
  digitalWrite(PUMP, RELAY_OFF);

  displayStatus("Molding");
  digitalWrite(ACT_DOWN, RELAY_ON);
  delay(50000);
  digitalWrite(ACT_DOWN, RELAY_OFF);

  delay(60000);

  displayStatus("Done Molding");
  digitalWrite(ACT_UP, RELAY_ON);
  delay(40000);
  digitalWrite(ACT_UP, RELAY_OFF);

  displayStatus("HEATING OFF");
  digitalWrite(HEATER, RELAY_OFF);

  displayStatus("Done");
  digitalWrite(BUZZER, RELAY_OFF);
  delay(5000);
  digitalWrite(BUZZER, RELAY_ON);

  displayStatus("Idle");
  mainMenu();
}

void readWeight() {
  float weight = scale.get_units(10) / 1000.0;
  lcd.setCursor(0, 2);
  lcd.print("Weight: ");
  lcd.print(weight, 3);
  lcd.print(" kg  ");
}
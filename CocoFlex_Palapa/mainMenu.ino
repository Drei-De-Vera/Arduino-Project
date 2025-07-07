void handleInput() {
  char key = keypad.getKey();
  if (key) {
    if (key == 'A') {
      screenMode = "process";
      grindingDurationMs = getGrindingTimeFromUser();
      shredMaterial();
    } else if (key == 'B') {
      screenMode = "settings";
      displaySettings();
    } else if (key == 'C') {
      mainMenu();
      readWeight();
    } else if (key == '*') {
      reset();
    }
  }
}

void displayStatus(String status) {
  if (status != currentStatus) {
    currentStatus = status;
    lcd.setCursor(0, 1);
    lcd.print("Status: ");
    lcd.print(currentStatus);
    lcd.print("      ");
  }
}

void displaySettings() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Settings:");
  lcd.setCursor(0, 1);
  lcd.print("HEATER ON");
  lcd.setCursor(0, 3);
  lcd.print("B-Exit");
  digitalWrite(HEATER, RELAY_ON);

  while (true) {
    char key = keypad.getKey();
    if (key == 'B') {
      mainMenu();
      break;
    }
    delay(10);
  }
}

void mainMenu() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("PALAPA SYSTEM");
  lcd.setCursor(0, 1);
  lcd.print("Status: ");
  lcd.print(currentStatus);
  lcd.setCursor(0, 3);
  lcd.print("A-Start  B-Settings");
}

void reset() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("RESETTING...");

  digitalWrite(GRINDER, RELAY_ON); // OFF GRINDER
  // digitalWrite(DISPENSER, RELAY_OFF);
  digitalWrite(HEATER, RELAY_OFF);
  digitalWrite(LOAD_DOWN, RELAY_OFF);
  digitalWrite(PUMP, RELAY_OFF);

  if (digitalRead(LMS_UP) == HIGH) {
    Serial.println("CLOSING");
    digitalWrite(LOAD_UP, RELAY_ON);
    while (digitalRead(LMS_UP) == HIGH){
      delay(10);
    }
      delay(50);
      digitalWrite(LOAD_UP, RELAY_OFF);
      Serial.println("stop");
    }

    delay(3000);

  if (digitalRead(LMS_BWD) == HIGH) {
    Serial.println("MOVING BACKWARD");
    digitalWrite(BACKWARD, RELAY_ON);
    while (digitalRead(LMS_BWD) == HIGH) {
      delay(10);
    }
      delay(50);
      digitalWrite(BACKWARD, RELAY_OFF);
      Serial.println("stop");
    }

  digitalWrite(ACT_UP, RELAY_ON);
  delay(40000);
  digitalWrite(ACT_UP, RELAY_OFF);

  lcd.clear();
  lcd.setCursor(0, 1);
  lcd.print("   DONE RESETTING"); //|||DONE RESETTING|||
  digitalWrite(BUZZER, RELAY_OFF);
  delay(5000);
  digitalWrite(BUZZER, RELAY_ON);

  scale.tare();
  currentStatus = "Idle";
  screenMode = "Idle";
  delay(2000);
  mainMenu();
}
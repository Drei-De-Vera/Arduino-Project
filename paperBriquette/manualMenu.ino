void showManualMenu() {
  static int manualMenuIndex = 0;
  String manualMenuItems[] = {
    "1. Measure Weight",
    "2. Lift Platform",
    "3. Dispense Water",
    "4. Dispense Binder",
    "5. Blending",
    "6. Tilt Mixer",
    "7. Mold",
    "8. Exit"
  };

  lcd.setCursor(0, 0);
  lcd.print("Manual Mode:");
  lcd.setCursor(1, 1);
  lcd.print(manualMenuItems[manualMenuIndex]);
  lcd.setCursor(0, 3);
  lcd.print(F("   A-OK  B-^  C-v"));

  char key = keypad.getKey();
  if (key) {
    if (key == 'C') {
      manualMenuIndex = (manualMenuIndex - 1 + 8) % 8;  // Move up in the menu
      lcd.clear();
    } else if (key == 'B') {
      manualMenuIndex = (manualMenuIndex + 1) % 8;  // Move down in the menu
      lcd.clear();
    } else if (key == 'A') {
      switch (manualMenuIndex) {
        case 0:  // Measure Weight
          measureWeight();
          break;
        case 1:  // Lift Platform
          liftPlatform();
          break;
        case 2:  // Dispense Water
          dispenseWater();
          break;
        case 3:  // Dispense Binder
          dispenseBinder();
          break;
        case 4:  // Blending
          blend();
          break;
        case 5:  // Tilt Mixer
          tiltMixer();
          break;
        case 6:  // Mold
          mold();
          break;
        case 7:  // Exit to main menu
          currentState = SELECT_MODE;
          lcd.clear();
          break;
      }
      lcd.clear();
    }
  }
}
void measureWeight() {
  float weight_kg = scale.get_units(10);  // read average of 10 readings
  float weight_g = weight_kg * 1000.0;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Weight: ");
  lcd.print(weight_g, 1);
  lcd.print(" g");
  delay(5000);  // Display for 5 seconds
}

void liftPlatform() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Lifting");

  Serial.println("Lifting Platform");
  digitalWrite(ACT1_UP, RELAY_ON);
  delay(23000);  // Platform lift duration
  digitalWrite(ACT1_UP, RELAY_OFF);
  Serial.println("Platform Lifted");

  delay(5000);

  Serial.println("READY TO WEIGH");
  digitalWrite(ACT1_DOWN, RELAY_ON);
  delay(25000);
  digitalWrite(ACT1_DOWN, RELAY_OFF);
  Serial.println("WEIGHING");

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Done Lifting");

}

void dispenseWater() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Adding Water");

  Serial.println("Dispensing Water");
  digitalWrite(PUMP, RELAY_ON);
  delay(120000);  // Water dispense duration
  digitalWrite(PUMP, RELAY_OFF);
  Serial.println("Water Dispensed");

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Water stopped");
}

void dispenseBinder() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Adding Binder");

  Serial.println("Dispensing Binder");
  digitalWrite(BINDER, RELAY_ON);
  delay(60000);
  digitalWrite(BINDER, RELAY_OFF);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Binder stopped");
  delay(1000);
}

void blend() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Grinding");

  Serial.println("Grinding/Blending");
  digitalWrite(GRINDER, RELAY_ON);
  delay(5000);  // Blending duration
  digitalWrite(GRINDER, RELAY_OFF);
  Serial.println("Blending Done");

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Done Grinding");
  delay(1000);
}

void tiltMixer() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Dropping Paper");

  delay(3000);

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
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Ready to Mold");
  delay(1000);
}

void mold() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Molding");

  Serial.println("PRESSING TO MOLD");
  digitalWrite(ACT2_DOWN, RELAY_ON);
  delay(40000);
  digitalWrite(ACT2_DOWN, RELAY_OFF);
  Serial.println("MOLDING");

  delay(10000);
  
  Serial.println("Molding Process");
  digitalWrite(ACT2_UP, RELAY_ON);
  delay(35000);  // Molding duration
  digitalWrite(ACT2_UP, RELAY_OFF);
  Serial.println("Molding Complete");

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Moldig Complete");
  delay(1000);
}

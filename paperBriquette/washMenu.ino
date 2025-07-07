void washMenu() {
  lcd.clear();
  lcd.setCursor(0, 1);
  lcd.print("Cleaning...");

  Serial.println("Dispensing Water");
  digitalWrite(PUMP, RELAY_ON);
  delay(45000);  // Water dispense duration
  digitalWrite(PUMP, RELAY_OFF);
  Serial.println("Water Dispensed");
  
  delay(5000);

  Serial.println("Grinding/Blending");
  digitalWrite(GRINDER, RELAY_ON);
  delay(10000);  // Blending duration
  digitalWrite(GRINDER, RELAY_OFF);
  Serial.println("Blending Done");
  
  delay(5000);

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

    delay(5000);

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
    currentState = MANUAL;
}
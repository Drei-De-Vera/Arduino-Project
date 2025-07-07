void calibrateMenu() {
  const int testDuration = 5000; // 5 seconds for each
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("CALIBRATION MODE");
  delay(1000);

  // ACT1_UP
  lcd.clear(); lcd.print("Testing ACT1_UP");
  Serial.println("Testing ACT1_UP");
  digitalWrite(ACT1_UP, RELAY_ON);
  delay(testDuration);
  digitalWrite(ACT1_UP, RELAY_OFF);

  // ACT1_DOWN
  lcd.clear(); lcd.print("Testing ACT1_DOWN");
  Serial.println("Testing ACT1_DOWN");
  digitalWrite(ACT1_DOWN, RELAY_ON);
  delay(testDuration);
  digitalWrite(ACT1_DOWN, RELAY_OFF);

  // PUMP
  lcd.clear(); lcd.print("Testing PUMP");
  Serial.println("Testing PUMP");
  digitalWrite(PUMP, RELAY_ON);
  delay(testDuration);
  digitalWrite(PUMP, RELAY_OFF);

  lcd.clear(); lcd.print("Testing Binder");
  Serial.println("Testing Binder");
  digitalWrite(BINDER, RELAY_ON);
  delay(testDuration);
  digitalWrite(BINDER, RELAY_OFF);

  // GRINDER
  lcd.clear(); lcd.print("Testing GRINDER");
  Serial.println("Testing GRINDER");
  digitalWrite(GRINDER, RELAY_ON);
  delay(testDuration);
  digitalWrite(GRINDER, RELAY_OFF);

  delay(10000);

  // MOTOR_DOWN (simulate paper drop until LMS_DOWN == LOW or timeout)
  lcd.clear(); lcd.print("Testing MOTOR_DOWN");
  Serial.println("Testing MOTOR_DOWN");
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
  delay(7000);
  // MOTOR_UP (simulate reset until LMS_UP == LOW or timeout)
  lcd.clear(); lcd.print("Testing MOTOR_UP");
  Serial.println("Testing MOTOR_UP");
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

  delay(3000);
  // ACT2_DOWN
  lcd.clear(); lcd.print("Testing ACT2_DOWN");
  Serial.println("Testing ACT2_DOWN");
  digitalWrite(ACT2_DOWN, RELAY_ON);
  delay(testDuration);
  digitalWrite(ACT2_DOWN, RELAY_OFF);
  
    // ACT2_UP
  lcd.clear(); lcd.print("Testing ACT2_UP");
  Serial.println("Testing ACT2_UP");
  digitalWrite(ACT2_UP, RELAY_ON);
  delay(testDuration);
  digitalWrite(ACT2_UP, RELAY_OFF);

  // Done
  lcd.clear();
  lcd.print("Calib. Complete");
  Serial.println("Calibration Finished");
  delay(2000);
  lcd.clear();

  currentState = SELECT_MODE;
}

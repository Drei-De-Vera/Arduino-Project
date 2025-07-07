void serialCommand() {
  String cmd = "";

  if (Serial.available() > 0) {
    cmd = Serial.readStringUntil('\n');
    cmd.trim();
  }

  if (cmd.length() <= 0) {
    return;
  }
  else if (cmd.startsWith("aup1")) {
    Serial.println("DROP PAPER");
    digitalWrite(ACT1_UP, RELAY_ON);
    delay(23000);
    digitalWrite(ACT1_UP, RELAY_OFF);
    Serial.println("DONE DROPPING");
  }
  else if (cmd.startsWith("adown1")) {
    Serial.println("READY TO WEIGH");
    digitalWrite(ACT1_DOWN, RELAY_ON);
    delay(25000);
    digitalWrite(ACT1_DOWN, RELAY_OFF);
    Serial.println("WEIGHING");
  }
  else if (cmd.startsWith("aup2")) {
    Serial.println("DONE MOLDING");
    digitalWrite(ACT2_UP, RELAY_ON);
    delay(35000);
    digitalWrite(ACT2_UP, RELAY_OFF);
    Serial.println("READY TO MOLD");
  }
  else if (cmd.startsWith("adown2")) {
    Serial.println("PRESSING TO MOLD");
    digitalWrite(ACT2_DOWN, RELAY_ON);
    delay(45000);
    digitalWrite(ACT2_DOWN, RELAY_OFF);
    Serial.println("MOLDING");
  }
  else if (cmd.startsWith("paper")) {
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
  }
  else if (cmd.startsWith("reset")) {
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
  }
  else if (cmd.startsWith("pump")) {
    Serial.println("PUMPING");
    digitalWrite(PUMP, RELAY_ON);
    delay(30000);
    digitalWrite(PUMP, RELAY_OFF);
    Serial.println("DONE PUMP");
  }
  else if (cmd.startsWith("grind")) {
    Serial.println("GRINDING");
    digitalWrite(GRINDER, RELAY_ON);
    delay(5000);
    digitalWrite(GRINDER, RELAY_OFF);
    Serial.println("DONE GRIND");
  }
  else if (cmd.startsWith("val")) {
  float weight_kg = scale.get_units(10);  // read average of 10 readings
  float weight_g = weight_kg * 1000.0;
  Serial.print("Weight: ");
  Serial.print(weight_g, 1);
  Serial.println(" g");
  }
  else if (cmd.startsWith("bind")) {
    Serial.println("Binder");
    digitalWrite(BINDER, RELAY_ON);
    delay(50000);
    digitalWrite(BINDER, RELAY_OFF);
    Serial.println("DONE binder");
  }
}  
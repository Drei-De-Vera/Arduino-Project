void serialCommand() {
  String cmd = "";

  if (Serial.available() > 0) {
    cmd = Serial.readStringUntil('\n');
    cmd.trim();
  }

  if (cmd.length() <= 0) {
    return;
  }
  else if (cmd.startsWith("grind")) {
    Serial.println("GRINDING");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("GRINDING...");
    digitalWrite(GRINDER, RELAY_OFF);
    delay(3000);
    digitalWrite(GRINDER, RELAY_ON);
    Serial.println("stop");
  }
  // else if (cmd.startsWith("disp")) {
  //   Serial.println("DISPENSING");
  //   digitalWrite(DISPENSER, RELAY_ON);
  //   delay(10000);
  //   digitalWrite(DISPENSER, RELAY_OFF);
  //   Serial.println("stop");
  // }
  else if (cmd.startsWith("pump")) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("PUMPING...");
    Serial.println("PUMPING");
    digitalWrite(PUMP, RELAY_ON);
    delay(10000);
    digitalWrite(PUMP, RELAY_OFF);
    Serial.println("stop");
  }
  else if (cmd.startsWith("open")) {
    //     lcd.clear();
    // lcd.setCursor(0, 0);
    // lcd.print("DROPPING...");
    if (digitalRead(LMS_DOWN) == HIGH) {
      Serial.println("OPENING");
      digitalWrite(LOAD_DOWN, RELAY_ON);
      while (digitalRead(LMS_DOWN) == HIGH) {
        delay(10);
    }
      delay(50);
      digitalWrite(LOAD_DOWN, RELAY_OFF);
      Serial.println("stop");
    }
  }
  else if (cmd.startsWith("close")) {
        lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("READY TO WEIGH...");
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
  } 
  else if (cmd.startsWith("fwd")) {
        lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("PUSHING...");
    if (digitalRead(LMS_FWD) == HIGH) {
      Serial.println("MOVING FORWARD");
      digitalWrite(FORWARD, RELAY_ON);  
      while (digitalRead(LMS_FWD) == HIGH) {
        delay(10);
      }  
      delay(50);
      Serial.println("stop");
      digitalWrite(FORWARD, RELAY_OFF); 
    }
  }
  else if (cmd.startsWith("bwd"))  {
        lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("BACKWARD...");
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
  }  
  else if (cmd.startsWith("down")) {
        lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("MOLDING...");
    Serial.println("PRESSING DOWN");
    digitalWrite(ACT_DOWN, RELAY_ON);
    delay(30000);
    digitalWrite(ACT_DOWN, RELAY_OFF);
    Serial.println("stop");
  }
  else if (cmd.startsWith("up")) {
        lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("DONE MOLDING...");
    Serial.println("MOVING UP");
    digitalWrite(ACT_UP, RELAY_ON);
    delay(40000);
    digitalWrite(ACT_UP, RELAY_OFF);
    Serial.println("stop");
  }
  // NOTE: INVERTED RELAY_OFF = RELAY_ON & RELAY_ON = RELAY_OFF
  else if (cmd.startsWith("buzz")) {
        lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("COMPLETE PROCESS...");
    Serial.println("BUZZING");
    digitalWrite(BUZZER, RELAY_OFF); 
    delay(5000);
    digitalWrite(BUZZER, RELAY_ON);
    Serial.println("stop");
  }
  else if (cmd.startsWith("heater")) {
        lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("HEATING...");
    Serial.println("HEATING ON");
    digitalWrite(HEATER, RELAY_ON);
    // delay(10000);
    // digitalWrite(HEATER, RELAY_OFF);
    // Serial.println("HEATING OFF");
  }
  else if (cmd.startsWith("sh")) {
        lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("DONE HEATING...");
    Serial.println("HEATING OFF");
    digitalWrite(HEATER, RELAY_OFF);
  }
}





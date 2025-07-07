void serialCommand() {
  String cmd = "";

  if (Serial.available() > 0) {
    cmd = Serial.readStringUntil('\n');
    cmd.trim();
  }

  if (cmd.length() <= 0) {
    return;
  } 
  else if (cmd.startsWith("onmotor")) {
    Serial.println("===MOTOR ON===");
    digitalWrite(MOTOR, RELAY_ON);
  } 
  else if (cmd.startsWith("offmotor")) {
    Serial.println("===MOTOR OFF===");
    digitalWrite(MOTOR, RELAY_OFF);
  } 
  else if (cmd.startsWith("down")) {
    if (digitalRead(LMS_DOWN) == HIGH) {
      Serial.println("===SCALE DOWN===");
      digitalWrite(SCALE_DOWN, RELAY_ON);
      while (digitalRead(LMS_DOWN) == HIGH) {
        delay(10);
      }
      delay(50);
      digitalWrite(SCALE_DOWN, RELAY_OFF);
      Serial.println("===DONE SCALE DOWN");
    }
  } 
  else if (cmd.startsWith("up")) {
    if (digitalRead(LMS_UP) == HIGH) {
      Serial.println("===SCALE UP===");
      digitalWrite(SCALE_UP, RELAY_ON);
      while (digitalRead(LMS_UP) == HIGH) {
        delay(10);
      }
      delay(50);
      digitalWrite(SCALE_UP, RELAY_OFF);
      Serial.println("===DONE SCALE UP");
    }
  } 
  else if (cmd.startsWith("conon")) {
    Serial.println("===CONVEYOR ON===");
    digitalWrite(CONVEYOR, RELAY_ON);
  } 
  else if (cmd.startsWith("conoff")) {
    Serial.println("===CONVEYOR OFF===");
    digitalWrite(CONVEYOR, RELAY_OFF);
  } 
  else if (cmd.startsWith("buzzon")) {
    Serial.println("===BUZZER ON===");
    digitalWrite(BUZZER, RELAY_ON);
  } 
  else if (cmd.startsWith("buzzoff")) {
    Serial.println("===BUZZER OFF===");
    digitalWrite(BUZZER, RELAY_OFF);
  } 
  else if (cmd.startsWith("buzz")) {
    Serial.println("===BUZZING===");
    digitalWrite(BUZZER, RELAY_OFF);
    delay(5000);
    digitalWrite(BUZZER, RELAY_ON);
    Serial.println("===DONE BUZZING===");
  } 
  else if (cmd.startsWith("o1")) {
    Serial.println("===BIN1_OPEN===");
    digitalWrite(BIN1_OPEN, RELAY_ON);
    delay(1400);
    digitalWrite(BIN1_OPEN, RELAY_OFF);
    Serial.println("===DONE BIN1_OPEN===");
  } 
  else if (cmd.startsWith("o2")) {
    Serial.println("===BIN2_OPEN===");
    digitalWrite(BIN2_OPEN, RELAY_ON);
    delay(1400);
    digitalWrite(BIN2_OPEN, RELAY_OFF);
    Serial.println("===DONE BIN2_OPEN===");
  }  
  else if (cmd.startsWith("o3")) {
    Serial.println("===BIN3_OPEN===");
    digitalWrite(BIN3_OPEN, RELAY_ON);
    delay(1400);
    digitalWrite(BIN3_OPEN, RELAY_OFF);
    Serial.println("===DONE BIN3_OPEN===");
  } 
  else if (cmd.startsWith("weigh")) {
    loadCellProcess();
  } 
  else if (cmd.startsWith("c4")) {
    irProcess();
  } 
  else if (cmd.startsWith("b1")) {
    Serial.println("===BIN1_OPEN===");
    digitalWrite(BIN1_OPEN, RELAY_ON);
    delay(1400);
    digitalWrite(BIN1_OPEN, RELAY_OFF);
    Serial.println("===DONE BIN1_OPEN===");
    if (digitalRead(LMS_DOWN) == HIGH) {
      Serial.println("===SCALE DOWN===");
      digitalWrite(SCALE_DOWN, RELAY_ON);
      while (digitalRead(LMS_DOWN) == HIGH) {
        delay(10);
      }
      delay(50);
      digitalWrite(SCALE_DOWN, RELAY_OFF);
      Serial.println("===DONE SCALE DOWN");
    }
    Serial.println("===CONVEYOR ON===");
    digitalWrite(CONVEYOR, RELAY_ON);
    delay(10000);
    digitalWrite(CONVEYOR, RELAY_OFF);
    Serial.println("===CONVEYOR OFF===");
    if (digitalRead(LMS_BIN1) == HIGH) {
      Serial.println("===BIN 1 CLOSING===");
      digitalWrite(BIN1_CLOSE, RELAY_ON);
      while (digitalRead(LMS_BIN1) == HIGH) {
        delay(10);
      }
      delay(50);
      digitalWrite(BIN1_CLOSE, RELAY_OFF);
      Serial.println("===DONE CLOSING BIN 1");
    }
    if (digitalRead(LMS_UP) == HIGH) {
      Serial.println("===SCALE UP===");
      digitalWrite(SCALE_UP, RELAY_ON);
      while (digitalRead(LMS_UP) == HIGH) {
        delay(10);
      }
      delay(50);
      digitalWrite(SCALE_UP, RELAY_OFF);
      Serial.println("===DONE SCALE UP");
    }
  } 
  else if (cmd.startsWith("b2")) {
    Serial.println("===BIN2_OPEN===");
    digitalWrite(BIN2_OPEN, RELAY_ON);
    delay(1400);
    digitalWrite(BIN2_OPEN, RELAY_OFF);
    Serial.println("===DONE BIN2_OPEN===");
    if (digitalRead(LMS_DOWN) == HIGH) {
      Serial.println("===SCALE DOWN===");
      digitalWrite(SCALE_DOWN, RELAY_ON);
      while (digitalRead(LMS_DOWN) == HIGH) {
        delay(10);
      }
      delay(50);
      digitalWrite(SCALE_DOWN, RELAY_OFF);
      Serial.println("===DONE SCALE DOWN");
    }
    Serial.println("===CONVEYOR ON===");
    digitalWrite(CONVEYOR, RELAY_ON);
    delay(20000);
    digitalWrite(CONVEYOR, RELAY_OFF);
    Serial.println("===CONVEYOR OFF===");
    if (digitalRead(LMS_BIN2) == HIGH) {
      Serial.println("===BIN 2 CLOSING===");
      digitalWrite(BIN2_CLOSE, RELAY_ON);
      while (digitalRead(LMS_BIN2) == HIGH) {
        delay(10);
      }
      delay(50);
      digitalWrite(BIN2_CLOSE, RELAY_OFF);
      Serial.println("===DONE CLOSING BIN 2");
    }
    if (digitalRead(LMS_UP) == HIGH) {
      Serial.println("===SCALE UP===");
      digitalWrite(SCALE_UP, RELAY_ON);
      while (digitalRead(LMS_UP) == HIGH) {
        delay(10);
      }
      delay(50);
      digitalWrite(SCALE_UP, RELAY_OFF);
      Serial.println("===DONE SCALE UP");
    }
  }
  else if (cmd.startsWith("b3")) {
    Serial.println("===BIN3_OPEN===");
    digitalWrite(BIN3_OPEN, RELAY_ON);
    delay(1400);
    digitalWrite(BIN3_OPEN, RELAY_OFF);
    Serial.println("===DONE BIN3_OPEN===");
    if (digitalRead(LMS_DOWN) == HIGH) {
      Serial.println("===SCALE DOWN===");
      digitalWrite(SCALE_DOWN, RELAY_ON);
      while (digitalRead(LMS_DOWN) == HIGH) {
        delay(10);
      }
      delay(50);
      digitalWrite(SCALE_DOWN, RELAY_OFF);
      Serial.println("===DONE SCALE DOWN");
    }
    Serial.println("===CONVEYOR ON===");
    digitalWrite(CONVEYOR, RELAY_ON);
    delay(30000);
    digitalWrite(CONVEYOR, RELAY_OFF);
    Serial.println("===CONVEYOR OFF===");
    if (digitalRead(LMS_BIN3) == HIGH) {
      Serial.println("===BIN 3 CLOSING===");
      digitalWrite(BIN3_CLOSE, RELAY_ON);
      while (digitalRead(LMS_BIN3) == HIGH) {
        delay(10);
      }
      delay(50);
      digitalWrite(BIN3_CLOSE, RELAY_OFF);
      Serial.println("===DONE CLOSING BIN 3");
    }
    if (digitalRead(LMS_UP) == HIGH) {
      Serial.println("===SCALE UP===");
      digitalWrite(SCALE_UP, RELAY_ON);
      while (digitalRead(LMS_UP) == HIGH) {
        delay(10);
      }
      delay(50);
      digitalWrite(SCALE_UP, RELAY_OFF);
      Serial.println("===DONE SCALE UP");
    }
  }
}
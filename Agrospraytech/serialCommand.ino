void serialCommand() {
  String cmd = "";
  
if (Serial.available() > 0) {
    cmd = Serial.readStringUntil('\n');
    cmd.trim();
    Serial.print("Received Command: ");
    Serial.println(cmd); 
}

  if (cmd.length() <= 0) {
    return;
  }   
else if (cmd == "fwd") {
    isRunning = true;
    leftMotor.setSpeed(motorSpeed);      // Forward
    rightMotor.setSpeed(-motorSpeed);    // Forward
    Serial.println("Moving Forward...");
}
else if (cmd == "bwd") {
    isRunning = true;
    leftMotor.setSpeed(-motorSpeed);     // left backward
    rightMotor.setSpeed(+motorSpeed);    // right backward
    Serial.println("Moving Backward...");
}
else if (cmd == "right") {
    isRunning = true;
    leftMotor.setSpeed(-motorSpeed);     // Left moves forward
    rightMotor.setSpeed(-motorSpeed);     // Right moves backward
    Serial.println("Turning Right...");
}
else if (cmd == "left") {
    isRunning = true;
    leftMotor.setSpeed(motorSpeed);   //BACKWARD
    rightMotor.setSpeed(motorSpeed);  // forward
    Serial.println("Turning Left...");
}
  // Stop all movement
  else if (cmd == "stop") {
      isRunning = false;
      leftMotor.setSpeed(0);
      rightMotor.setSpeed(0);
      Serial.println("Motors Stopped");
  }
  // PUMP
  else if (cmd == "pump") {
    Serial.println("PUMP 1 & 2 ON");
    digitalWrite(PUMP_1, RELAY_ON);
    digitalWrite(PUMP_2, RELAY_ON);
    delay(5000);
    digitalWrite(PUMP_1, RELAY_OFF);
    digitalWrite(PUMP_2, RELAY_OFF);
    Serial.println("DONE PUMPING");
  }
  // else if (cmd == "p1") {
  //   Serial.println("PUMP 1 ON");
  //   digitalWrite(PUMP_1, RELAY_ON);
  //   delay(5000);
  //   digitalWrite(PUMP_1, RELAY_OFF);
  // } 
  // else if (cmd == "p2") {
  //   Serial.println("PUMP 2 ON");
  //   digitalWrite(PUMP_2, RELAY_ON);
  //   delay(5000);
  //   digitalWrite(PUMP_2, RELAY_OFF);
  // } 
  // DC MOTOR RIGHT
  else if (cmd == "open") {
    if (digitalRead(LMS_OPEN) == HIGH) {
      digitalWrite(OPEN, RELAY_ON);
      while (digitalRead(LMS_OPEN) == HIGH) {
        delay(10);
      }
          delay(50);
    digitalWrite(OPEN, RELAY_OFF);
    }
  }
  // DC MOTOR LEFT
  else if (cmd == "close") {
    if (digitalRead(LMS_CLOSE) == HIGH) {
      digitalWrite(CLOSE, RELAY_ON);
      while (digitalRead(LMS_CLOSE) == HIGH) {
        delay(10);
      }
      delay(50);
      digitalWrite(CLOSE, RELAY_OFF);
    }
  }
  else if (cmd == "buzz") {
    Serial.println("BUZZING");
    digitalWrite(BUZZER, RELAY_OFF);
    delay(5000);
    digitalWrite(BUZZER, RELAY_ON);
  }
  else if (cmd == "val") {
      distance = getDistance();
      Serial.print("Distance: ");
      Serial.print(distance);
      Serial.println(" cm");
    } 
    else {
      Serial.println("Invalid Command");
  }  
  processCommand(cmd);
}  

void processCommand(String cmd) {
  cmd.trim();

  if (cmd.length() <= 0) {
    return;
  }   
  else if (cmd == "fwd") {
      isRunning = true;
      leftMotor.setSpeed(motorSpeed);      // Forward
      rightMotor.setSpeed(-motorSpeed);    // Forward
      Serial.println("Moving Forward...");
  }
  else if (cmd == "bwd") {
      isRunning = true;
      leftMotor.setSpeed(-motorSpeed);     // left backward
      rightMotor.setSpeed(+motorSpeed);    // right backward
      Serial.println("Moving Backward...");
  }
  else if (cmd == "right") {
      isRunning = true;
      leftMotor.setSpeed(-motorSpeed);     // Left moves forward
      rightMotor.setSpeed(-motorSpeed);    // Right moves backward
      Serial.println("Turning Right...");
  }
  else if (cmd == "left") {
      isRunning = true;
      leftMotor.setSpeed(motorSpeed);   //BACKWARD
      rightMotor.setSpeed(motorSpeed);  // forward
      Serial.println("Turning Left...");
  }
  else if (cmd == "stop") {
      isRunning = false;
      leftMotor.setSpeed(0);
      rightMotor.setSpeed(0);
      Serial.println("Motors Stopped");
  }
}

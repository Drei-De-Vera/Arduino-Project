// Handle serial commands
void serialCommand() {
    String cmd = "";

    if (Serial.available() > 0) {
        cmd = Serial.readStringUntil('\n'); // Read input command
        cmd.trim(); // Remove extra spaces or line breaks
        Serial.println(cmd);
    }

    if (cmd.length() <= 0) {
        // Do nothing for zero length
    }
    // Move nozzle up until top limit switch is hit
    else if (cmd == "up") {
        if (digitalRead(LMS_UP) == HIGH) {
            Serial.println("Nozzle up");
            digitalWrite(NOZZLE_UP, RELAY_ON);
            while (digitalRead(LMS_UP) == HIGH) {
                delay(10);
            }
            delay(50);
            digitalWrite(NOZZLE_UP, RELAY_OFF);
            Serial.println("STOP");
        }
    }
    // Move nozzle down until bottom limit switch is hit
    else if (cmd == "down") {
        if (digitalRead(LMS_DOWN) == HIGH) {
            Serial.println("Nozzle down");
            digitalWrite(NOZZLE_DOWN, RELAY_ON);
            while (digitalRead(LMS_DOWN) == HIGH) {
                delay(10);
            }
            delay(50);
            digitalWrite(NOZZLE_DOWN, RELAY_OFF);
            Serial.println("Stop");
        }
    }
     // Release fire extinguisher (extend actuator)
    else if (cmd == "release") {
        Serial.println("Releasing Extinguisher");
        digitalWrite(ACT_UP, RELAY_ON);
        delay(8000); 
        digitalWrite(ACT_UP, RELAY_OFF);
        Serial.println("Stop");
    }
     // Press fire extinguisher (retract actuator)
    else if (cmd == "press") {
        Serial.println("Extinguish");
        digitalWrite(ACT_DOWN, RELAY_ON);
        delay(8000); // change value
        digitalWrite(ACT_DOWN, RELAY_OFF);
        Serial.println("Stop");
    }
    // Move forward
    else if (cmd == "fwd") {
        isRunning = true;
        leftMotor.setSpeed(-3000);
        rightMotor.setSpeed(3000);
        Serial.println("Moving Forward...");
    }
    // Move backward
    else if (cmd == "bwd") {  
        isRunning = true;
        leftMotor.setSpeed(motorSpeed);
        rightMotor.setSpeed(-motorSpeed);
        Serial.println("Moving Backward...");
    }
    // Turn right
    else if (cmd == "right") {  
        isRunning = true;
        leftMotor.setSpeed(-motorSpeed);
        rightMotor.setSpeed(-motorSpeed);
        Serial.println("Turning Right...");
    }
    // Turn left
    else if (cmd == "left") {  
        isRunning = true;
        leftMotor.setSpeed(motorSpeed);
        rightMotor.setSpeed(motorSpeed);
        Serial.println("Turning Left...");
    }
    // Stop all movement
    else if (cmd == "stop") {
        isRunning = false;
        leftMotor.setSpeed(0);
        rightMotor.setSpeed(0);
        Serial.println("Motors Stopped");
    }
    // Print current LiDAR reading
    else if (cmd == "val") {
        int distance = readLidar();
        if (distance != -1) {
            Serial.print("LiDAR Distance: ");
            Serial.print(distance);
            Serial.println(" cm");
        } else {
            Serial.println("Error: No valid LiDAR reading.");
        }
    }
}

// Toggle states
bool fwdState = false;
bool bwdState = false;
bool prevFwdBtn = HIGH;
bool prevBwdBtn = HIGH;

String currentMovement = "stop";  // Track the current movement
// int fwdPressCount = 0;
// int bwdPressCount = 0;
// unsigned long lastRemotePressTime = 0;
// const unsigned long debounceTime = 500;  // 500 ms debounce
// bool remoteModeEnabled = false;

void handleRFRemote() {
  bool currentFwdBtn = digitalRead(MANUAL_FWD);
  bool currentBwdBtn = digitalRead(MANUAL_BWD);
  bool currentLeftBtn = digitalRead(MANUAL_LEFT);
  bool currentRightBtn = digitalRead(MANUAL_RIGHT);

  // === Enable Remote Mode with Forward Button Press ===
  if (prevFwdBtn == HIGH && currentFwdBtn == LOW) {  // Falling edge of forward button
    if (millis() - lastRemotePressTime < debounceTime) {
      fwdPressCount++;  // Increment the forward press counter
    } else {
      fwdPressCount = 1;  // Reset count if debounce period passed
    }
    bwdPressCount = 0;  // Reset backward press count
    lastRemotePressTime = millis();  // Update last press time

    if (fwdPressCount >= 3 && !remoteModeEnabled) {  // If forward button pressed 3 times
      remoteModeEnabled = true;
      fwdPressCount = 0;  // Reset forward press count
      Serial.println("REMOTE MODE ENABLED");
      delay(500);  // Feedback delay (optional)
    }
  }

  // === Disable Remote Mode with Backward Button Press ===
  if (prevBwdBtn == HIGH && currentBwdBtn == LOW) {  // Falling edge of backward button
    if (millis() - lastRemotePressTime < debounceTime) {
      bwdPressCount++;  // Increment the backward press counter
    } else {
      bwdPressCount = 1;  // Reset count if debounce period passed
    }
    fwdPressCount = 0;  // Reset forward press count
    lastRemotePressTime = millis();  // Update last press time

    if (bwdPressCount >= 3 && remoteModeEnabled) {  // If backward button pressed 3 times
      remoteModeEnabled = false;
      bwdPressCount = 0;  // Reset backward press count
      Serial.println("REMOTE MODE DISABLED");
      delay(500);  // Feedback delay (optional)
    }
  }

  // Update previous button states
  prevFwdBtn = currentFwdBtn;
  prevBwdBtn = currentBwdBtn;

  // === Remote Mode Movement Commands ===
  if (remoteModeEnabled) {
    // Handle Forward Button Press
    if (currentFwdBtn == LOW && currentMovement != "fwd") {
      processCommand("fwd");
      currentMovement = "fwd";
    }

    // Handle Backward Button Press
    else if (currentBwdBtn == LOW && currentMovement != "bwd") {
      processCommand("bwd");
      currentMovement = "bwd";
    }

    // Handle Left and Right Buttons
    else if (currentLeftBtn == LOW && currentMovement != "left") {
      processCommand("left");
      currentMovement = "left";
    } 
    else if (currentRightBtn == LOW && currentMovement != "right") {
      processCommand("right");
      currentMovement = "right";
    } 

    // Stop if no buttons are pressed
    else if (currentLeftBtn == HIGH && currentRightBtn == HIGH && currentFwdBtn == HIGH && currentBwdBtn == HIGH) {
      if (currentMovement != "stop") {
        processCommand("stop");
        currentMovement = "stop";
      }
    }
  }
}

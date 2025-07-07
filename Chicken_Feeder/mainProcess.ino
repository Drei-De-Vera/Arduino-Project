void startFeedingCycle(int mealIndex) {
    cycleDone = false; // Reset cycle status to ensure execution

    Serial.println("Starting Feeding Cycle...");

    // Step 1: Turn on feeder for 3 seconds
    digitalWrite(MOVE_RIGHT, LOW);
    delay(500);
    digitalWrite(MOVE_RIGHT, HIGH);
    dispenseFood();

    // Step 2: Move right and dispense food multiple times
    for (int i = 0; i < 6; i++) {
        moveRight();
        dispenseFood();
    }

    // Move right until the limit switch is triggered
    digitalWrite(MOVE_RIGHT, LOW);
    while (digitalRead(RIGHT_LIMIT) == HIGH) {
        delay(10);
    }

    // Stop moving right and debounce
    delay(100);
    digitalWrite(MOVE_RIGHT, HIGH);

    // Step 5: Start moving left until LEFT_LIMIT is pressed
    digitalWrite(MOVE_LEFT, LOW);
    while (digitalRead(LEFT_LIMIT) == HIGH) {
        delay(10);
    }

    // Stop moving left and debounce
    delay(100);
    digitalWrite(MOVE_LEFT, HIGH);

    if (mealIndex == 0) {
    digitalWrite(WATER, LOW);
    delay(50000); // CHANGE HERE
    digitalWrite(WATER, HIGH);
    }

    Serial.println("Feeding Done!");

    // Display completion message on LCD
    lcd.clear();
    lcd.setCursor(3, 2);
    lcd.print("Done Feeding");
    delay(2000);
    
    cycleDone = true;

    showMenu();
}

// Function to dispense food
void dispenseFood() {
    digitalWrite(FEEDER, LOW);
    delay(5000);
    digitalWrite(FEEDER, HIGH);
    delay(3000);
}

// Function to move right for 3 seconds
void moveRight() {
    digitalWrite(MOVE_RIGHT, LOW);
    delay(3000);  // Use the provided move time
    digitalWrite(MOVE_RIGHT, HIGH);
}
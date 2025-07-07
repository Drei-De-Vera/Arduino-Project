void checkBinLevel() {
    static unsigned long lastCheck = 0;
    if (millis() - lastCheck < 500) return;  // Check every 500ms
    lastCheck = millis();

    const int numReadings = 7;
    float distances[numReadings];
    int validReadings = 0;

    // Collect ultrasonic readings
    for (int i = 0; i < numReadings; i++) {
        digitalWrite(TRIG, LOW);
        delayMicroseconds(2);
        digitalWrite(TRIG, HIGH);
        delayMicroseconds(10);
        digitalWrite(TRIG, LOW);

        long duration = pulseIn(ECHO, HIGH, 30000); // 30ms timeout

        if (duration > 0) {
            float distance = duration * 0.0343 / 2;
            distances[validReadings++] = distance;
        }

        delay(50); // Short delay between readings
    }

    // If no valid readings, skip
    if (validReadings < 3) { // Need at least 3 to filter outliers
        Serial.println("No valid ultrasonic readings.");
        digitalWrite(BUZZER, OFF); // Turn off buzzer
        return;
    }

    // Sort the distances
    for (int i = 0; i < validReadings - 1; i++) {
        for (int j = i + 1; j < validReadings; j++) {
            if (distances[i] > distances[j]) {
                float temp = distances[i];
                distances[i] = distances[j];
                distances[j] = temp;
            }
        }
    }

    // Remove lowest and highest, average the middle
    float total = 0;
    for (int i = 1; i < validReadings - 1; i++) {
        total += distances[i];
    }
    float averageDistance = total / (validReadings - 2);

    // Show average distance
    Serial.print("Filtered Average Distance: ");
    Serial.print(averageDistance, 2);
    Serial.println(" cm");

    // Trigger buzzer if bin is not empty
    digitalWrite(BUZZER, averageDistance < 23.00 ? HIGH : LOW);
}



void checkFeedingTime() {
    DateTime now = rtc.now();
    int currentTime = now.hour() * 100 + now.minute();

    for (int i = 0; i < 3; i++) {
        int mealHour = EEPROM.read(i * 2);
        int mealMinute = EEPROM.read(i * 2 + 1);
        int mealTime = mealHour * 100 + mealMinute;

        if (currentTime == mealTime && !hasFedToday[i]) {
            hasFedToday[i] = true;
            lastFeedingHour = mealHour;
            lastFeedingMinute = mealMinute;
            Serial.println("Starting Feeding Cycle...");
            
            // Pass the meal index (0 for first meal, 1 for second, etc.)
            startFeedingCycle(i);
            delay(60000);
            break;
        }
    }

    // Reset the flag at midnight
    if (now.hour() == 0 && now.minute() == 0) {
        for (int i = 0; i < 3; i++) hasFedToday[i] = false;
        cycleDone = false; // Reset cycle for the next day
    }
}
void showMenu() {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(F("CHICKEN FEEDER"));
    lcd.setCursor(3, 1);
    lcd.print(menuItems[menuIndex]);
    lcd.setCursor(0, 3);
    lcd.print(F("A-OK B-Back C-^ D-v"));
    delay(200);
}

void handleKeypad() {
    static char lastKey = '\0';
    static unsigned long lastPressTime = 0;
    const unsigned long debounceDelay = 150; // Reduce debounce delay

    char key = keypad.getKey();
    
    if (key && (key != lastKey || millis() - lastPressTime > debounceDelay)) {
        lastPressTime = millis();
        lastKey = key;

        if (key == 'D') { // SCROLL DOWN
            menuIndex = (menuIndex + 1) % menuLength;
        } else if (key == 'C') { // SCROLL UP
            menuIndex = (menuIndex - 1 + menuLength) % menuLength;
        } else if (key == 'B') { // BACK
            showMenu();
            return;
        } else if (key == 'A') { // SELECT MENU
            switch (menuIndex) {
                case 0: viewSchedule(); break;
                case 1: editSchedule("Edit Feeding Time", mealTimes, 3); break;
                case 2: viewRTC(); break;
                case 3: startFeedingCycle(0); break;
            }
            return;
        }
        showMenu();  // Refresh menu only once per valid keypress
    }
}

void viewSchedule() {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(F("Feeding Schedule:"));

    for (int i = 0; i < 3; i++) {
        int mealHour = EEPROM.read(i * 2);
        int mealMinute = EEPROM.read(i * 2 + 1);

        // Check for uninitialized EEPROM values (255 is default for empty EEPROM)
        if (mealHour >= 24 || mealMinute >= 60 || mealHour == 255 || mealMinute == 255) {
            mealTimes[i] = "__:__";  // Show blank if no valid time is set
        } else {
            mealTimes[i] = (mealHour < 10 ? "0" : "") + String(mealHour) + ":" +
                           (mealMinute < 10 ? "0" : "") + String(mealMinute);
        }

        lcd.setCursor(0, i + 1);
        lcd.print("M" + String(i + 1) + ": " + mealTimes[i]);
    }

    delay(5000);

    unsigned long startTime = millis();  // Record the start time
    while (true) {
        char key = keypad.getKey();
        if (key == 'B') {
            showMenu();
            break;
        }
        // Exit loop after 10 seconds
        if (millis() - startTime > 10000) {
            showMenu();
            break;
        }
    }
}

// Function to edit schedule
void editSchedule(const char* title, String schedules[], int size) {
    int i = 0;  // Start at the first meal

    while (i < size) {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(title);
        lcd.setCursor(0, 1);
        lcd.print("Set ");
        lcd.print(i + 1);
        lcd.print(" Time: ");
        lcd.setCursor(0, 2);
        lcd.print(schedules[i]);

        String newTime = "__:__";
        int cursorPos = 0;
        bool showCursor = true;
        unsigned long previousMillis = millis();

        while (cursorPos < 5) {
            char key = keypad.getKey();
            if (key) {
                if (key == '*') {  // Exit and return to menu
                    showMenu();
                    return;
                }
                if (key == 'B') {  // Go back to previous meal
                    if (i > 0) i--;  // Move back only if not at the first meal
                    break;
                }
                if (key >= '0' && key <= '9') {
                    if (cursorPos == 2) cursorPos++; // Skip colon
                    newTime.setCharAt(cursorPos, key);
                    cursorPos++;
                } else if (key == '#' && cursorPos > 0) {  // Backspace function
                    cursorPos--;
                    newTime.setCharAt(cursorPos, '_');
                }
            }

            if (millis() - previousMillis >= 500) {
                previousMillis = millis();
                showCursor = !showCursor;
            }

            lcd.setCursor(0, 2);
            lcd.print(newTime);
            if (showCursor) {
                lcd.setCursor(cursorPos, 2);
                lcd.print('_');
            }
        }

        int hour = newTime.substring(0, 2).toInt();
        int minute = newTime.substring(3, 5).toInt();

        if (hour >= 0 && hour <= 23 && minute >= 0 && minute <= 59) {
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Confirm?");
            lcd.setCursor(0, 1);
            lcd.print(newTime);
            lcd.setCursor(0, 2);
            lcd.print("A-Save B-Back #-Skip");

            while (true) {
                char confirmKey = keypad.getKey();
                if (confirmKey == 'A') {  // Confirm and save the time
                    schedules[i] = newTime;
                    EEPROM.update(i * 2, hour);
                    EEPROM.update(i * 2 + 1, minute);
                    i++;  // Move to the next meal
                    break;
                } else if (confirmKey == 'B') {  // Go back to edit
                    break;
                } else if (confirmKey == '#') {  // Exit and return to menu
                    i++;  // Move to the next meal
                    break;
                }
            }
        } else {
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Invalid Time!");
            lcd.setCursor(0, 1);
            lcd.print("Use HH:MM (24hr)");
            delay(1500);
        }
    }
    showMenu();
}

void viewRTC() {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(F("Current Date & Time:"));
    
    DateTime now = rtc.now();
    lcd.setCursor(0, 1);
    lcd.print(now.year(), DEC);
    lcd.print("/");
    lcd.print(now.month(), DEC);
    lcd.print("/");
    lcd.print(now.day(), DEC);
    
    lcd.setCursor(0, 2);
    lcd.print(now.hour(), DEC);
    lcd.print(":");
    lcd.print(now.minute(), DEC);
    lcd.print(":");
    lcd.print(now.second(), DEC);
    
    while (true) {
        char key = keypad.getKey();
        if (key == 'B') {
            showMenu();
            break;
        }
    }
}
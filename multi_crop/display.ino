void alternateDisplay() {
  static unsigned long lastSwitch = 0;
  static bool showFirst = true;

  if (millis() - lastSwitch >= 5000) {
    showFirst = !showFirst;
    lastSwitch = millis();
  }

  if (showFirst != lastShown) {
    lcd.clear();
    lastShown = showFirst;
  }
  if (showFirst) display1();
  else display2();
}

void display1() {
  DateTime now = rtc.now();
  
  lcd.setCursor(0, 0);
  lcd.print("Fruit: ");
  lcd.print(fruit); // Safe concatenation

  lcd.setCursor(0, 1);
  lcd.print("Set Temp: ");
  lcd.print(setTemp, 1);
  lcd.print((char)223); // Degree symbol
  lcd.print("C");

  lcd.setCursor(0, 2);
  lcd.print("Ripeness: ");
  lcd.print(ripeness);

  lcd.setCursor(0, 3);
  lcd.print("D&T:");

  lcd.setCursor(5, 3);
  char buffer[21]; // max 20 chars + null terminator
  snprintf(buffer, sizeof(buffer), "%02d/%02d/%02d %02d:%02d", 
           now.month(), now.day(), now.year() % 100,
           now.hour(), now.minute());
  lcd.print(buffer);
}


void display2() {
  lcd.setCursor(0, 0);
  lcd.print("Current Temp: ");
  lcd.print(temperature);
  lcd.setCursor(0, 1);
  lcd.print("Humidity: ");
  lcd.print(humidity);
}

void fruitSelectionScreen() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Select Fruit Type:");
  lcd.setCursor(0, 1);
  lcd.print("  BTN1: PINEAPPLE");
  lcd.setCursor(0, 2);
  lcd.print("  BTN2: DRAGONFRUIT");

  while (true) {
    if (digitalRead(BTN_PA) == LOW) {
      lcd.clear();
      lcd.setCursor(4, 1);
      lcd.print("PINEAPPLE");
      fruit = "PINEAPPLE";
      delay(2000);  // show the fruit name for 2 seconds
      break;
    }
    if (digitalRead(BTN_DF) == LOW) {
      lcd.clear();
      lcd.setCursor(3, 1);
      lcd.print("DRAGONFRUIT");
      fruit = "DRAGONFRUIT";
      delay(2000);  // show the fruit name for 2 seconds
      break;
    }
  }

  lcd.clear(); // Clear screen after selection
}


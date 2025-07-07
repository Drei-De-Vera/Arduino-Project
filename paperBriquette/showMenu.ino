void showSelectMenu() {
  static int menuIndex = 0;
  String menuItems[] = {"1.Automatic", "2.Manual", "3.Calibrate", "4.Wash", "5.Exit"};

  lcd.setCursor(0, 0);
  lcd.print("Select Mode:      ");
  lcd.setCursor(5, 1);
  lcd.print(menuItems[menuIndex]);
  lcd.setCursor(0, 3);
  lcd.print(F("   A-OK  B-^  C-v"));

  char key = keypad.getKey();
  if (key) {
    if (key == 'C') {
      menuIndex = (menuIndex - 1 + 5) % 5;
      lcd.clear();
    } else if (key == 'B') {
      menuIndex = (menuIndex + 1) % 5;
      lcd.clear();
    } else if (key == 'A') {
      switch (menuIndex) {
        case 0: currentState = AUTOMATIC; break;
        case 1: currentState = MANUAL; break;
        case 2: currentState = CALIBRATE; break;
        case 3: currentState = WASH; break;
        case 4:
          currentState = ENTER_PASSWORD;
          lcd.clear();
          break;
      }
      lcd.clear();
    }
  }
}

void handlePasswordInput() {
  lcd.setCursor(0, 0);
  lcd.print("   Paper Briquette");
  lcd.setCursor(0, 2);
  lcd.print("Enter Password:   ");
  lcd.setCursor(15, 2);
  lcd.print(inputPassword + String("    ").substring(inputPassword.length()));

  char key = keypad.getKey();
  if (key) {
    lastKeyPressTime = millis();

    if (key == 'A') { // Confirm
      if (inputPassword == correctPassword) {
        lcd.clear();
        lcd.setCursor(3, 1);
        lcd.print("Access Granted");
        delay(1000);
        lcd.clear();
        inputPassword = "";
        currentState = SELECT_MODE;
      } else {
        lcd.clear();
        lcd.setCursor(3, 1);
        lcd.print("Wrong Password");
        delay(1500);
        lcd.clear();
        inputPassword = "";
      }
    } else if (key == '*') { // Clear input
      inputPassword = "";
    } else if (inputPassword.length() < 4 && isDigit(key)) {
      inputPassword += key;
    }
  }

  // Optional timeout reset
  if (inputPassword.length() > 0 && millis() - lastKeyPressTime > passwordTimeout) {
    inputPassword = "";
    lcd.clear();
  }
}

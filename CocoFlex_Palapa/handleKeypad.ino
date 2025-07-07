unsigned long getGrindingTimeFromUser() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Enter MM:SS:");

  String input = "";
  int cursorPosition = 0; // To track where the user is in the input

  while (input.length() < 4) { // Only need 4 digits: MMSS
    char key = keypad.getKey();

    if (key && isDigit(key)) {
      input += key;
      lcd.setCursor(cursorPosition, 1);
      lcd.print(key);
      cursorPosition++;

      if (cursorPosition == 2) {
        lcd.print(":");  // Add colon after minutes
        cursorPosition++; // Skip one position for the colon
      }
    }
  }

  // Parse minutes and seconds from the input string
  int minutes = input.substring(0, 2).toInt();
  int seconds = input.substring(2, 4).toInt();

  // Convert to milliseconds
  unsigned long totalMillis = (minutes * 60UL + seconds) * 1000UL;

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Grind Timer:");
  lcd.setCursor(0, 1);
  lcd.print(minutes); lcd.print("m ");
  lcd.print(seconds); lcd.print("s");

  return totalMillis;
}

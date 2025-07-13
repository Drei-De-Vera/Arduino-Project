void updateLCDLine(uint8_t line, const String& message) {
  String msg = message;
  while (msg.length() < 20) msg += ' ';
  if (lcdBuffer[line] != msg) {
    lcd.setCursor(0, line);
    lcd.print(msg);
    lcdBuffer[line] = msg;
  }
}

void clearLCD() {
  lcd.clear();
  for (int i = 0; i < 4; i++) {
    lcdBuffer[i] = "                    ";
  }
}

// === LCD DISPLAY FUNCTIONS ===

void display1() {
  clearLCD();
  updateLCDLine(0, "       Welcome");
  updateLCDLine(1, "         To");
  updateLCDLine(2, "  Sapling Planters");
  updateLCDLine(3, "       System");
  delay(3000);
  display2();
}

void display2() {
  clearLCD();
  updateLCDLine(0, "  SAPLING PLANTERS");
  updateLCDLine(1, "BTN1: START");
  updateLCDLine(2, "BTN2: FINISH");
  updateLCDLine(3, "BTN3: RESET");
}

void plasticDisplay1() {
  clearLCD();
  updateLCDLine(0, "Shredding Plastic...");
  updateLCDLine(2, "Collecting Weight...");
}

void plasticDisplay2(bool done = false) {
  clearLCD();
  if (done) {
    updateLCDLine(0, "    Done Shredding");
    updateLCDLine(1, "    Plastic: 7g"); //change value
  } else {
    updateLCDLine(0, "Shredding Plastic...");
  }
}

void plasticProgressDisplay(int cycle, float cycleWeight, float totalWeight) {
  clearLCD();

  if (totalWeight >= PLASTIC_WEIGHT) {
    updateLCDLine(1, "   Target Reached"); //|||||Done Mixing||||
    updateLCDLine(2, "   Done Shredding"); //|||Done Shredding|||
  } else {
    updateLCDLine(0, "Cycle: " + String(cycle));
    updateLCDLine(1, "This: " + String(cycleWeight, 1) + "g");
    updateLCDLine(2, "Total: " + String(totalWeight, 1) + "g");
    updateLCDLine(3, "Shredding...");
  }
}

void mixingDisplay1() {
  clearLCD();
  updateLCDLine(0, "Mixing & Heating");
  // updateLCDLine(1, "Disp. Glycerin");
  // updateLCDLine(2, "Disp. Vinegar");
  // updateLCDLine(3, "Disp. Water");
}

void mixingDisplay2() {
  clearLCD();
  updateLCDLine(1, "     Done Mixing");
  delay(3000);
  clearLCD();
  updateLCDLine(1, "      DROPPING");
  updateLCDLine(2, "      MIXTURE");
}

void moldDisplay1() {
  clearLCD();
  updateLCDLine(1, "     MOLDING...");
}

void moldDisplay2() {
  clearLCD();
  updateLCDLine(0, "  RELEASE PRESSURE");
  updateLCDLine(1, "        AND");
  updateLCDLine(2, "    PRESS BTN 2");
  updateLCDLine(3, "    FOR MOLD OUT");
}

void moldDisplay3() {
  clearLCD();
  updateLCDLine(0, "  CAUTION: HOT MOLD"); 
  updateLCDLine(1, "   USE SAFETY GEAR");  
  updateLCDLine(3, "  BTN 3: RESET SYS");
}

void btnDisplay() {
  clearLCD();
  updateLCDLine(0, "Press BTN 1 First");
  delay(3000);
  display2();
}

void resetDisplay() {
  clearLCD();
  updateLCDLine(1, "    INITIALIZING");
  updateLCDLine(2, "       SYSTEM");
}

void updateMixingStatus() {
  unsigned long elapsed = millis() - mixStartTime;
  unsigned long remaining = (MIX_TIME > elapsed) ? (MIX_TIME - elapsed) : 0;  

  int seconds = (remaining / 1000) % 60;
  int minutes = (remaining / 1000) / 60;

  char timeBuffer[17];  // For a 16x2 or 20x4 LCD line
  sprintf(timeBuffer, "Time: %02d:%02d", minutes, seconds);
  updateLCDLine(2, String(timeBuffer));

  String heaterStatus = heaterOn ? "Heater: ON " : "Heater: OFF";
  updateLCDLine(3, heaterStatus);
}


void setWetMaterialStatus(bool done) {
  if (done) {
    updateLCDLine(1, "Done Wet Materials");
  } else {
    updateLCDLine(1, "Disp. Wet Materials");
  }
}


void resetDisplay() {
  lcd.clear();
  lcd.setCursor(0, 1);
  lcd.print("    INITIALIZING");
  lcd.setCursor(0, 2);
  lcd.print("       SYSTEM");
}
void display1() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("       Welcome");  // |||||||Welome|||||||
  lcd.setCursor(0, 1);          // ||||INITIALIZING||||
  lcd.print("         To");     // ||Sapling Planters||
  lcd.setCursor(0, 2);          // |||||||System|||||||
  lcd.print("  Sapling Planters");
  lcd.setCursor(0, 3);
  lcd.print("       System");
  delay(5000);
}

void display2() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("  SAPLING PLANTERS");  // ||SAPLING PLANTERS||
  lcd.setCursor(0, 1);            // BTN1: START|||||||||
  lcd.print("BTN1: START");       // BTN2: STOP/PAUSE||||
  lcd.setCursor(0, 2);            // BTN3: CONTINUE||||||
  lcd.print("BTN2: FINISH");
  lcd.setCursor(0, 3);
  lcd.print("BTN3: RESET");
}

void plasticDisplay1() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Shredding Plastic...");
}

void plasticDisplay2() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Done Shredding");     // Done Shredding||||||
  lcd.setCursor(0, 1);             // ||||Plastic: 50g||||
  lcd.print("    Plastic: 50g");
}

void coffeeDisplay1() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Dispensing coffee...");
}

void coffeeDisplay2() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Done Coffee");        // Done Coffee|||||||||
  lcd.setCursor(0, 1);             // ||||Coffee: 400g||||
  lcd.print("    Coffee: 400g");
}

void starchDisplay1() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Dispensing starch...");
}

void starchDisplay2() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Done Starch");        // Done Starch|||||||||
  lcd.setCursor(0, 1);             // |||Starch: 12.5g||||
  lcd.print("   Starch: 12.5g");
}

void agarDisplay1() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Dispensing agar...");
}

void agarDisplay2() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Done Agar");          // |||||||MOLDING||||||
  lcd.setCursor(0, 1);             // ||||||MIXTURE|||||||
  lcd.print("     Agar: 12.5g");
}

void mixingDisplay1() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Mixing & Heating");
  lcd.setCursor(0, 1);
  lcd.print("Dispensing Glycerin.");
  lcd.setCursor(0, 2);
  lcd.print("Dispensing Vinegar..");
  lcd.setCursor(0, 3);
  lcd.print("Dispensing Water...");
}

void mixingDisplay2() {
  lcd.clear();
  lcd.setCursor(3, 1);
  lcd.print("Wet Materials");
  lcd.setCursor(7, 2);
  lcd.print("Done");
  delay(3000);
  lcd.clear();
  lcd.setCursor(0, 1);
  lcd.print("      DROPPING");
  lcd.setCursor(0, 2);
  lcd.print("      MIXTURE");
}

void moldDisplay1() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("       MOLDING");
}

void moldDisplay2() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("RELEASE THE PRESSURE");
  lcd.setCursor(0, 1);
  lcd.print("        and");
  lcd.setCursor(0, 2);
  lcd.print("    PRESS BTN 2");
}

void moldDisplay3() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("  PLEASE BE CAREFUL");
  lcd.setCursor(0, 1);
  lcd.print("     HOT SURFACE");
  lcd.setCursor(0, 3);
  lcd.print("    BTN 3: RESET");
}
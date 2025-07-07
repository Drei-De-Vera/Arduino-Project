void readWeight() {
  float weight_kg = scale.get_units(10);  // returns weight in kg
  float weight_g = weight_kg * 1000.0;     // convert to grams

  // // If near zero, treat as zero to avoid noise
  // if (abs(weight_g) < 50.0) {  // equivalent to 0.05 kg
  //   weight_g = 0.0;
  // }

  Serial.print("Weight: ");
  Serial.print(weight_g, 1);  // 1 decimal place for grams
  Serial.println(" g");

  // Optional: display on LCD
  lcd.setCursor(0, 0);
  lcd.print("Weight:        ");
  lcd.setCursor(0, 0);
  lcd.print("Weight: ");
  lcd.print(weight_g, 1);
  lcd.print(" g");

  delay(1000);
}

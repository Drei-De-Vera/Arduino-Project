void Maintain_Temp() {
  TempValue = TC.readCelsius();
  // Serial.println(TempValue);
  // delay(500);


  if (TempValue >= TempMax) {
    if (TempMaxFL == 0) {
//      Serial.println("TEMP MAX");
      TempMaxFL = 1; TempMinFL = 0;
      digitalWrite(Heater, HIGH); delay(100);
      digitalWrite(Fan, LOW);
    }
  }
  else if (TempValue <= TempMin) {
    if (TempMinFL == 0) {
//      Serial.println("TEMP MIN");
      TempMaxFL = 0; TempMinFL = 1;
      digitalWrite(Heater, LOW); delay(100);
      digitalWrite(Fan, HIGH);
    }
  }

}

void Get_Volt_Watt() {

  Volt = pzem.voltage(ip);
  Watt = pzem.power(ip);

  if (Volt < 0); Volt = 0.0;
  if (Watt < 0); Watt = 0.0;

  //  Serial.print(Volt); Serial.print("V,  ");
  //  Serial.print(Watt); Serial.println("W");
  //  delay(100);

}

void Open_Funnel() {

  if (digitalRead(lmtFunnel_O) == HIGH) {
    digitalWrite(mtrFunnel_O, LOW);
    while (digitalRead(lmtFunnel_O) == HIGH);
    digitalWrite(mtrFunnel_O, HIGH);
  }

}

void Close_Funnel() {

  if (digitalRead(lmtFunnel_C) == HIGH) {
    digitalWrite(mtrFunnel_C, LOW);
    while (digitalRead(lmtFunnel_C) == HIGH);
    digitalWrite(mtrFunnel_C, HIGH);
  }

}

void TIMER(int Min, int Sec) {

  int myMin = 0, mySec = 0;
  myMin = Min; mySec = Sec;
  lcd.clear();
  while (1) {

    lcd.setCursor(0, 0); lcd.print("TEMP:");
    // lcd.setCursor(0, 1); lcd.print("VOLTAGE:");
    // lcd.setCursor(0, 2); lcd.print("WATTS:");
    lcd.setCursor(0, 3); lcd.print("TIME:");

    Maintain_Temp();
    lcd.setCursor(6, 0); lcd.print(TempValue); lcd.print(" ");

      //  Get_Volt_Watt();
      //  lcd.setCursor(9, 1); lcd.print(Volt);
      //  lcd.setCursor(7, 2); lcd.print(Volt);

    lcd.setCursor(6, 3);
    if (myMin < 10) lcd.print("0");
    lcd.print(myMin); lcd.print(":");
    if (mySec < 10) lcd.print("0");
    lcd.print(mySec);

//    Serial.print(myMin); Serial.print(":"); Serial.println(mySec);

    if (mySec > 0) {
      mySec--;
    }
    else {
      if (myMin > 0) {
        myMin--;
        mySec = 59;
      }
      else {
        goto DoneTimer;
      }
    }
    delay(1000);
  }

DoneTimer: {}
  //  digitalWrite(Heater, LOW);
  //  digitalWrite(Fan, HIGH);
  //delay(1000);

}

void Despense_Output() {

  lcd.clear();
  lcd.setCursor(0, 0); lcd.print("     DESPENSING     ");
  lcd.setCursor(0, 0); lcd.print("      OUTPUT...     ");
  Open_Funnel();
  delay(5000);

  lcd.clear();
  lcd.setCursor(0, 0); lcd.print("      CLOSE         ");
  lcd.setCursor(0, 0); lcd.print("      FUNNEL.       ");
  Close_Funnel();
  delay(1000);

  lcd.clear();
  lcd.setCursor(0, 0); lcd.print("       PROCESS      ");
  lcd.setCursor(0, 0); lcd.print("      COMPLETE.     ");
  delay(2000);

}

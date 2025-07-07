// Helper function for comparing floats with a margin
bool inRange(float value, float target, float epsilon = 0.1) {
  return abs(value - target) < epsilon;
}

String classifyRipeness() {

  while (Serial1.available()) {
    char c = Serial1.read();
    if (c == '\n') {
      processData(slave1Buffer, "Slave 1");
      slave1Buffer = "";  // Clear buffer after processing
    } else {
      slave1Buffer += c;
    }
  }

  // Debug prints
  Serial.print("Acidity: ");
  Serial.println(acidity);
  Serial.print("Brix: ");
  Serial.println(brix);

  // Compare with some tolerance to handle floating point inaccuracies
  if (inRange(acidity, 1.89) && inRange(brix, 7.47)) {
    ripeness = "25%";
  } else if (inRange(acidity, 0.95) && inRange(brix, 4.15)) {
    ripeness = "50%";
  } else if (inRange(acidity, 0.95) && inRange(brix, 3.32)) {
    ripeness = "75%";
  } else if (inRange(acidity, 0.47) && inRange(brix, 1.66)) {
    ripeness = "100%";
  } else {
    ripeness = "UNKNOWN";
  }

  Serial.print("Ripeness: ");
  Serial.println(ripeness);

  return ripeness;
}

// void controlCooling() {
//   float currentTemp = dht.readTemperature();  // Read from DHT22

//   // Safety check
//   if (isnan(currentTemp)) {
//     Serial.println("Failed to read temperature from DHT22!");
//     return;
//   }

//   Serial.print("Target Temp: ");
//   Serial.print(setTemp);
//   Serial.print(" | Current Temp: ");
//   Serial.println(currentTemp);

//   digitalWrite(PUMP, RELAY_ON);

//   // Turn on Peltier until currentTemp reaches near setTemp
//   while (currentTemp - setTemp > 0.5) {
//     Serial.println("Cooling ON...");
//     digitalWrite(PELTIER, RELAY_ON);
//     delay(2000);  // Wait a bit before checking again
//     currentTemp = dht.readTemperature();

//     if (isnan(currentTemp)) {
//       Serial.println("Sensor error during cooling loop!");
//       break;
//     }

//     Serial.print("Temp: ");
//     Serial.println(currentTemp);
//   }

//   // When set temperature is reached
//   Serial.println("Target reached. Cooling OFF.");
//   digitalWrite(PELTIER, RELAY_OFF);
//   digitalWrite(PUMP, RELAY_OFF);
// }
void updateCooling(FruitController &ctrl) {
  float currentTemp = dht.readTemperature();
  if (isnan(currentTemp)) return;

  unsigned long now = millis();  // for timing
  DateTime nowTime = rtc.now();  // for real-time clock access


  switch (ctrl.state) {
    case IDLE:
      if (currentTemp >= ctrl.triggerTemp) {
        ctrl.active = true;
        ctrl.displayTemp = currentTemp;
        ctrl.state = COOLING;
        ctrl.lastActionTime = now;
        Serial.println(ctrl.name + " → Cooling Start");
      }
      break;

    case COOLING:
      if (now - ctrl.lastActionTime >= 180000) {  // 3 min
        ctrl.displayTemp -= 1.0;
        ctrl.lastActionTime = now;
        if (ctrl.displayTemp <= ctrl.setTemp) {
          ctrl.displayTemp = ctrl.setTemp;
          ctrl.state = HOLDING;
          ctrl.lastActionTime = now;
          Serial.println(ctrl.name + " → Holding at " + String(ctrl.setTemp));
        }
      }
      break;

    case HOLDING:
      if (now - ctrl.lastActionTime >= 300000) {  // 5 min
        ctrl.state = HEATING;
        ctrl.lastActionTime = now;
        Serial.println(ctrl.name + " → Heating Up");
      }
      break;

    case HEATING:
      if (now - ctrl.lastActionTime >= 180000) {  // 3 min
        ctrl.displayTemp += 1.0;
        ctrl.lastActionTime = now;
        if (ctrl.displayTemp >= ctrl.maxTemp) {
          ctrl.displayTemp = ctrl.maxTemp;
          ctrl.state = PUMPING;
          digitalWrite(PUMP, RELAY_ON);
          Serial.println(ctrl.name + " → PUMP ON");
        }
      }
      break;

    case PUMPING:
      if (currentTemp <= ctrl.setTemp + 0.2) {
        digitalWrite(PUMP, RELAY_OFF);
        ctrl.state = IDLE;
        ctrl.active = false;
        Serial.println(ctrl.name + " → Cooling cycle complete");
      }
      break;
  }

  // Display state if active
  if (fruit == ctrl.name && ctrl.active) {
    lcd.setCursor(0, 0);
    lcd.print(ctrl.name);
    lcd.print(12, 0);
    lcd.print(" R: ");
    lcd.print(ripeness);
    lcd.setCursor(0, 1);
    lcd.print("Set Temp: ");
    lcd.print(ctrl.setTemp, 1);
    lcd.print((char)223);
    lcd.print("C");
    lcd.setCursor(0, 2);
    lcd.print("T: ");
    lcd.print(currentTemp, 1);
    lcd.print((char)223);
    lcd.print("C");
    lcd.setCursor(10, 2);
    lcd.print("H: ");
    lcd.print(dht.readHumidity(), 1);  // Read and show humidity
    lcd.setCursor(0, 3);
    lcd.print("D&T:");
    lcd.setCursor(5, 3);
    char buffer[21];  // max 20 chars + null terminator
    snprintf(buffer, sizeof(buffer), "%02d/%02d/%02d %02d:%02d",
             nowTime.month(), nowTime.day(), nowTime.year() % 100,
             nowTime.hour(), nowTime.minute());
    lcd.print(buffer);
  }
}

void dataLogger() {
  DateTime now = rtc.now();
  humidity = dht.readHumidity();
  temperature = dht.readTemperature();

  // Only log if minute is divisible by 15 and it hasn't been logged yet
  if ((now.minute() % 15 == 0) && (now.minute() != lastLoggedMinute)) {
    lastLoggedMinute = now.minute();  // Update tracker

    dataFile = SD.open("logs.csv", FILE_WRITE);
    if (dataFile) {
      dataFile.print(now.timestamp(DateTime::TIMESTAMP_DATE));
      dataFile.print(",");
      dataFile.print(now.timestamp(DateTime::TIMESTAMP_TIME));
      dataFile.print(",");
      dataFile.print(fruit);
      dataFile.print(",");
      dataFile.print(temperature);
      dataFile.print(",");
      dataFile.print(humidity);
      dataFile.print(",");
      dataFile.print(setTemp);
      dataFile.print(",");
      dataFile.println(ripeness);
      dataFile.close();

      Serial.println("Data logged.");
    } else {
      Serial.println("ERROR writing to SD card");
    }
  }
}
void processData(String data, String source) {
  acidity = extractValue(data, "J");
  brix = extractValue(data, "T");

  if (acidity != -1 && brix != -1) {
    Serial.print(source);
    Serial.print(" -> J: ");
    Serial.print(acidity);
    Serial.print(" | T: ");
    Serial.println(brix);
  }
}

float extractValue(String input, String label) {
  int index = input.indexOf(label + ":");
  if (index == -1) return -1;  // Label not found
  int start = index + label.length() + 1;
  int end = input.indexOf(",", start);
  if (end == -1) end = input.length();
  return input.substring(start, end).toFloat();
}

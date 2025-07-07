void serialCommand() {
  String cmd = "";

  if (Serial.available() > 0) {
    cmd = Serial.readStringUntil('\n');
    cmd.trim();
  }

  if (cmd.length() <= 0) {
    return;
  } else if (cmd.startsWith("A")) {
    Serial.println("===PELTIER ON===");
    digitalWrite(PELTIER, RELAY_ON);
    delay(5000);
    digitalWrite(PELTIER, RELAY_OFF);
    Serial.println("===PELTIER OFF===");
  } else if (cmd.startsWith("C")) {
    // Turn ON IR LED
    sensor.enableIndicator();  // Optional visual indicator
    sensor.enableBulb(AS7265x_LED_IR);

    delay(1000);  // Let LED stabilize

    // Capture calibrated spectral readings
    sensor.takeMeasurements();

    // Turn OFF IR LED and indicator after reading
    sensor.disableBulb(AS7265x_LED_IR);
    sensor.disableIndicator();

    // Print all 18 calibrated channels
    Serial.println("=== All 18 Calibrated Channel Values ===");
    Serial.print("A: ");
    Serial.print(sensor.getCalibratedA());
    Serial.print("\t");
    Serial.print("B: ");
    Serial.print(sensor.getCalibratedB());
    Serial.print("\t");
    Serial.print("C: ");
    Serial.print(sensor.getCalibratedC());
    Serial.print("\t");
    Serial.print("D: ");
    Serial.print(sensor.getCalibratedD());
    Serial.print("\t");
    Serial.print("E: ");
    Serial.print(sensor.getCalibratedE());
    Serial.print("\t");
    Serial.print("F: ");
    Serial.print(sensor.getCalibratedF());
    Serial.println();

    Serial.print("G: ");
    Serial.print(sensor.getCalibratedG());
    Serial.print("\t");
    Serial.print("H: ");
    Serial.print(sensor.getCalibratedH());
    Serial.print("\t");
    Serial.print("R: ");
    Serial.print(sensor.getCalibratedR());
    Serial.print("\t");
    Serial.print("I: ");
    Serial.print(sensor.getCalibratedI());
    Serial.print("\t");
    Serial.print("S: ");
    Serial.print(sensor.getCalibratedS());
    Serial.println();

    Serial.print("J: ");
    Serial.print(sensor.getCalibratedJ());
    Serial.print("\t");
    Serial.print("T: ");
    Serial.print(sensor.getCalibratedT());
    Serial.print("\t");
    Serial.print("U: ");
    Serial.print(sensor.getCalibratedU());
    Serial.print("\t");
    Serial.print("V: ");
    Serial.print(sensor.getCalibratedV());
    Serial.print("\t");
    Serial.print("W: ");
    Serial.print(sensor.getCalibratedW());
    Serial.println();

    Serial.print("K: ");
    Serial.print(sensor.getCalibratedK());
    Serial.print("\t");
    Serial.print("L: ");
    Serial.println(sensor.getCalibratedL());

    Serial.println("=========================================\n");
  } else if (cmd.startsWith("D")) {
  }
}
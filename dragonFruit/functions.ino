void irProcess() {
  int sensorValue = digitalRead(ir1);

  if (sensorValue == LOW) {
    // LOW means obstacle/object detected (active LOW)
    Serial.println("DEFECT DRAGONFRUIT DETECTED");
  } else {
    // HIGH means no obstacle
    Serial.println("NO DEFECT");
  }

  delay(500); // Wait half a second before reading again
}
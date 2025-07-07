void resetProcess() {
  Serial.println("===RESETTING===");
  
  digitalWrite(MOLD_OUT, 0);
  delay(60000);
  digitalWrite(MOLD_OUT, 1);

  Serial.println("===DONE RESET===");
}
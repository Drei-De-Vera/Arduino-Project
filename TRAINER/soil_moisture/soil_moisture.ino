// Define the analog pin connected to the soil moisture sensor
const int moistureSensorPin = A0;

// Define thresholds for soil condition (adjust as needed)
const int dryThreshold = 700;
const int wetThreshold = 300;

void setup() {
  Serial.begin(9600); // Start serial communication
}

void loop() {
  // Read analog value from the soil moisture sensor
  int sensorValue = analogRead(moistureSensorPin);

  // Display the reading and interpret it
  Serial.print("Soil Moisture Value: ");
  Serial.print(sensorValue);
  Serial.print(" → Status: ");

  if (sensorValue < wetThreshold) {
    Serial.println("Wet");
  } else if (sensorValue >= wetThreshold && sensorValue <= dryThreshold) {
    Serial.println("Moist");
  } else {
    Serial.println("Dry");
  }

  delay(2000); // Wait 2 seconds before next reading
}

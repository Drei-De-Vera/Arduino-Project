// Pin definitions
const int soundSensorDigitalPin = 10;  // Digital pin for sound detection
const int soundSensorAnalogPin = A0;   // Analog pin for sound intensity

// Variables to store sensor data
int soundValueDigital = 0;  // Digital sound detection (HIGH or LOW)
int soundValueAnalog = 0;   // Analog sound intensity (0 to 1023)

void setup() {
  // Start serial communication
  Serial.begin(9600);
  
  // Set the sound sensor pins
  pinMode(soundSensorDigitalPin, INPUT);
  pinMode(soundSensorAnalogPin, INPUT);
}

void loop() {
  // Read the digital sound value (HIGH or LOW)
  soundValueDigital = digitalRead(soundSensorDigitalPin);
  
  // Read the analog sound value (sound intensity)
  soundValueAnalog = analogRead(soundSensorAnalogPin);
  
  // Print the results to the Serial Monitor
  Serial.print("Digital Sound Detection: ");
  if (soundValueDigital == HIGH) {
    Serial.println("Sound detected");
  } else {
    Serial.println("No sound detected");
  }

  Serial.print("Analog Sound Intensity: ");
  Serial.println(soundValueAnalog);
  
  // Add a small delay for stability
  delay(500);
}

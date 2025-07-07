// const int trigPin = 11;
// const int echoPin = 12;

// long duration;
// float distanceCm;

// void setup() {
//   Serial.begin(9600);

//   pinMode(trigPin, OUTPUT);
//   pinMode(echoPin, INPUT);
// }

// void loop() {
//   // Send ultrasonic pulse
//   digitalWrite(trigPin, LOW);
//   delayMicroseconds(2);
//   digitalWrite(trigPin, HIGH);
//   delayMicroseconds(10);
//   digitalWrite(trigPin, LOW);

//   // Measure the duration of the echo pulse
//   duration = pulseIn(echoPin, HIGH);

//   // Calculate the distance in cm (speed of sound: 343 m/s or 0.0343 cm/µs)
//   distanceCm = duration * 0.0343 / 2;

//   Serial.print("Distance: ");
//   Serial.print(distanceCm);
//   Serial.println(" cm");

//   delay(500);
// }

// #include <Wire.h>
// #include <MPU6050.h>

// MPU6050 gyroSensor;

// int16_t gx, gy, gz;
// int16_t gx_offset = 0, gy_offset = 0, gz_offset = 0;

// void setup() {
//   Serial.begin(9600);
//   Wire.begin();
//   gyroSensor.initialize();

//   if (!gyroSensor.testConnection()) {
//     Serial.println("MPU6050 connection failed.");
//     while (1);
//   }

//   Serial.println("Calibrating... Keep MPU6050 still");
//   delay(2000); // Give time to keep sensor still

//   // Take 100 samples to calculate offset
//   long sumX = 0, sumY = 0, sumZ = 0;
//   for (int i = 0; i < 100; i++) {
//     gyroSensor.getRotation(&gx, &gy, &gz);
//     sumX += gx;
//     sumY += gy;
//     sumZ += gz;
//     delay(5);
//   }

//   gx_offset = sumX / 100;
//   gy_offset = sumY / 100;
//   gz_offset = sumZ / 100;

//   Serial.println("Calibration done.");
// }

// void loop() {
//   gyroSensor.getRotation(&gx, &gy, &gz);

//   int16_t correctedX = gx - gx_offset;
//   int16_t correctedY = gy - gy_offset;
//   int16_t correctedZ = gz - gz_offset;

//   Serial.print("Gyro X: "); Serial.print(correctedX);
//   Serial.print(" | Gyro Y: "); Serial.print(correctedY);
//   Serial.print(" | Gyro Z: "); Serial.println(correctedZ);

//   delay(500);
// }

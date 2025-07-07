#include <Wire.h>
#include "SparkFun_AS7265X.h"

AS7265X sensor;

void setup() {
  Serial.begin(9600);   // For debugging
  

  if (!sensor.begin()) {
    Serial.println("AS7265x not detected. Check wiring.");
    while (1)
      ;
  }

  Serial.println("AS7265x Sensor Initialized.");
}

void loop() {
  sensor.enableIndicator();
  sensor.enableBulb(AS7265x_LED_IR);
  delay(1000);
  sensor.takeMeasurements();
  sensor.disableBulb(AS7265x_LED_IR);
  sensor.disableIndicator();

  // Read all values
  float A = sensor.getCalibratedA();
  float B = sensor.getCalibratedB();
  float C = sensor.getCalibratedC();
  float D = sensor.getCalibratedD();
  float E = sensor.getCalibratedE();
  float F = sensor.getCalibratedF();
  float G = sensor.getCalibratedG();
  float H = sensor.getCalibratedH();
  float R = sensor.getCalibratedR();
  float I = sensor.getCalibratedI();
  float S = sensor.getCalibratedS();
  float J = sensor.getCalibratedJ();
  float T = sensor.getCalibratedT();
  float U = sensor.getCalibratedU();
  float V = sensor.getCalibratedV();
  float W = sensor.getCalibratedW();
  float K = sensor.getCalibratedK();
  float L = sensor.getCalibratedL();

  // Send all data to Master as CSV format
  Serial.print("A:");
  Serial.print(A);
  Serial.print(",");
  Serial.print("B:");
  Serial.print(B);
  Serial.print(",");
  Serial.print("C:");
  Serial.print(C);
  Serial.print(",");
  Serial.print("D:");
  Serial.print(D);
  Serial.print(",");
  Serial.print("E:");
  Serial.print(E);
  Serial.print(",");
  Serial.print("F:");
  Serial.print(F);
  Serial.print(",");
  Serial.print("G:");
  Serial.print(G);
  Serial.print(",");
  Serial.print("H:");
  Serial.print(H);
  Serial.print(",");
  Serial.print("R:");
  Serial.print(R);
  Serial.print(",");
  Serial.print("I:");
  Serial.print(I);
  Serial.print(",");
  Serial.print("S:");
  Serial.print(S);
  Serial.print(",");
  Serial.print("J:");
  Serial.print(J);
  Serial.print(",");
  Serial.print("T:");
  Serial.print(T);
  Serial.print(",");
  Serial.print("U:");
  Serial.print(U);
  Serial.print(",");
  Serial.print("V:");
  Serial.print(V);
  Serial.print(",");
  Serial.print("W:");
  Serial.print(W);
  Serial.print(",");
  Serial.print("K:");
  Serial.print(K);
  Serial.print(",");
  Serial.print("L:");
  Serial.print(L);
  Serial.println();

  Serial.println("Data sent to Master.");

  delay(5000);
}

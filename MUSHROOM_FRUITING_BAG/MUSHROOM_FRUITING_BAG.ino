//MUSHROOM PINOUT
#define Stepper_P 22
#define Stepper_D 24
#define Stepper_E 26

#define mtrComp_Down 2
#define mtrComp_Up   3
#define btnStart 4

long PosValue, CTR;
int RotateFL = 0;

void setup() {
  // put your setup code here, to run once:

  Serial.begin(9600);
  delay(1000);

  pinMode(Stepper_P, OUTPUT); digitalWrite(Stepper_P, LOW);
  pinMode(Stepper_D, OUTPUT); digitalWrite(Stepper_D, LOW);
  pinMode(Stepper_E, OUTPUT); digitalWrite(Stepper_E, HIGH);

  pinMode(mtrComp_Down, OUTPUT); digitalWrite(mtrComp_Down, HIGH);
  pinMode(mtrComp_Up, OUTPUT);   digitalWrite(mtrComp_Up, HIGH);
  pinMode(btnStart, INPUT_PULLUP);      //digitalWrite(mtrComp_Up, HIGH);
  Compressor_Up();

  Serial.println("READY");

//    Test();

}

void loop() {
  // put your main code here, to run repeatedly:

  if (digitalRead(btnStart) == LOW) {
    delay(100); while (digitalRead(btnStart) == LOW); delay(100);
    stpMovingRight();

    if (RotateFL < 2) {
      RotateFL++;
    }
    else {
      RotateFL = 0;
      stpMovingRight_Jog();
    }
    
    Compressor_Down(); delay(500);
    Compressor_Up(); delay(500);
  }
}

void Test() {
  while (1) {

    if (Serial.available()) {
      switch (Serial.read()) {

        case 'A':
          Compressor_Down();
          break;

        case 'B':
          Compressor_Up();
          break;

        case 'C':
          stpMovingRight();
          break;

        case 'D':
          stpMovingRight_Half();
          break;

        case 'E':
          stpMovingLeft_Half();
          break;

        case 'F':
          stpMovingRight_Jog ();
          break;          

      }
    }

  }
}

void stpMovingRight() {

  Serial.print("MOVING RIGHT ... ");
  digitalWrite(Stepper_E, LOW); // LOW = Direction to Right
  digitalWrite(Stepper_D, LOW); // LOW = Direction to Right
  PosValue = 105000;
  for (CTR = 0; CTR <= PosValue; CTR++) { // goes from 180 degrees to 0 degrees
    digitalWrite(Stepper_P, !digitalRead(Stepper_P));
    delayMicroseconds(100);
  }

  digitalWrite(Stepper_E, HIGH);
  Serial.println("DONE");

}
void stpMovingRight_Half() {

  Serial.print("MOVING RIGHT ... ");
  digitalWrite(Stepper_E, LOW); // LOW = Direction to Right
  digitalWrite(Stepper_D, LOW); // LOW = Direction to Right
  PosValue = 1500;
  for (CTR = 0; CTR <= PosValue; CTR++) { // goes from 180 degrees to 0 degrees
    digitalWrite(Stepper_P, !digitalRead(Stepper_P));
    delayMicroseconds(200);
  }

  digitalWrite(Stepper_E, HIGH);
  Serial.println("DONE");

}

void stpMovingLeft_Half() {

  Serial.print("MOVING LEFT ... ");
  digitalWrite(Stepper_E, LOW); // LOW = Direction to Right
  digitalWrite(Stepper_D, HIGH); // LOW = Direction to Right
  PosValue = 750;
  for (CTR = 0; CTR <= PosValue; CTR++) { // goes from 180 degrees to 0 degrees
    digitalWrite(Stepper_P, !digitalRead(Stepper_P));
    delayMicroseconds(200);
  }

  digitalWrite(Stepper_E, HIGH);
  Serial.println("DONE");

}

void stpMovingRight_Jog() {

  Serial.print("MOVING LEFT ... ");
  digitalWrite(Stepper_E, LOW); // LOW = Direction to Right
  digitalWrite(Stepper_D, LOW); // LOW = Direction to Right
  PosValue = 750;
  for (CTR = 0; CTR <= PosValue; CTR++) { // goes from 180 degrees to 0 degrees
    digitalWrite(Stepper_P, !digitalRead(Stepper_P));
    delayMicroseconds(200);
  }

  digitalWrite(Stepper_E, HIGH);
  Serial.println("DONE");

}

void Compressor_Down () {

  Serial.print("COMPRESSOR DOWN ...");
  digitalWrite(mtrComp_Down, LOW); delay(40000); // 50SECS DELAY
  digitalWrite(mtrComp_Down, HIGH); delay(100);
  Serial.println("DONE");

}

void Compressor_Up () {

  Serial.print("COMPRESSOR UP ...");
  digitalWrite(mtrComp_Up, LOW); delay(40000); // 50SECS DELAY
  digitalWrite(mtrComp_Up, HIGH); delay(100);
  Serial.println("DONE");

}

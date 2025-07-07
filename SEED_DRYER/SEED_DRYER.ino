#include "max6675.h"            // ***** THERMOCOUPLE LIBRARY *****
#include <PZEM004T.h>           // ***** PZEMM004T LIBRARY *****
#include <SoftwareSerial.h>     // Arduino IDE <1.6.6
#include <LiquidCrystal_I2C.h>  // ***** I2C LCD LIBRARY *****


LiquidCrystal_I2C lcd(0x27, 20, 4);

PZEM004T pzem(12, 13); // (RX,TX) connect to TX,RX of PZEM
IPAddress ip(192, 168, 1, 1);

#define btn15Mins   A5
#define btn30Mins   A4
#define btnManualFunnel A3  

#define lmtFunnel_O 2
#define lmtFunnel_C 3

#define Heater_Sck 5
#define Heater_Cs  6
#define Heater_So  7

#define mtrFunnel_O 9
#define mtrFunnel_C 8
#define Fan    10
#define Heater 11

int TempValue = 0;
int TempMin = 40, TempMax = 45;
int Min = 0, Sec = 0;
int TempMaxFL = 0, TempMinFL = 0;

float Volt = 0.0, Watt = 0.0;

MAX6675 TC(Heater_Sck, Heater_Cs, Heater_So);


void setup() {
  // put your setup code here, to run once:

  Serial.begin(9600);

  pzem.setAddress(ip);
  pinMode(mtrFunnel_O, OUTPUT); digitalWrite(mtrFunnel_O, HIGH);
  pinMode(mtrFunnel_C, OUTPUT); digitalWrite(mtrFunnel_C, HIGH);
  pinMode(Heater, OUTPUT); digitalWrite(Heater, HIGH);
  pinMode(Fan, OUTPUT);    digitalWrite(Fan, HIGH);

  pinMode(lmtFunnel_O, INPUT_PULLUP);
  pinMode(lmtFunnel_C, INPUT_PULLUP);
  pinMode(btn15Mins, INPUT);
  pinMode(btn30Mins, INPUT);
  pinMode(btnManualFunnel, INPUT_PULLUP); 

  lcd.clear();
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 1); lcd.print("INITIALIZING MOTOR, ");
  lcd.setCursor(0, 2); lcd.print("PLEASE WAIT ...");
  Close_Funnel(); delay(1000);

  lcd.clear();
  lcd.setCursor(0, 1); lcd.print("      WELCOME,      ");
  lcd.setCursor(0, 2); lcd.print("     SEED DRYER.    ");

  delay(2000);
  //    Serial.println("READY");
  lcd.setCursor(0, 0); lcd.print("SELECT TIME:        ");
  lcd.setCursor(0, 1); lcd.print("                    ");
  lcd.setCursor(0, 2); lcd.print("[LEFT] - 30 MINUTES ");
  lcd.setCursor(0, 3); lcd.print("[RIGHT]- 60 MINUTES ");


  //  Test_Program();
}

void loop() {
  // Get_Volt_Watt();
  // Check for button presses to select time for automatic dispensing
  if (digitalRead(btn15Mins) == LOW) {
    delay(50); while (digitalRead(btn15Mins) == LOW); delay(50);
    TIMER(30, 0);
    Despense_Output();
  } else if (digitalRead(btn30Mins) == LOW) {
    delay(50); while (digitalRead(btn30Mins) == LOW); delay(50);
    TIMER(60, 0);
    Despense_Output();
  }
  
  // Manual funnel control logic - Only open when the 3rd button is pressed
  if (digitalRead(btnManualFunnel) == LOW) {  // Button pressed
    // Open the funnel when the button is held down
    Open_Funnel();
  } else {
    // Close the funnel if the button is not pressed
    Close_Funnel();
  }
}


void Test_Program() {

  while (1) {

    if (Serial.available()) {
      switch (Serial.read()) {

        case 'A': // TEST ALL LIMIT SWITCH & BUTTONS STATE
          Serial.print(digitalRead(lmtFunnel_O)); Serial.print("_");
          Serial.print(digitalRead(lmtFunnel_C)); Serial.print("_");
          Serial.print(digitalRead(btn15Mins)); Serial.print("_");
          Serial.println(digitalRead(btn30Mins)); //Serial.print("_");
          break;

        case 'B': // GET TEMPERATURE VALUE
          TempValue = TC.readCelsius();
          Serial.println(TempValue);
          break;

        case 'C': // OPEN FUNNEL
          Open_Funnel();
          break;

        case 'D': // CLOSE FUNNEL
          Close_Funnel();
          break;

        case 'E': // FAN OPEN
          digitalWrite(Fan, LOW);
          break;

        case 'F': // FAN CLOSE
          digitalWrite(Fan, HIGH);
          break;

        case 'G': // HEATER ON
          digitalWrite(Heater, LOW);
          break;

        case 'H': // HEATER OFF
          digitalWrite(Heater, HIGH);
          break;

      }
    }

  }
}

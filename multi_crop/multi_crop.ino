#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 20, 4);
#include "RTClib.h"
RTC_DS1307 rtc;
#include "SparkFun_AS7265X.h"
AS7265X sensor;
#include "DHT.h"           // Include DHT library
#define DHTPIN 7           // Digital pin connected to the DHT sensor
#define DHTTYPE DHT22      // DHT 22 (AM2302)
DHT dht(DHTPIN, DHTTYPE);  // Initialize DHT sensor
#include <SPI.h>
#include <SD.h>

#define PELTIER 12
#define PUMP 8
#define BTN_PA 2
#define BTN_DF 3

#define RELAY_OFF HIGH
#define RELAY_ON LOW

const int chipSelect = 53;
File dataFile;

float acidity, brix;
String fruit = "NONE";
String ripeness = "N/A";
float setTemp = 0;
float temperature = 0;
float humidity = 0;
String slave1Buffer = "";

bool lastShown = true;
int lastLoggedMinute = -1;

enum FruitState { IDLE, COOLING, HOLDING, HEATING, PUMPING };

struct FruitController {
  String name;
  float triggerTemp;
  float setTemp;
  float maxTemp;
  float displayTemp;
  FruitState state;
  unsigned long lastActionTime;
  bool active;

  FruitController(String n, float trig, float setT, float maxT, float dispT)
    : name(n), triggerTemp(trig), setTemp(setT), maxTemp(maxT),
      displayTemp(dispT), state(IDLE), lastActionTime(0), active(false) {}
};
FruitController pineapple("PINEAPPLE", 18.0, 10.0, 12.0, 18.0);
FruitController dragonfruit("DRAGONFRUIT", 16.0, 7.0, 9.0, 16.0);


void setup() {
  Serial.begin(9600);
  Serial1.begin(9600);
  dht.begin();
  temperature = dht.readTemperature();
  humidity = dht.readHumidity();
  lcd.init();
  lcd.backlight();
  delay(1000);

  pinMode(PELTIER, OUTPUT);
  digitalWrite(PELTIER, RELAY_OFF);
  pinMode(PUMP, OUTPUT);
  digitalWrite(PUMP, RELAY_OFF);
  pinMode(BTN_PA, INPUT_PULLUP);
  pinMode(BTN_DF, INPUT_PULLUP);

  // if (sensor.begin() == false) {
  //   Serial.println("AS7265X not found. Check wiring.");
  //   while (1)
  //     ;
  // }

  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1) delay(10);
  }
  if (!rtc.isrunning()) {
    Serial.println("RTC is NOT running, let's set the time!");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }

  if (!SD.begin(chipSelect)) {
    Serial.println(" SD Card initialization failed");
  } else {
    Serial.println("SD card initialized");
    dataFile = SD.open("logs.csv", FILE_WRITE);
    if (dataFile) {
      dataFile.println("DATE,TIME,FRUIT,TEMPERATURE,HUMIDITY,SET TEMPERATURE,RIPENESS");
      dataFile.close();
    }
  }

  lcd.clear();
  lcd.setCursor(0, 1);
  lcd.print("     WELCOME TO");
  lcd.setCursor(0, 2);
  lcd.print("  MULTI-CROP SYSTEM");
  delay(5000);
  Serial.println("===SYSTEM READY===");
  fruitSelectionScreen();
}

void loop() {
  // serialCommand();
  static unsigned long button1PressStart = 0;
  static bool button1Pressed = false;
  static unsigned long button2PressStart = 0;
  static bool button2Pressed = false;

  byte buttonState1 = digitalRead(BTN_PA);
  byte buttonState2 = digitalRead(BTN_DF);

  // Button 1 - Pineapple
  if (buttonState1 == LOW && !button1Pressed) {
    button1PressStart = millis();
    button1Pressed = true;
  }
  if (buttonState1 == HIGH && button1Pressed) {
    unsigned long pressDuration = millis() - button1PressStart;
    if (pressDuration >= 2000) {
      Serial.println("Pineapple Button Pressed");
      fruit = "PINEAPPLE";
      setTemp = 10.0;
      Serial.print("setTemp after button press: ");
      Serial.println(setTemp);
      ripeness = classifyRipeness();
      pineapple.state = IDLE;  // reset state
      pineapple.active = false;

      dataLogger();
    }
    button1Pressed = false;
  }

  // Button 2 - Dragonfruit
  if (buttonState2 == LOW && !button2Pressed) {
    button2PressStart = millis();
    button2Pressed = true;
  }
  if (buttonState2 == HIGH && button2Pressed) {
    unsigned long pressDuration = millis() - button2PressStart;
    if (pressDuration >= 2000) {
      Serial.println("Dragonfruit Button Pressed");
      fruit = "DRAGONFRUIT";
      setTemp = 7.0;
      Serial.print("setTemp after button press: ");
      Serial.println(setTemp);
      ripeness = classifyRipeness();
      dragonfruit.state = IDLE;
      dragonfruit.active = false;

      dataLogger();
    }
    button2Pressed = false;
  }
  if (fruit == "PINEAPPLE") updateCooling(pineapple);
  if (fruit == "DRAGONFRUIT") updateCooling(dragonfruit);

  // alternateDisplay();
}

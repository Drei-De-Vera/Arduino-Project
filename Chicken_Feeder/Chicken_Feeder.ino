// Include necessary libraries
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <RTClib.h>
#include <Keypad.h>
#include <EEPROM.h>

// Define constants and pin assignments
#define OFF HIGH
#define ON LOW
// CHICKEN FEEDER PINOUT
const int FEEDER = 11;
const int MOVE_RIGHT = 8;
const int MOVE_LEFT = 9;
const int RIGHT_LIMIT = 4;
const int LEFT_LIMIT = 5;
const int TRIG = 7;
const int ECHO = 6;
const int BUZZER = 12;
const int WATER = 10;

// Global variables
bool hasFedToday[3] = {false, false, false};
bool movingRight = true;
bool movingLeft = false;
bool cycleDone = false;
int lastFeedingHour = -1;
int lastFeedingMinute = -1;
unsigned long debounceTime = 100;
unsigned long lastDebounce = 0;
String newTime = "__:__";  // Default placeholder
String mealTimes[3] = {"__:__","__:__","__:__"}; // Default times in HHMM format

// LCD and RTC
LiquidCrystal_I2C lcd(0x27, 20, 4);
RTC_DS3231 rtc;

// Keypad configuration
const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {36, 34, 32, 30};
byte colPins[COLS] = {28, 26, 24, 22};
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);
// Meal schedule struct
struct MealTime {
    int hour;
    int minute;
};

// Function to input time from keypad
MealTime getTimeInput() {
    int timeInput = 0, digitCount = 0;
    unsigned long startTime = millis();
    const unsigned long inputTimeout = 30000; // 30-second timeout

    while (millis() - startTime < inputTimeout) {  // Timeout after 30 seconds
        char key = keypad.getKey();
        if (!key) continue;

        startTime = millis();  // Reset timeout on key press

        if (key >= '0' && key <= '9' && digitCount < 4) {
            timeInput = (timeInput * 10) + (key - '0');
            digitCount++;
        } else if (key == 'C') {  // Reset input
            timeInput = 0;
            digitCount = 0;
        } else if (key == 'D' && digitCount > 0) {  // Delete last digit
            timeInput /= 10;
            digitCount--;
        } else if (key == 'B') {
            return {-1, -1};
        }else if (key == 'A' && digitCount == 4) {  // Confirm input
            int newHour = timeInput / 100;
            int newMinute = timeInput % 100;
            
            if (newHour < 24 && newMinute < 60) {
                return {newHour, newMinute};  // Return valid time
            } else {
                lcd.setCursor(0, 1);
                lcd.print(F("Invalid Time!   "));
                delay(1000);
                lcd.setCursor(0, 1);
                lcd.print(F("Time: --:--     "));
                timeInput = 0;
                digitCount = 0;
            }
        }
        // Update LCD display
        lcd.setCursor(6, 1);
        lcd.print((timeInput / 100) < 10 ? "0" : ""); lcd.print(timeInput / 100);
        lcd.print(":");
        lcd.print((timeInput % 100) < 10 ? "0" : ""); lcd.print(timeInput % 100);
    }

    // If timeout occurs, return invalid time
    return {-1, -1};
}

// Menu
int menuIndex = 0;
const char* menuItems[] = {
    "1.View Schedule",
    "2.Edit Schedule",
    "3.View RTC",
    "4.Start Feeding"
};
const int menuLength = sizeof(menuItems) / sizeof(menuItems[0]);
const int lastMenuIndex = -1;

// Function prototypes
void initializeHardware();
void showMenu();
void handleKeypad();
void checkBinLevel();
void checkFeedingTime();
void viewSchedule();
void editSchedule();
void viewRTC();
void startFeedingCycle();

void setup() {
    Serial.begin(9600);
    lcd.init();
    lcd.backlight();

    pinMode(FEEDER, OUTPUT);
    pinMode(MOVE_RIGHT, OUTPUT);
    pinMode(MOVE_LEFT, OUTPUT);
    pinMode(WATER, OUTPUT);
    pinMode(BUZZER, OUTPUT);
    pinMode(RIGHT_LIMIT, INPUT_PULLUP);
    pinMode(LEFT_LIMIT, INPUT_PULLUP);
    pinMode(TRIG, OUTPUT);
    pinMode(ECHO, INPUT);

    digitalWrite(FEEDER, OFF);
    digitalWrite(MOVE_RIGHT, OFF);
    digitalWrite(MOVE_LEFT, OFF);
    digitalWrite(WATER, OFF);
    digitalWrite(BUZZER, ON);

    if (!rtc.begin()) {
        Serial.println("RTC Error: Initialization Failed!");
        while (1);
    }

    // Set RTC once (then comment this line out after successful upload)
    // rtc.adjust(DateTime(2025, 4, 9, 19, 20, 0)); // Set to April 9, 2025, 19:19:00

    if (rtc.lostPower()) {
        Serial.println("RTC lost power! Setting default time...");
        rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    }

    for (int i = 0; i < 3; i++) {
        int hour = EEPROM.read(i * 2);
        int minute = EEPROM.read(i * 2 + 1);

        // If EEPROM contains invalid values, set to default "__:__"
        if (hour < 24 && minute < 60) {
            mealTimes[i] = (hour < 10 ? "0" : "") + String(hour) + ":" +
                           (minute < 10 ? "0" : "") + String(minute);
        } else {
            mealTimes[i] = "__:__";  // Set default if EEPROM values are corrupted
        }
    }
    
    showMenu(); // Call this to ensure the menu appears at startup
}

void loop() {
    Serial.print(digitalRead(RIGHT_LIMIT));
    Serial.print("||");
    Serial.print(digitalRead(LEFT_LIMIT));
    delay(500);
    checkBinLevel();
    checkFeedingTime();
    handleKeypad();
}
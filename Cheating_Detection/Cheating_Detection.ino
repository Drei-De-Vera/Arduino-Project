#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

int thisUnit = 1; // change this depends on the device number
String SSID = "PLDT_Home_46F62";
String PASSWORD = "pldthome";

#define MIC_PIN A0  // Analog input pin

int micValue = 0;
bool haveNoise = false;
unsigned long lastNoiseTime = 0;    // Timestamp of last noise detection
const int noiseCooldown = 5000;     // 5 second cooldown in millisecond.s

// Built-in blue LED on ESP8266 NodeMCU
const int LED_PIN = LED_BUILTIN; // Usually GPIO 2 on NodeMCU

// Create web server instance
ESP8266WebServer server(80);

void setup() {
    Serial.begin(115200);
    
    // Initialize LED pin
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, HIGH);  // LED OFF initially (ESP8266 LED is active LOW)
    
    // Connect to WiFi
    WiFi.begin(SSID, PASSWORD);
    Serial.print("Connecting to WiFi");
    
    // Blink LED while connecting to WiFi
    while (WiFi.status() != WL_CONNECTED) {
        digitalWrite(LED_PIN, HIGH);  // LED ON (ESP8266 LED is active LOW)
        delay(100);
        digitalWrite(LED_PIN, LOW);   // LED OFF
        delay(100);
        Serial.print(".");
    }
    
    // Keep LED on when connected
    digitalWrite(LED_PIN, HIGH);  // LED OFF (active LOW)
    
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
    
    // Set up mDNS responder
    String hostName = "exam_" + String(thisUnit);
    if (MDNS.begin(hostName)) {
        Serial.println("mDNS responder started as " + hostName + ".local");
    } else {
        Serial.println("Error setting up MDNS responder!");
    }

    // Set up server routes
    server.on("/", handleRoot);
    server.onNotFound(handleNotFound);

    // Start server
    server.begin();
    Serial.println("HTTP server started");
}   

void loop() {
    // Handle incoming client requests
    server.handleClient();
    MDNS.update();
    
    // Read the microphone value
    micValue = analogRead(MIC_PIN);
    Serial.println(micValue);
    
    unsigned long currentTime = millis();
    
    // Detect if sound exceeds a threshold
    if (micValue >= 20) {  // Adjust this threshold based on your sensor
        Serial.println("Loud Sound Detected!");
        
        // When noise is detected, set the state to 1 and start the 5-second timer
        haveNoise = true;
        lastNoiseTime = currentTime;
            
        // Turn LED on when noise detected (ESP8266 LED is active LOW)
        digitalWrite(LED_PIN, LOW);
    } else if (haveNoise && (currentTime - lastNoiseTime >= noiseCooldown)) {
        // After 5 seconds with no new noise, reset the state
        haveNoise = false;
        // Turn LED off when no noise (ESP8266 LED is active HIGH)
        digitalWrite(LED_PIN, HIGH);
    }
    
    delay(50);  // Short delay to prevent too many readings
}

// Handle root path - return current noise state
void handleRoot() {
    server.send(200, "text/plain", String(haveNoise ? "1" : "0"));
    Serial.println("Request received - Noise state: " + String(haveNoise ? "1" : "0"));
}

// Handle 404 errors
void handleNotFound() {
    String message = "File Not Found\n\n";
    message += "URI: ";
    message += server.uri();
    message += "\nMethod: ";
    message += (server.method() == HTTP_GET) ? "GET" : "POST";
    message += "\nArguments: ";
    message += server.args();
    message += "\n";
    
    for (uint8_t i = 0; i < server.args(); i++) {
        message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
    }
    
    server.send(404, "text/plain", message);
}

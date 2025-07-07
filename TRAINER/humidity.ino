#include <dht.h>

dht DHT;

int sensor_pin = 10;

void setup(){
  Serial.begin(9600);
}

void loop()
{
  int chk = DHT.read11(sensor_pin);
  Serial.print("Temperature = ");
  Serial.print(DHT.temperature);
   Serial.println();
 
  Serial.print("Humidity = ");
  Serial.print(DHT.humidity);
   Serial.println();
   Serial.println();
  delay(4000);
}

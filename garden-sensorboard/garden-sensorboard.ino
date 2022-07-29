// code to be run on the ESP32 module
#include "Led.h"
#include "Photoresistor.h"
#include "TemperatureSensor.h"
#include "httpService.h"

// ????????
#define PIN_TEMP 34
#define PIN_LED 35
#define PIN_PHOTO 32

const char* ssid = "Mi10";
const char* password = "12345678";
const char* serviceURI = "";
TemperatureSensor *temp;
Led *led;
Photoresistor *photo;
httpService* http;

void setup() {
  temp = new TemperatureSensor(PIN_TEMP);
  led = new Led(PIN_LED);
  photo = new Photoresistor(PIN_PHOTO);
  http = new httpService("Mi10", password, serviceURI);
  http->on();
}

void loop() {
  delay(10000);
  http->send(temp -> getTemperature(), photo -> getLight());
}

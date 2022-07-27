// code to be run on the ESP32 module
#include "Led.h"
#include "Photoresistor.h"
#include "TemperatureSensor.h"
#include "httpService.h"

// ????????
#define PIN_TEMP G34
#define PIN_LED G35
#define PIN_PHOTO G32

const char* ssid = "Mi10";
const char* password = "12345678";
const char *serviceURI = "https://?????????????????????????????????????????????????????????";
TemperatureSensor *temp;
Led *led;
Photoresistor *photo;
httpService* http;

void setup() {
  temp = new TemperatureSensor(PIN_TEMP);
  led = new Led(PIN_LED);
  photo = new Photoresistor(PIN_PHOTO);
  http = new httpService(ssid, password, service)
  http->on()
}

void loop() {
  delay(10000);
  http->send(temp -> getTemperature(), photo -> getLight())
}

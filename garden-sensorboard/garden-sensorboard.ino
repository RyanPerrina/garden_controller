// code to be run on the ESP32 module
#include "Led.h"
#include "Photoresistor.h"
#include "TemperatureSensor.h"

// ????????
#define PIN_TEMP G34
#define PIN_LED G35
#define PIN_PHOTO G32

TemperatureSensor *temp;
Led *led;
Photoresistor *photo;

void setup() {
  temp = new TemperatureSensor(PIN_TEMP);
  led = new Led(PIN_LED);
  photo = new Photoresistor(PIN_PHOTO);

}

void loop() {
  delay(10000);
  l = photo -> getLight();
  t = temp -> getTemperature();

  // facci qualcosa con quelli
}

#include "TemperatureSensor.h"
#include <Arduino.h>

TemperatureSensor :: TemperatureSensor(int pin){
  this -> pin = pin;
  pinMode(this -> pin, INPUT);
}

int TemperatureSensor :: getTemperature(){
  int value = map(analogRead(this -> pin), 0, 4096, 1, 5);
  return value;
}

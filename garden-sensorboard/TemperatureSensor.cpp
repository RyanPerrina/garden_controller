#include "TemperatureSensor.h"
#include <Arduino.h>

TemperatureSensor :: TemperatureSensor(int pin){
  this -> pin = pin;
}

float TemperatureSensor :: getTemperature(){
  int value = analogRead(this -> pin);
  float value_in_mV = value * (5000 / 1024.0);     // value in mV
  float value_in_C = (value_in_mV - 500) * 0.1;    // value in Celsius
  return value_in_C;
}

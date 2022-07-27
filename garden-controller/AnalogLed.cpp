#include "AnalogLed.h"
#include "Arduino.h"

AnalogLed :: AnalogLed(int pin){
  this -> pin = pin;
  pinMode(pin ,OUTPUT);
}

void AnalogLed :: switchOn(){
  digitalWrite(pin, HIGH);
}

void AnalogLed :: switchOff(){
  digitalWrite(pin, LOW);
};

void AnalogLed :: setIntensity(int intensity){
  intensity = map(intensity, 0, 4, 0, 255);
  analogWrite(pin, intensity);
}

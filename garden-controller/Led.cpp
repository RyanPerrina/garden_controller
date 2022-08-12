#include "Led.h"
#include "Arduino.h"

Led::Led(int pin){
  this->pin = pin;
  pinMode(pin,OUTPUT);
  this->state = LedState::OFF;
}

void Led::switchOn(){
  digitalWrite(pin,HIGH);
  this->state = LedState::ON;

}

void Led::switchOff(){
  digitalWrite(pin,LOW);
  this->state = LedState::OFF;
};

bool Led::isLedOn(){
  return this->state == LedState::ON;
}

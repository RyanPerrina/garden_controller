#include "Led.h"
#include "Arduino.h"

Led::Led(int pin){
  this -> pin = pin;
  this -> isOn = false;
  pinMode(pin,OUTPUT);
}

void Led::switchOn(){
  digitalWrite(pin,HIGH);
  this -> isOn = true;
}

void Led::switchOff(){
  digitalWrite(pin,LOW);
  this -> isOn = false;
};

void Led::toggle(){
  if (this -> isOn){
    switchOff();
  } else {
    switchOn();
  }
}

#include <Arduino.h>
#include "IrrigationSystem.h"

#define IRRIGATE_TIME 2000
#define SLEEP_TIME 60000

IrrigationSystem :: IrrigationSystem(int pin) {
  this -> pin = pin;
  this -> speed = 0;
  this -> pos = 0;
  this -> direction = 1;
  
  this -> on();
  this -> setPosition(0);
  this -> off();
}

void IrrigationSystem :: setSpeed(int value){
  this -> speed = value;
}

void IrrigationSystem :: irrigate(){
  this -> on();

  bool timeElapsed = false;
  while(!timeElapsed){
    this -> pos = this -> pos + this -> direction;
    if (this -> pos == 0 || this -> pos == 180){
      this -> direction = -this -> direction;
    }
    this -> setPosition(this -> pos);
    delay(10 / this -> speed);
  }
  
  this -> off();
}

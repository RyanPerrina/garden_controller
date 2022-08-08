#include "IrrigationSystem.h"

#define IRRIGATE_TIME 5000
#define SLEEP_TIME 60000

IrrigationSystem::IrrigationSystem(int pin):ServoMotorImpl(pin) {
  this -> pin = pin;
  this -> speed = 0;
  this -> pos = 0;
  this -> direction = 1;
  
  this -> on();
  this -> setPosition(0);
  delay(1000);
  this -> off();
}

void IrrigationSystem :: setSpeed(int value){
  this -> speed = value;
}

void IrrigationSystem :: irrigate(){
  this -> on();

  double start = millis();
  bool timeElapsed = false;
  while(!timeElapsed){
    this -> pos = this -> pos + this -> direction;
    if (this -> pos == 0 || this -> pos == 180){
      this -> direction = -this -> direction;
    }
    if(millis() - start >= IRRIGATE_TIME){
      timeElapsed = true;
    }
    this -> setPosition(this -> pos);
    delay(20 / this -> speed);
  }
  
  this -> off();
}

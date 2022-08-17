#include "IrrigationSystem.h"

#define IRRIGATE_TIME 5000
#define SLEEP_TIME 60000


IrrigationSystem::IrrigationSystem(ServoMotorImpl* servo){
  this->servo = servo;
  this->degree = 0;
  this->speed = 0;
  this->increment = 1;
  this->servo->on();
  this->servo->setPosition(this->degree);
  delay(100);
  this->servo->off();
  //comment :
  //this->servo->on();
}

void IrrigationSystem::update(){
  if(!this->servo->isOn()){
    return;
  }
  updatePosition();
  this->servo->setPosition(this->degree);
  delay(10*(MAXSPEED - this->speed+1));
}

void IrrigationSystem::updatePosition(){
  if((this->increment == 1 && this->degree>=180) || (this->increment == -1 and this->degree<=0)){
    this->increment *= -1;
  } 
  this->degree += this->increment;
}

void IrrigationSystem::setSpeed(int newSpeed){
  if(newSpeed<MINSPEED || newSpeed>MAXSPEED){return;}
  this->speed = newSpeed;
}

void IrrigationSystem::increaseSpeed(){
  int newSpeed = this->speed + 1;
  setSpeed(newSpeed);
};
void IrrigationSystem::decreaseSpeed(){
  int newSpeed = this->speed - 1;
  setSpeed(newSpeed);

};

bool IrrigationSystem::isOn(){
  return this->servo->isOn();
}

void IrrigationSystem::onOff(){
  if(isOn()){
    this->servo->off();
  } else {
    this->servo->on();
  }
}

int IrrigationSystem::getSpeed(){
  return this->speed;
}
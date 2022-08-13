#include "ServoMotorImpl.h"
#include <Arduino.h>

ServoMotorImpl::ServoMotorImpl(int pin){
  this -> pin = pin;  
  this->servo = new ServoTimer2();
  this->state = State::OFF;
  this->degree = 0;
  this->speed = 0;
  this->increment = 1;
  on();
  setPosition(this->degree);
  off();
} 

void ServoMotorImpl::on(){
  this->state = State::ON;
  this->servo->attach(pin);    
}

void ServoMotorImpl::setPosition(int angle){
  if(this->isOn()){
    int ms = map(angle,0,180,this->minPulseWidth,this->maxPulseWidth);
    this->servo->write(ms);
  }
}

void ServoMotorImpl::off(){
  this->servo->detach();    
  this->state = State::OFF;

}

bool ServoMotorImpl::isOn(){
  return this->state == State::ON;
}

void ServoMotorImpl::update(){
  if(!isOn()){
    return;
  }
  updatePosition();
  setPosition(this->degree);
  delay(10*this->speed);
}

void ServoMotorImpl::updatePosition(){
  if((this->increment == 1 && this->degree>=180) || (this->increment == -1 and this->degree<=0)){
    this->increment *= -1;
  } 
  this->degree += this->increment;
  Serial.println(this->degree);
}

void ServoMotorImpl::setSpeed(int newSpeed){
  if(newSpeed<0 || newSpeed>4){return;}
  this->speed = newSpeed;
}

void ServoMotorImpl::increaseSpeed(){
  int newSpeed = this->speed + 1;
  setSpeed(newSpeed);
};
void ServoMotorImpl::decreaseSpeed(){
  int newSpeed = this->speed - 1;
  setSpeed(newSpeed);

};
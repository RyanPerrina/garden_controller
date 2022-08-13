#include "ServoMotorImpl.h"
#include <Arduino.h>

ServoMotorImpl::ServoMotorImpl(int pin){
  this -> pin = pin;  
  this->servo = new ServoTimer2();
  this->state = State::OFF;
  
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




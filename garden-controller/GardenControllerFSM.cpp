#include "GardenControllerFSM.h"
#include <Arduino.h>
#include "async_fsm.h"

GardenControllerFSM :: GardenControllerFSM(Led *l1, Led *l2, AnalogLed *l3, AnalogLed *l4, ServoMotorImpl *servo){
  this -> l1 = l1;
  this -> l2 = l2;
  this -> l3 = l3;
  this -> l4 = l4;
  this -> servo = servo;
  this->state = State::AUTO;
}

void GardenControllerFSM::checkEvents(){
  AsyncFSM::checkEvents();
}

void GardenControllerFSM :: handleEvent(Event *e){

  switch(this->state){
    case State::AUTO:
      Serial.println("ciao");
      break;

    case State::MANUAL:
      break;

    case State::ALARM:
      break;
  }
}

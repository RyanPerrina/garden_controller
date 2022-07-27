#include "GardenControllerFSM.h"
#include <Arduino.h>

GardenControllerFSM :: GardenControllerFSM(Led *l1, Led *l2, AnalogLed *l3, AnalogLed *l4, ServoMotorImpl *servo){
  this -> l1 = l1;
  this -> l2 = l2;
  this -> l3 = l3;
  this -> l4 = l4;
  this -> servo = servo;
}

void GardenControllerFSM :: handleEvent(Event *e){
  switch(state){
    case AUTO:
      break;

    case MANUAL:
      break;

    case ALARM:
      break;
  }
}

#include "GardenControllerFSM.h"
#include <Arduino.h>

GardenControllerFSM :: GardenControllerFSM(Led *l1, Led *l2, AnalogLed *l3, AnalogLed *l4, IrrigationSystem *irr){
  this -> l1 = l1;
  this -> l2 = l2;
  this -> l3 = l3;
  this -> l4 = l4;
  this -> irr = irr;
}

void GardenControllerFSM :: handleEvent(Event *e){
  switch(state){
    case AUTO:
      if (e -> getType() == SWITCH_TO_ALARM){
        this -> state = ALARM;
      }
      if (e -> getType() == SWITCH_TO_MANUAL){
        this -> state = MANUAL;
      }
      break;

    case MANUAL:
      if (e -> getType() == SWITCH_TO_AUTO){
        this -> state = AUTO;
      }
      
      if (e -> getType() == LED_1){
        l1 -> toggle();
      }
      if (e -> getType() == LED_2){
        l2 -> toggle();
      }
      break;

    case ALARM:
      if (e -> getType() == DEACTIVATE_ALARM){
        this -> state = AUTO;
      }
      break;
  }
}

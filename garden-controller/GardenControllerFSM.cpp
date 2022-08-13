#include "GardenControllerFSM.h"
#include <Arduino.h>
#include "async_fsm.h"
#include "Events.h"
#include "MsgServiceBTES.h"

GardenControllerFSM ::GardenControllerFSM(Led *l1, Led *l2, AnalogLed *l3, AnalogLed *l4, ServoMotorImpl *servo)
{
  this->l1 = l1;
  this->l2 = l2;
  this->l3 = l3;
  this->l4 = l4;
  this->irrigationSystem = new IrrigationSystem(servo);
  this->state = State::AUTO;
}

void GardenControllerFSM::checkEvents()
{
  AsyncFSM::checkEvents();
}

void GardenControllerFSM::execRuotine(){
  switch (this->state){
    case State::AUTO:
      this->irrigationSystem->update();
      break;
    case State::MANUAL:
      this->irrigationSystem->update();
      break;
    case State::ALARM:
      break;
  }
}

void GardenControllerFSM ::handleEvent(Event *e)
{

  switch (this->state)
  {
  case State::AUTO:
    // if(e->getType() == 1){
    //   Serial.println("messaggio ricevuto da seriale");
    // } else if(e->getType() == 2){
    //   Serial.println("messaggio bluetooth ricevuto");
    // } else
    switch (e->getType())
    {
    case MANUALMODEREQUESTEVENT:
      MsgServiceBt.sendMsg(Msg("MANUAL MODE ON"));
      break;
    case ACKMANUAMODEOKEVENT:
      this->state = State::MANUAL;
      break;
    }
    break;

  case State::MANUAL:
    switch(e->getType()){
        case LED1ONOFFEVENT : 
          if(this->l1->isLedOn()){
            this->l1->switchOff();
          } else {
            this->l1->switchOn();
          }
        break;
    }
    break;

  case State::ALARM:
    break;
  }
}

#include "GardenControllerFSM.h"
#include <Arduino.h>
#include "async_fsm.h"
#include "Events.h"
#include "MsgServiceBTES.h"
#include <string.h>


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
    
    switch (e->getType())
    {
    case MANUALMODEREQUESTEVENT:
      MsgServiceBt.sendMsg(Msg("MANUAL MODE ON"));
      break;
    case ACKMANUAMODEOKEVENT:
      this->state = State::MANUAL;
      break;
    case CONTROLEVENTAUTO:
      ControlEventAuto* eventAuto = (ControlEventAuto*) e;
      String msg = eventAuto->getMsg();
      if(msg.indexOf("LED1ON LED2ON") != -1){
        this->l1->switchOn();
        this->l2->switchOn();
      } else {
        this->l1->switchOff();
        this->l2->switchOff();
      }
      int index = msg.indexOf("LED34:");
      if( index != -1){
        int intensity = msg.substring(index+String("LED34:").length(),index+String("LED34:").length()+1).toInt();
        this->l3->setIntensity(intensity);
        this->l4->setIntensity(intensity);
      }
      //aggiungere parte con irrigazione e parte con allarme.
      //timer che scatta dopo tot secondi che ferma l irrigazione e lo manda in stop.
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
        case LED2ONOFFEVENT : 
          if(this->l2->isLedOn()){
            this->l2->switchOff();
          } else {
            this->l2->switchOn();
          }
        break;
      case IRRIGATIONONOFFEVENT : 
          this->irrigationSystem->onOff();
        break;
      case IRRIGATIONSPEEDUPEVENT : 
          this->irrigationSystem->increaseSpeed();
          //mettere ack?
          MsgServiceBt.sendMsg(Msg("IRRIGATION SPEED: " + String(this->irrigationSystem->getSpeed())));
          break;
      case IRRIGATIONSPEEDDOWNEVENT:
          this->irrigationSystem->decreaseSpeed();
          MsgServiceBt.sendMsg(Msg("IRRIGATION SPEED: " + String(this->irrigationSystem->getSpeed())));
          break;
      case LED3INTENSITYUPEVENT:
          this->l3->increaseIntensity();
          MsgServiceBt.sendMsg(Msg("LED3 INTENSITY: " + String(this->l3->getIntensity())));
          break;
      case LED3INTENSITYDOWNEVENT:
          this->l3->decreaseIntensity();
          MsgServiceBt.sendMsg(Msg("LED3 INTENSITY: " + String(this->l3->getIntensity())));
          break;
      case LED4INTENSITYUPEVENT:
          this->l4->increaseIntensity();
          MsgServiceBt.sendMsg(Msg("LED4 INTENSITY: " + String(this->l4->getIntensity())));
          break;
      case LED4INTENSITYDOWNEVENT:
          this->l4->decreaseIntensity();
          MsgServiceBt.sendMsg(Msg("LED4 INTENSITY: " + String(this->l4->getIntensity())));
          break;
    }
    break;

  case State::ALARM:
    break;
  }
}

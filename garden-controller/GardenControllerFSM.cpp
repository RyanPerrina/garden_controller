#include "GardenControllerFSM.h"
#include <Arduino.h>
#include "async_fsm.h"
#include "Events.h"
#include "MsgServiceBTES.h"
#include <string.h>
#include "TimerOne.h"

#define ALARMTIMERPERIOD 5
volatile int alarmCount = 0;
boolean timerFlag = false;
void alarmTimer(){
  alarmCount += 1;
  if( alarmCount >= ALARMTIMERPERIOD){
    timerFlag = true;
    alarmCount = 0;
  }
}

GardenControllerFSM ::GardenControllerFSM(Led *l1, Led *l2, AnalogLed *l3, AnalogLed *l4, ServoMotorImpl *servo)
{
  this->l1 = l1;
  this->l2 = l2;
  this->l3 = l3;
  this->l4 = l4;
  this->irrigationSystem = new IrrigationSystem(servo);
  this->state = State::AUTO;

  // this->state = State::ALARM;
  // //togliere:
  //         Timer1.initialize();
  //       Timer1.attachInterrupt(alarmTimer);
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
      if(timerFlag){
        timerFlag = false;
        MsgServiceEs.sendMsg(Msg("ALARMON"));
      }
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
      MsgService.sendMsg(Msg("MANUALMODEON"));
      break;
    case CONTROLEVENTAUTO:
      ControlEventAuto* eventAuto = (ControlEventAuto*) e;
      String msg = eventAuto->getMsg();
      Serial.println(msg);
      if(msg==""){
        break;
      }
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
      } else {
        this->l3->setIntensity(0);
        this->l4->setIntensity(0);
      }

      //testare da qui
      index = msg.indexOf("IRRIGATIONON");
      if( index != -1 && !this->irrigationSystem->isInPause() && !this->irrigationSystem->isOn() ){
        this->irrigationSystem->on();
      } 

      index = msg.indexOf("IRRIGATIONOFF");
      if(index != -1){
        this->irrigationSystem->off();
      }

      index = msg.indexOf("SPEED:");
      if(index!=-1){
         int speed = msg.substring(index+String("SPEED:").length(),index+String("SPEED:").length()+1).toInt();
         this->irrigationSystem->setSpeed(speed);
      }

      index = msg.indexOf("TEMPERATURECHECK");
      if(index != -1  && this->irrigationSystem->isInPause() ){
        this->state = State::ALARM;
        this->irrigationSystem->off();
        Timer1.initialize();
        Timer1.attachInterrupt(alarmTimer);
      }
      break;
    }
    break;

  case State::MANUAL:
    switch(e->getType()){
        case DISABLEMANUALMODEEVENT:
          this->state = State::AUTO;
          MsgServiceBt.sendMsg(Msg("AUTOMODEENABLED"));
          MsgService.sendMsg(Msg("MANUALMODEOFF"));
          break;
        case LED1ONOFFEVENT : 
          if(this->l1->isLedOn()){
            this->l1->switchOff();
          } else {
            this->l1->switchOn();
          }
          MsgService.sendMsg(Msg("L1"));
          break;
        case LED2ONOFFEVENT : 
          if(this->l2->isLedOn()){
            this->l2->switchOff();
          } else {
            this->l2->switchOn();
          }
          MsgService.sendMsg(Msg("L2"));
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
          MsgService.sendMsg(Msg("L3 UP"));
          break;
      case LED3INTENSITYDOWNEVENT:
          this->l3->decreaseIntensity();
          MsgServiceBt.sendMsg(Msg("LED3 INTENSITY: " + String(this->l3->getIntensity())));
          MsgService.sendMsg(Msg("L3 DOWN"));
          break;
      case LED4INTENSITYUPEVENT:
          this->l4->increaseIntensity();
          MsgServiceBt.sendMsg(Msg("LED4 INTENSITY: " + String(this->l4->getIntensity())));
          MsgService.sendMsg(Msg("L4 UP"));
          break;
      case LED4INTENSITYDOWNEVENT:
          this->l4->decreaseIntensity();
          MsgServiceBt.sendMsg(Msg("LED4 INTENSITY: " + String(this->l4->getIntensity())));
          MsgService.sendMsg(Msg("L4 DOWN"));
          break;
    }
    break;

  case State::ALARM:
    switch(e->getType()){
          case MANUALMODEREQUESTEVENT:
          MsgServiceBt.sendMsg(Msg("ALARM"));
          break;
          case DISABLEALARMEVENT:
          this->state = State::AUTO;
          MsgServiceBt.sendMsg(Msg("ALARM DISABLED"));
          MsgServiceEs.sendMsg(Msg("ALARMOFF"));
          Timer1.stop();
          break;
    }
    break;
  }
}

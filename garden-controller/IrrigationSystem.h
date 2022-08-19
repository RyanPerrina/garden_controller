#ifndef __IRRIGATION_SYSTEM__
#define __IRRIGATION_SYSTEM__

#include <Arduino.h>
#include <TimerOne.h>
#include "ServoMotorImpl.h"
#include "async_fsm.h"
#define MAXSPEED 4
#define MINSPEED 0

class IrrigationSystem {

public:
  IrrigationSystem(ServoMotorImpl* servo);
  void update();
  void setSpeed(int newSpeed);
  void increaseSpeed();
  void decreaseSpeed();
  bool isOn();
  void onOff();
  int getSpeed();
  bool isInPause();

 
private:
  int speed;
  int increment;
  int degree;
  ServoMotorImpl* servo;
  void updatePosition();
  enum State {ON,OFF,PAUSE} state;
};

#endif

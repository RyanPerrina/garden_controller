#ifndef __IRRIGATION_SYSTEM__
#define __IRRIGATION_SYSTEM__

#include <Arduino.h>
#include <TimerOne.h>
#include "ServoMotorImpl.h"
#include "async_fsm.h"

class IrrigationSystem {

public:
  IrrigationSystem(ServoMotorImpl* servo);
  void update();
  void setSpeed(int newSpeed);
  void increaseSpeed();
  void decreaseSpeed();

 
private:
  int speed;
  int increment;
  int degree;
  ServoMotorImpl* servo;
  void updatePosition();
};

#endif

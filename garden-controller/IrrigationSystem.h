#ifndef __IRRIGATION_SYSTEM__
#define __IRRIGATION_SYSTEM__

#include <Arduino.h>
#include <TimerOne.h>
#include "ServoMotorImpl.h"
#include "async_fsm.h"

class IrrigationSystem : public ServoMotorImpl {

public:
  IrrigationSystem(int pin);
  void setSpeed(int value);
  void irrigate();

private:
  int speed;
  int pos;
  int direction;
};

#endif

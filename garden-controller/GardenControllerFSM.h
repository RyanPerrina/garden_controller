#ifndef __GARDEN_CONTROLLER_FSM__
#define __GARDEN_CONTROLLER_FSM__

#include "async_fsm.h"
#include "Led.h"
#include "AnalogLed.h"
#include "ServoMotorImpl.h"
#include "IrrigationSystem.h"

class GardenControllerFSM : public AsyncFSM
{

public:
  GardenControllerFSM(Led *l1, Led *l2, AnalogLed *l3, AnalogLed *l4, ServoMotorImpl *servo);
  void handleEvent(Event *e);
  void checkEvents();
  void execRuotine();

private:
  Led *l1;
  Led *l2;
  AnalogLed *l3;
  AnalogLed *l4;

  IrrigationSystem *irrigationSystem;

  enum class State
  {
    AUTO,
    MANUAL,
    ALARM
  } state;
};

#endif

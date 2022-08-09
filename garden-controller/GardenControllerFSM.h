#ifndef __GARDEN_CONTROLLER_FSM__
#define __GARDEN_CONTROLLER_FSM__

#include "async_fsm.h"
#include "Led.h"
#include "AnalogLed.h"
#include "IrrigationSystem.h"
#include "MsgService.h"

enum EventType { NO_EVENT, LED_1, LED_2, LED_3, LED_4, IRRIGATE, SWITCH_TO_AUTO, SWITCH_TO_MANUAL, SWITCH_TO_ALARM, DEACTIVATE_ALARM };

class GardenControllerFSM : AsyncFSM{

public:
  GardenControllerFSM(Led *l1, Led *l2, AnalogLed *l3, AnalogLed *l4, IrrigationSystem *irr);
  void handleEvent(Event *e);

private:
  Led *l1;
  Led *l2;
  AnalogLed *l3;
  AnalogLed *l4;
  IrrigationSystem *irr;
  enum { AUTO, MANUAL, ALARM } state;
};


#endif

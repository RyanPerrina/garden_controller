// code to be run on Arduino
#include "Led.h"
#include "AnalogLed.h"
#include "ServoMotorImpl.h"
#include "GardenControllerFSM.h"

#include "MSGServiceES.h"
#include "MsgServiceBTES.h"

#define PIN_LED1 13
#define PIN_LED2 12
#define PIN_LED3 5
#define PIN_LED4 6
#define PIN_SERVO 3
GardenControllerFSM *fsm;


MsgServiceBTES MsgServiceBt(7,8);

void setup() {
  Led *l1 = new Led(PIN_LED1);
  Led * l2 = new Led(PIN_LED2);
  AnalogLed * l3 = new AnalogLed(PIN_LED3);
  AnalogLed *l4 = new AnalogLed(PIN_LED4);
  ServoMotorImpl *servo = new ServoMotorImpl(PIN_SERVO);
  servo -> on();
  servo -> setPosition(0);
  delay(2000);
  servo -> off();
  fsm = new GardenControllerFSM(l1, l2, l3, l4, servo);
  MsgServiceEs.init(fsm);
  MsgServiceBt.init(fsm);
}

void loop() {
  
  MsgServiceEs.checkMSG();
  MsgServiceBt.checkMSG();
  fsm->checkEvents();
  fsm->execRuotine();
  //TODO:metodo tipo fsm->executestuff() e lui fa cose che deve fare periodicamente come aggiornare sistema di irrigazione fino a che evento sopracciunge? o meglio task
  // MsgServiceBt.sendMsg(Msg("MANUAL MODE ON"));

  }

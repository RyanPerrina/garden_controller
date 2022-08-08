// code to be run on Arduino
#include "Led.h"
#include "AnalogLed.h"
#include "ServoMotorImpl.h"
#include "GardenControllerFSM.h"
#include "IrrigationSystem.h"
#include "MsgService.h"

#define PIN_LED1 13
#define PIN_LED2 12
#define PIN_LED3 11
#define PIN_LED4 10
#define PIN_SERVO 2

ServoMotorImpl *servo;
Led *l1;
Led *l2;
AnalogLed *l3;
AnalogLed *l4;
GardenControllerFSM *fsm;
IrrigationSystem *irr;
enum { NO_EVENT, LED_1, LED_2, LED_3, LED_4, IRRIGATE } event;

void setup() {
  Serial.begin(9600);
  l1 = new Led(PIN_LED1);
  l2 = new Led(PIN_LED2);
  l3 = new AnalogLed(PIN_LED3);
  l4 = new AnalogLed(PIN_LED4);
  servo = new ServoMotorImpl(PIN_SERVO);
  MsgService.init();

  fsm = new GardenControllerFSM(l1, l2, l3, l4, servo);
  irr = new IrrigationSystem(PIN_SERVO);
  event = NO_EVENT;
}

void loop() {
  // put your main code here, to run repeatedly:
  if (MsgService.isMsgAvailable()){
    Msg *msg = MsgService.receiveMsg();

    // controlla evento
    //if (msg -> getContent() == ""){
    //  event = ...
    //}
  }

  Event *e = new Event(event);
  fsm -> handleEvent(e);
}

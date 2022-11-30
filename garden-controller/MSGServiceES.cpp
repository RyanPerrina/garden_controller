#include "MSGServiceES.h"
#include "Arduino.h"
#include "async_fsm.h"
#include "MsgService.h"
#include "Events.h"

// String content;
MsgServiceClassES MsgServiceEs;
ControlEventAuto events[MAX_EVQUEUE_SIZE];
ControlEventAuto event;
int head = 0;

void MsgServiceClassES::init(Observer* observer){
    MsgServiceClass::init();
    EventSource::registerObserver(observer);
}
void MsgServiceClassES::checkMSG(){
    if(isMsgAvailable()){
        Msg* m = MsgServiceClass::receiveMsg();
        String msg = m->getContent();
        delete m;
        event = ControlEventAuto(this,msg);
        events[head] = event;
        head = (head+1) %MAX_EVQUEUE_SIZE;
        generateEvent(&event);
    } 

}

void MsgServiceClassES::notifyInterrupt(int pin){

}

void serialEvent() {
  /* reading the content */
  while (Serial.available()) {
    char ch = (char) Serial.read();
    if (ch == '\n'){
      MsgServiceEs.currentMsg = new Msg(content);
      MsgServiceEs.msgAvailable = true;      
    } else {
      content += ch;      
    }
  }
}
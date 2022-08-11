#include "MSGServiceES.h"
#include "Arduino.h"
#include "async_fsm.h"
#include "MsgService.h"
#include "Events.h"

// String content;
MsgServiceClassES MsgServiceEs;

void MsgServiceClassES::init(Observer* observer){
    MsgServiceClass::init();
    EventSource::registerObserver(observer);
}
void MsgServiceClassES::checkMSG(){
    if(isMsgAvailable()){
        EventSource::generateEvent(new ControlEventAuto(this));
        receiveMsg();
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
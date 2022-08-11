#include "MsgServiceBTES.h"
#include "events.h"

MsgServiceBTES::MsgServiceBTES(int rxPin,int txPin):MsgServiceBT(rxPin,txPin){
}

void MsgServiceBTES::init(Observer* observer){
    MsgServiceBT::init();
    EventSource::registerObserver(observer);
}

void MsgServiceBTES::checkMSG(){
    if(MsgServiceBT::isMsgAvailable()){
        EventSource::generateEvent(new ControlEventManual(this));
        MsgServiceBT::receiveMsg();
    }
}

void MsgServiceBTES::notifyInterrupt(int pin){

}
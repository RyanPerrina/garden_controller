#include "MsgServiceBTES.h"
#include "events.h"

enum Options{
    LED1ONOFF,
    MANUALMODEREQ,
    ACKMANUALMODEON
};

Options resolveOption(String str){
    if(str == "LED1: ON/OFF"){ return Options::LED1ONOFF;}
    if(str == "MANUAL MODE REQUEST"){ return Options::MANUALMODEREQ;}
    if(str == "ACK MANUAL MODE ON"){ return Options::ACKMANUALMODEON;}

}

MsgServiceBTES::MsgServiceBTES(int rxPin,int txPin):MsgServiceBT(rxPin,txPin){
}

void MsgServiceBTES::init(Observer* observer){
    MsgServiceBT::init();
    EventSource::registerObserver(observer);
}

void MsgServiceBTES::checkMSG(){
    if(MsgServiceBT::isMsgAvailable()){
        String msg = MsgServiceBT::receiveMsg()->getContent();
        Serial.println(msg);
        switch(resolveOption(msg)) {
            case Options::LED1ONOFF:
                generateEvent(new Event(LED1ONOFFEVENT));
                break;
            case Options::MANUALMODEREQ :
                generateEvent(new Event(MANUALMODEREQUESTEVENT));
                break;
            case Options::ACKMANUALMODEON : 
                generateEvent(new Event(ACKMANUAMODEOKEVENT));
                Serial.println("ok");
                break;
        }
        //EventSource::generateEvent(new ControlEventManual(this));
    }
}

void MsgServiceBTES::notifyInterrupt(int pin){

}

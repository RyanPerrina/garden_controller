#include "MsgServiceBTES.h"
#include "events.h"

enum Options{
    LED1ONOFF,
    MANUALMODEREQ,
    ACKMANUALMODEON,
    LED2ONOFF,
    IRRIGATIONONOFF,
    IRRIGATIONSPEEDUP,
    IRRIGATIONSPEEDDOWN
};

Options resolveOption(String str){
    if(str == "LED1: ON/OFF"){ return Options::LED1ONOFF;}
    if(str == "MANUAL MODE REQUEST"){ return Options::MANUALMODEREQ;}
    if(str == "ACK MANUAL MODE ON"){ return Options::ACKMANUALMODEON;}
    if(str == "LED2: ON/OFF"){ return Options::LED2ONOFF;}
    if(str == "IRRIGATION: ON/OFF"){ return Options::IRRIGATIONONOFF;}
    if(str == "IRRIGATION: SPEED UP"){ return Options::IRRIGATIONSPEEDUP;}
    if(str == "IRRIGATION: SPEED DOWN"){ return Options::IRRIGATIONSPEEDDOWN;}
    

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
                break;
            case Options::LED2ONOFF:
                generateEvent(new Event(LED2ONOFFEVENT));
                break;
            case Options::IRRIGATIONONOFF:
                generateEvent(new Event(IRRIGATIONONOFFEVENT));
                break;
            case Options::IRRIGATIONSPEEDUP:
                generateEvent(new Event(IRRIGATIONSPEEDUPEVENT));
                break;
            case Options::IRRIGATIONSPEEDDOWN:
                generateEvent(new Event(IRRIGATIONSPEEDDOWNEVENT));
                break;

        }
        //EventSource::generateEvent(new ControlEventManual(this));
    }
}

void MsgServiceBTES::notifyInterrupt(int pin){

}

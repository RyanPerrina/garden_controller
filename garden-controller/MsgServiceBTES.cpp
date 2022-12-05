#include "MsgServiceBTES.h"
#include "events.h"

enum Options{
    LED1ONOFF,
    MANUALMODEREQ,
    ACKMANUALMODEON,
    LED2ONOFF,
    IRRIGATIONONOFF,
    IRRIGATIONSPEEDUP,
    IRRIGATIONSPEEDDOWN,
    LED3INTENSITYDOWN,
    LED3INTENSITYUP,
    LED4INTENSITYDOWN,
    LED4INTENSITYUP,
    DISABLEMANUALMODE,
    DISABLEALARM
};

Options resolveOption(String str){
    if(str == "LED1: ON/OFF"){ return Options::LED1ONOFF;}
    if(str == "MANUAL MODE REQUEST"){ return Options::MANUALMODEREQ;}
    if(str == "ACK MANUAL MODE ON"){ return Options::ACKMANUALMODEON;}
    if(str == "LED2: ON/OFF"){ return Options::LED2ONOFF;}
    if(str == "IRRIGATION: ON/OFF"){ return Options::IRRIGATIONONOFF;}
    if(str == "IRRIGATION: SPEED UP"){ return Options::IRRIGATIONSPEEDUP;}
    if(str == "IRRIGATION: SPEED DOWN"){ return Options::IRRIGATIONSPEEDDOWN;}
    if(str == "LED3: INTENSITY UP"){ return Options::LED3INTENSITYUP;}
    if(str == "LED3: INTENSITY DOWN"){ return Options::LED3INTENSITYDOWN;}
    if(str == "LED4: INTENSITY UP"){ return Options::LED4INTENSITYUP;}
    if(str == "LED4: INTENSITY DOWN"){ return Options::LED4INTENSITYDOWN;}
    if(str == "DISABLEMANUALMODE"){ return Options::DISABLEMANUALMODE;}
    if(str == "DISABLEALARM"){ return Options::DISABLEALARM;}


}

MsgServiceBTES::MsgServiceBTES(int rxPin,int txPin):MsgServiceBT(rxPin,txPin){
}

void MsgServiceBTES::init(Observer* observer){
    MsgServiceBT::init();
    EventSource::registerObserver(observer);
}

void MsgServiceBTES::checkMSG(){
    if(MsgServiceBT::isMsgAvailable()){
        Msg * m = MsgServiceBT::receiveMsg();
        String msg = m->getContent();
        delete m;
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
            case Options::LED3INTENSITYUP:
                generateEvent(new Event(LED3INTENSITYUPEVENT));
                break;
            case Options::LED3INTENSITYDOWN:
                generateEvent(new Event(LED3INTENSITYDOWNEVENT));
                break;
            case Options::LED4INTENSITYUP:
                generateEvent(new Event(LED4INTENSITYUPEVENT));
                break;
            case Options::LED4INTENSITYDOWN:
                generateEvent(new Event(LED4INTENSITYDOWNEVENT));
                break;
            case Options::DISABLEMANUALMODE:
                generateEvent(new Event(DISABLEMANUALMODEEVENT));
                break;
            case Options::DISABLEALARM:
                generateEvent(new Event(DISABLEALARMEVENT));
                break;
        }
    }
}

void MsgServiceBTES::notifyInterrupt(int pin){

}

#ifndef __EVENTS__
#define __EVENTS__
#include "async_fsm.h"
#include "MSGServiceES.h"
#include "MsgServiceBTES.h"
#define CONTROLEVENTAUTO 1
#define MANUALMODEREQUESTEVENT 3
#define ACKMANUAMODEOKEVENT 4
#define LED1ONOFFEVENT 5
#define LED2ONOFFEVENT 6
#define IRRIGATIONONOFFEVENT 7
#define IRRIGATIONSPEEDUPEVENT 8
#define IRRIGATIONSPEEDDOWNEVENT 9
#define LED3INTENSITYUPEVENT 10
#define LED3INTENSITYDOWNEVENT 11
#define LED4INTENSITYUPEVENT 12
#define LED4INTENSITYDOWNEVENT 13
#define DISABLEMANUALMODEEVENT 14
#define DISABLEALARMEVENT 15

class ControlEventAuto : public Event {

    public:
        ControlEventAuto(MsgServiceClassES* source,String msg);

        MsgServiceClassES* getSource();
        String getMsg();
    
    private:
        MsgServiceClassES* source;
        String msg;

};


class ControlEventManual : public Event {

    public:
        ControlEventManual(MsgServiceBTES* source);

        MsgServiceBTES* getSource();
    
    private:
        MsgServiceBTES* source;

};


// class AllarmEvent : public Event {

// };
#endif
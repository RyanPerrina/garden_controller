#ifndef __EVENTS__
#define __EVENTS__
#include "async_fsm.h"
#include "MSGServiceES.h"
#include "MsgServiceBTES.h"
#define MANUALMODEREQUESTEVENT 3
#define ACKMANUAMODEOKEVENT 4
#define LED1ONOFFEVENT 5
#define LED2ONOFFEVENT 6
#define IRRIGATIONONOFFEVENT 7
#define IRRIGATIONSPEEDUPEVENT 8
#define IRRIGATIONSPEEDDOWNEVENT 9

class ControlEventAuto : public Event {

    public:
        ControlEventAuto(MsgServiceClassES* source);

        MsgServiceClassES* getSource();
    
    private:
        MsgServiceClassES* source;

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
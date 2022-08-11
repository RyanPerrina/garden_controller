#ifndef __EVENTS__
#define __EVENTS__
#include "async_fsm.h"
#include "MSGServiceES.h"
#include "MsgServiceBTES.h"

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
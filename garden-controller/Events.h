#ifndef __EVENTS__
#define __EVENTS__
#include "async_fsm.h"
#include "MSGServiceES.h"

class ControlEventAuto : public Event {

    public:
        ControlEventAuto(MsgServiceClassES* source);

        MsgServiceClassES* getSource();
    
    private:
        MsgServiceClassES* source;

};

// class AllarmEvent : public Event {

// };
#endif
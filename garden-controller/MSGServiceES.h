#ifndef __MSGSERVICEES__
#define __MSGSERVICEES__
#include "async_fsm.h"
#include "MsgService.h"

class MsgServiceClassES: public EventSource, public MsgServiceClass  {
    public:
        void init(Observer* observer);
        void checkMSG();
        void notifyInterrupt(int pin);
};

extern MsgServiceClassES MsgServiceEs;
extern String content;
#endif
#ifndef __MSGSERVICEBTES__
#define __MSGSERVICEBTES__
#import "MsgServiceBT.h"
#import "async_fsm.h"

class MsgServiceBTES : public EventSource, public MsgServiceBT{
    public:
        MsgServiceBTES(int rxPin, int txPin);  
        void init(Observer* observer);
        void checkMSG();
        void notifyInterrupt(int pin);
};


#endif

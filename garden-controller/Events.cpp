#include "Events.h"

ControlEventAuto::ControlEventAuto(MsgServiceClassES *source) : Event(1)
{
    this->source = source;
}

MsgServiceClassES *ControlEventAuto::getSource()
{
    return this->source;
}

ControlEventManual::ControlEventManual(MsgServiceBTES *source) :  Event(2){
    this->source = source;
}

MsgServiceBTES *ControlEventManual::getSource(){
    return this->source;

}

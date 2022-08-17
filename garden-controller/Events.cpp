#include "Events.h"

ControlEventAuto::ControlEventAuto(MsgServiceClassES *source,String msg) : Event(CONTROLEVENTAUTO)
{
    this->source = source;
    this->msg = msg;
}

String ControlEventAuto::getMsg(){
    return this->msg;
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

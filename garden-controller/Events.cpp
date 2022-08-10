#include "Events.h"


ControlEventAuto::ControlEventAuto(MsgServiceClassES* source):Event(1){
    this->source = source;
}

MsgServiceClassES* ControlEventAuto::getSource(){
    return this->source;
}
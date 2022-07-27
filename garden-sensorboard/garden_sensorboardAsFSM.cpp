#include "garden_sensorboardAsFSM.h"
#include "Led.h"
#include "Photoresistor.h"
#include "TemperatureSensor.h"

garden_sensorboardAsFSM::garden_sensorboardAsFSM(TemperatureSensor *temp, Led* led, Console* console, Photoresistor *photo){
      currentState = AUTO;
      this->temp = temp;
      this->photo = photo;
      this->led = led;
      this->console = console;
      led->switchOff();
      temp->registerObserver(this);
      photo->registerObserver(this);
}

void garden_sensorboardAsFSM::handleEvent(Event* ev) {
    switch (currentState) {
        case AUTO:  
            if (ev->getType() == BUTTON_PRESSED_EVENT){
                photo->getLight();
                led->switchOn();
                count = count + 1;
                console->log(count);
                currentState = ON;
            }
        break; 
        case MANUAL: 
            if (ev->getType() == BUTTON_RELEASED_EVENT){
                console->log("OFF");
                led->switchOff();
                count = count + 1;
                console->log(count);
                currentState = OFF;
            }
        
        case ALLARM:  
            if (ev->getType() == BUTTON_PRESSED_EVENT){
                console->log("ON");
                led->switchOn();
                count = count + 1;
                console->log(count);
                currentState = ON;
            }
        break; 
    }
}

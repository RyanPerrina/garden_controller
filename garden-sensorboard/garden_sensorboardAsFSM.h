
#include "async_fsm.h"
#include "Led.h"
#include "Photoresistor.h"
#include "TemperatureSensor.h"

class class garden_sensorboardAsFSM : public AsyncFSM {

  private:
    enum  { AUTO, MANUAL, ALLARM} currentState;
    TemperatureSensor *temp;
    Led* led;
    Console* console;
    Photoresistor *photo;

  public:  
    garden_sensorboardAsFSM(TemperatureSensor *temp, Led* led, Console* console, Photoresistor *photo);
    void handleEvent(Event* ev);
   
}; 
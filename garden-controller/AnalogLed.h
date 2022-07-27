#ifndef __ANALOG_LED__
#define __ANALOG_LED__

#include "Light.h"

class AnalogLed : public Light { 
  
public:
  AnalogLed(int pin);
  void switchOn();
  void switchOff();
  void setIntensity(int value);
  
private:
  int pin;  
};

#endif

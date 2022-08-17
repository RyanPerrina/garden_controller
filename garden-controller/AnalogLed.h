#ifndef __ANALOG_LED__
#define __ANALOG_LED__

#include "Light.h"
#define MININTENSITY 0 
#define MAXINTENSITY 4

class AnalogLed : public Light { 
  
public:
  AnalogLed(int pin);
  void switchOn();
  void switchOff();
  void setIntensity(int value);
  bool isOn();
  void increaseIntensity();
  void decreaseIntensity();
  int getIntensity();
  
private:
  int pin;  
  enum State {ON,OFF} state;
  int intensity;
};

#endif

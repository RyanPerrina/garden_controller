#ifndef __PHOTORESISTOR__
#define __PHOTORESISTOR__

#include <Arduino.h>

#define FREQ 5
#define PERIOD 1000/FREQ

class Photoresistor{

public:
  Photoresistor(int pin);
  int getLight();

private:
  int pin;
};

#endif

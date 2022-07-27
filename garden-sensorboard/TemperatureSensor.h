#ifndef __TEMPERATURE_SENSOR__
#define __TEMPERATURE_SENSOR__

class TemperatureSensor{

public:
  TemperatureSensor(int pin);
  float getTemperature();

private:
  int pin;
  
};

#endif

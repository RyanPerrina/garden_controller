#ifndef __SERVO_MOTOR_IMPL__
#define __SERVO_MOTOR_IMPL__

#include "ServoMotor.h"
#include "ServoTimer2.h"
#include <Arduino.h>



class ServoMotorImpl: public ServoMotor {

public:
  ServoMotorImpl(int pin);

  void on();
  void setPosition(int angle);
  void off();
  bool isOn();
 
    
protected:
  int pin; 
  ServoTimer2* servo; 
  const int maxPulseWidth = 2250;
  const int minPulseWidth = 750;
  enum State {ON,OFF} state;
};

#endif

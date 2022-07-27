#include "Photoresistor.h"

Photoresistor :: Photoresistor(int pin){
  this -> pin = pin;
  pinMode(this -> pin, INPUT);
}

int Photoresistor :: getLight(){
  int light = map(analogRead(this -> pin), 0, 1023, 1, 8);
  delay(PERIOD);
  return light;
}

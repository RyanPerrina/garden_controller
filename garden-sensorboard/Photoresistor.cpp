#include "Photoresistor.h"

Photoresistor :: Photoresistor(int pin){
  this -> pin = pin;
  pinMode(this -> pin, INPUT);
}

int Photoresistor :: getLuminosity(){
  int luminosity = map(analogRead(this -> pin), 0, 4096, 1, 8);
  delay(PERIOD);
  return luminosity;
}

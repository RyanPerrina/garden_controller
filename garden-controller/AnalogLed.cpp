#include "AnalogLed.h"
#include "Arduino.h"

AnalogLed ::AnalogLed(int pin)
{
  this->pin = pin;
  pinMode(pin, OUTPUT);
  this->state = State::OFF;
  this->intensity = 0;
}

void AnalogLed ::switchOn()
{
  digitalWrite(pin, HIGH);
  this->state = State::ON;
  this->intensity = MAXINTENSITY;
}

void AnalogLed ::switchOff()
{
  digitalWrite(pin, LOW);
  this->state = State::ON;
  this->intensity = 0;
};

void AnalogLed ::setIntensity(int intensity)
{
  if (intensity < MININTENSITY || intensity > MAXINTENSITY)
  {
    return;
  }
  if (intensity == MININTENSITY)
  {
    this->state = State::OFF;
  } else {
    this->state = State::ON;
  }
  this->intensity = intensity;
  int analogIntensity = map(intensity, MININTENSITY, MAXINTENSITY, 0, 255);
  analogWrite(pin, analogIntensity);
}

bool AnalogLed::isOn(){
  return this->state == State::ON;
};

void AnalogLed::increaseIntensity(){
  if(this->intensity >=4 ){
    return;
  } else {
    this->intensity++;
  }
  setIntensity(this->intensity);
}

void AnalogLed::decreaseIntensity(){
  if(this->intensity < MININTENSITY){
    return;
  } else {
    this->intensity--;
  }
  Serial.println(this->intensity);
  setIntensity(this->intensity);
}

int AnalogLed::getIntensity(){
  return this->intensity;
}
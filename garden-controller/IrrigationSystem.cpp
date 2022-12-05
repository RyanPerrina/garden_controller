#include "IrrigationSystem.h"
#include "TimerOne.h"

#define IRRIGATE_TIME 20
#define SLEEP_TIME 60

volatile bool stopped = false;
volatile int count = 0;

void stopIrrigationRoutine();
void exitPauseRoutine();

IrrigationSystem::IrrigationSystem(ServoMotorImpl *servo)
{
  this->servo = servo;
  this->degree = 0;
  this->speed = 0;
  this->increment = 1;
  this->servo->on();
  this->servo->setPosition(this->degree);
  delay(100);
  this->servo->off();
  this->state = State::OFF;
  // comment :
  // this->servo->on();
}

void IrrigationSystem::update()
{
  switch (this->state)
  {
  case State::OFF:
    return;
    break;
  case State::ON:
    if (stopped)
    {
      this->state = State::PAUSE;
      Timer1.attachInterrupt(exitPauseRoutine);
      Timer1.restart();
      break;
    }
    updatePosition();
    this->servo->setPosition(this->degree);
    delay(10 * (MAXSPEED - this->speed + 1));
    break;
  case State::PAUSE:
    if (!stopped)
    {
      this->state = State::ON;
      Timer1.attachInterrupt(stopIrrigationRoutine);
      Timer1.restart();
    }
    break;
  default:
    break;
  }
}

void IrrigationSystem::updatePosition()
{
  if ((this->increment == 1 && this->degree >= 180) || (this->increment == -1 and this->degree <= 0))
  {
    this->increment *= -1;
  }
  this->degree += this->increment;
}

void IrrigationSystem::setSpeed(int newSpeed)
{
  if (newSpeed < MINSPEED || newSpeed > MAXSPEED)
  {
    return;
  }
  this->speed = newSpeed;
}

void IrrigationSystem::increaseSpeed()
{
  setSpeed(this->speed + 1);
};
void IrrigationSystem::decreaseSpeed()
{
  setSpeed(this->speed - 1);
};

bool IrrigationSystem::isOn()
{
  return this->state == State::ON;
}

bool IrrigationSystem::isInPause()
{
  return this->state == State::PAUSE;
}

void IrrigationSystem::onOff()
{
  if (isOn())
  {
    off();
  }
  else
  {
    on();
  }
}

void IrrigationSystem::off()
{
  this->state = State::OFF;
  this->servo->off();
  Timer1.stop();
  count = 0;
}

void IrrigationSystem::on()
{
  count = 0;
  stopped = false;
  this->state = State::ON;
  this->servo->on();
  Timer1.initialize();
  Timer1.attachInterrupt(stopIrrigationRoutine);
}

int IrrigationSystem::getSpeed()
{
  return this->speed;
}

void stopIrrigationRoutine()
{
  count += 1;
  if (count >= IRRIGATE_TIME)
  {
    count = 0;
    stopped = true;
  }
}

void exitPauseRoutine()
{
  count += 1;
  if (count >= SLEEP_TIME)
  {
    count = 0;
    stopped = false;
  }
}

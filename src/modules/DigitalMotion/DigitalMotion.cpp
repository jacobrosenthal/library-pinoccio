
#include <Arduino.h>
#include <Scout.h>
#include "modules/DigitalMotion/DigitalMotion.h"
#include "key/key.h"

using namespace pinoccio;

DigitalMotionModule DigitalMotionModule::instance;

const __FlashStringHelper *DigitalMotionModule::name() const {
  return F("dmotion");
}

static void readTimerHandler(SYS_Timer_t *timer) {
  DigitalMotionModule* m = &(DigitalMotionModule::instance);
  m->read();
}

static void timeoutTimerHandler(SYS_Timer_t *timer) {
  DigitalMotionModule* m = &(DigitalMotionModule::instance);
  m->timeout();
}

static numvar dmotionStatus(void) {
  DigitalMotionModule* m = &(DigitalMotionModule::instance);
  return m->getStatus();
}

static numvar dmotionRead(void) {
  DigitalMotionModule* m = &(DigitalMotionModule::instance);
  m->read();
  return 1;
}

static numvar dmotionStop(void) {
  DigitalMotionModule* m = &(DigitalMotionModule::instance);
  m->stop();
  return 1;
}

static numvar dmotionStart(void) {
  if (!checkArgs(4, F("usage: dmotion.start(readIntervalMs, timeoutIntervalMs, \"triggerFunction\", \"timeoutFunction\")"))) {
    return 0;
  }

  const char *triggerFunction = NULL;
  if (isstringarg(3))
    triggerFunction = (const char*)getarg(3);
  else
    triggerFunction = keyGet(getarg(3));

  const char *timeoutFunction = NULL;
  if (isstringarg(4))
    timeoutFunction = (const char*)getarg(4);
  else
    timeoutFunction = keyGet(getarg(4));

  if (triggerFunction && !Shell.defined(triggerFunction)) {
    sp("triggerFunction Must be the name of function: ");
    speol(triggerFunction);
    return 0;
  }

  if (timeoutFunction && !Shell.defined(timeoutFunction)) {
    sp("timeoutFunction Must be the name of function: ");
    speol(timeoutFunction);
    return 0;
  }

  DigitalMotionModule* m = &(DigitalMotionModule::instance);
  m->start(getarg(1), getarg(2), triggerFunction, timeoutFunction);
  return 1;
}

void DigitalMotionModule::loop() {

}

bool DigitalMotionModule::enable() {

  readTimer.mode = SYS_TIMER_PERIODIC_MODE;
  readTimer.handler = readTimerHandler;

  timeoutTimer.mode = SYS_TIMER_INTERVAL_MODE;
  timeoutTimer.handler = timeoutTimerHandler;

  Shell.addFunction("dmotion.read", dmotionRead);
  Shell.addFunction("dmotion.start", dmotionStart);
  Shell.addFunction("dmotion.stop", dmotionStop);
  Shell.addFunction("dmotion.status", dmotionStatus);

}

void DigitalMotionModule::start(uint32_t readInterval, uint32_t timeoutInterval, const char* mfunc, const char* tfunc)
{
  // to be safe
  stop();

  // todo, take this as input
  pinMode(2, INPUT);

  motion = false;
  history = 0;

  motionFunction = mfunc ? strdup(mfunc) : NULL;
  timeoutFunction = tfunc ? strdup(tfunc) : NULL;

  timeoutTimer.interval = timeoutInterval;

  readTimer.interval = readInterval;
  SYS_TimerStart(&readTimer);
}

void DigitalMotionModule::stop(){
  if(SYS_TimerStarted(&readTimer)){
    SYS_TimerStop(&readTimer);
  }

  if(SYS_TimerStarted(&timeoutTimer)){
    SYS_TimerStop(&timeoutTimer);
  }

  if (motionFunction){
    free(motionFunction);
    motionFunction = NULL;
  }

  if (timeoutFunction){
    free(timeoutFunction);
    timeoutFunction = NULL;
  }
}

void DigitalMotionModule::read() {
  
  uint8_t reading = digitalRead(2);
  history = history << 1 | reading;
  
  // if we're current off and weve seen motion in all time periods
  if (!motion && history & 0xFF000000 && history & 0x00FF0000 && history & 0x0000FF00 && history & 0x000000FF) {
    motion = true;
    if(motionFunction){
      Shell.eval(motionFunction);
    }
  }

  // if we got a hit and motion is true we should set timeoutTimer
  if (reading && motion) {
    // if we're already started, restart (and reset interval?)
    if (SYS_TimerStarted(&timeoutTimer)) {
      SYS_TimerStop(&timeoutTimer);
    }
    SYS_TimerStart(&timeoutTimer);
  }
}

bool DigitalMotionModule::getStatus() {
  return motion;
}

void DigitalMotionModule::timeout() {
  if(timeoutFunction){
    Shell.eval(timeoutFunction);
  }
  motion = false;
}

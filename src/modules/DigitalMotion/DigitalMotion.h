
#ifndef LIB_PINOCCIO_DIGITALMOTION_MODULE_H_
#define LIB_PINOCCIO_DIGITALMOTION_MODULE_H_

#include "../Module.h"

namespace pinoccio {
  class DigitalMotionModule : public Module {

    public:

      bool enable();
      const __FlashStringHelper *name() const;
      void loop();
      static DigitalMotionModule instance;

      void start(uint32_t readInterval, uint32_t timeoutInterval, const char* mfunc, const char* tfunc);
      void stop();
      void read();
      bool getStatus();
      void timeout();

    protected:
      bool motion;
      uint64_t history;

      char * motionFunction;
      char * timeoutFunction;

      SYS_Timer_t readTimer;
      SYS_Timer_t timeoutTimer;

    private:
      using Module::Module;
  };
}

#endif
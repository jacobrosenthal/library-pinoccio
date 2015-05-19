
#ifndef LIB_PINOCCIO_SHT_MODULE_H_
#define LIB_PINOCCIO_SHT_MODULE_H_

#include "../Module.h"

namespace pinoccio {
  class SHTModule : public Module {

    public:
      bool enable();
      const __FlashStringHelper *name() const;
      void loop();
      static SHTModule instance;

    protected:
    

    private:
      using Module::Module;
  };
}

#endif
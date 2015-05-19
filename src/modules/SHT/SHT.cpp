
#include <Arduino.h>
#include <Scout.h>
#include "modules/SHT/SHT.h"
#include "modules/SHT/SHT2x.h"

using namespace pinoccio;

SHTModule SHTModule::instance;

const __FlashStringHelper *SHTModule::name() const {
  return F("sht");
}

static numvar shtTemperatureC(void) {
  return SHT2x.readT();
}

static numvar shtTemperatureF(void) {
  float f;
  f = round((1.8 * SHT2x.readT()) + 32);
  return (uint32_t)f;
}

static numvar shtHumidity(void) {
  return SHT2x.readRH();
}


bool SHTModule::enable() {
  
  //set 11 bit resolution for ~100ms reads
  uint8_t reg = SHT2x.readUserRegister();
  SHT2x.writeUserRegister(reg || SHT2x_RES_11_11BIT);

  Shell.addFunction("sht.humidity", shtHumidity);
  Shell.addFunction("sht.temperature.c", shtTemperatureC);
  Shell.addFunction("sht.temperature.f", shtTemperatureF);

}

void SHTModule::loop() {


}

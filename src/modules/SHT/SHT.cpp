
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
  SHTModule* m = &(SHTModule::instance);
  return (uint32_t)m->getC();
}

static numvar shtTemperatureF(void) {
  SHTModule* m = &(SHTModule::instance);
  return (uint32_t)m->getF();
}

static numvar shtTemperatureLastC(void) {
  SHTModule* m = &(SHTModule::instance);
  return (uint32_t)m->getLastC();
}

static numvar shtTemperatureLastF(void) {
  SHTModule* m = &(SHTModule::instance);
  return (uint32_t)m->getLastF();
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

  Shell.addFunction("sht.temperature.lastc", shtTemperatureC);
  Shell.addFunction("sht.temperature.lastf", shtTemperatureC);

}

void SHTModule::loop() {
}

float SHTModule::getC() {
  lastC = SHT2x.readT();
  return lastC;
}

float SHTModule::getF() {
  lastC = SHT2x.readT();
  return round((1.8 * lastC) + 32);
}

float SHTModule::getLastC() {
  return lastC;
}

float SHTModule::getLastF() {
  return round((1.8 * lastC) + 32);
}

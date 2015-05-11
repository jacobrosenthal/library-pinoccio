/**************************************************************************\
* Pinoccio Library                                                         *
* https://github.com/Pinoccio/library-pinoccio                             *
* Copyright (c) 2012-2014, Pinoccio Inc. All rights reserved.              *
* ------------------------------------------------------------------------ *
*  This program is free software; you can redistribute it and/or modify it *
*  under the terms of the BSD License as described in license.txt.         *
\**************************************************************************/
#include <stdbool.h>
#include "halTemperature.h"
#include "SHT2x.h"


void HAL_TemperatureConfig() {
  uint8_t reg = SHT2x.readUserRegister();
  SHT2x.writeUserRegister(reg || SHT2x_RES_11_11BIT);
}

int16_t HAL_MeasureTemperature(void) {
  return SHT2x.readT() * 100;
}

#ifndef MODULE_SHT30_H
#define MODULE_SHT30_H

#include <Adafruit_SHT31.h>
#include <Wire.h>

extern Adafruit_SHT31 SHT;

bool SHT30_Setup(void);

#endif // MODULE_SHT30_H
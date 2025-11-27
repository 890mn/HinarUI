#ifndef MODULE_SHT30_H
#define MODULE_SHT30_H

#include <Adafruit_SHT31.h>
#include <Wire.h>

extern Adafruit_SHT31 SHT;

bool SHT30_Setup(void);
float SHT30_LastTemp();
float SHT30_LastHum();
struct Sht30Status {
    float temp;
    float hum;
};
Sht30Status sht30ReadStatus(unsigned long now, unsigned long intervalMs = 5000, bool force = false);

#endif // MODULE_SHT30_H

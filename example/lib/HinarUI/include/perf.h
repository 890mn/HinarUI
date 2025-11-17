#ifndef PERF_H
#define PERF_H

#include <Arduino.h>

class PerfMonitor {
public:
    void begin();
    void loop();
    void onFrame(uint32_t frameMs, uint32_t pixelCount, float coveragePercent);
};

extern PerfMonitor perf;

#endif

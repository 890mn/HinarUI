#ifndef PERF_H
#define PERF_H

#include <Arduino.h>

class PerfMonitor {
public:
    void begin();
    void loop();
    void onFrame(uint32_t frameMs, uint32_t pixelCount, float coveragePercent);
    float currentFps() const { return currentFps_; }
    const String& fpsLabel() const { return fpsLabel_; }

private:
    float currentFps_ = 0.0f;
    String fpsLabel_ = "HinarUI";
};

extern PerfMonitor perf;

#endif

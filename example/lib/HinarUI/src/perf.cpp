#include "perf.h"

#include <Arduino.h>

#include "HinarUI/core/FrameBufferManager.h"

namespace {
uint32_t lastPrint = 0;
}

void PerfMonitor::begin() {
    lastPrint = millis();
}

void PerfMonitor::loop() {
    const uint32_t now = millis();
    if (now - lastPrint < 500) return;
    lastPrint = now;

    const uint32_t pixels = frameBuffer.lastPixelCount();
    const uint32_t frameMs = frameBuffer.lastFrameTimeMs();
    float fps = frameMs ? (500.0f / frameMs) : 0.0f;

    Serial.print(F("[PERF] pixels="));
    Serial.print(pixels);
    Serial.print(F(" frame_ms="));
    Serial.print(frameMs);
    Serial.print(F(" fps="));
    Serial.print(fps, 1);
    Serial.print(F(" coverage="));
    Serial.print(frameBuffer.lastCoveragePercent(), 1);
    Serial.println(F("%"));
}

PerfMonitor perf;

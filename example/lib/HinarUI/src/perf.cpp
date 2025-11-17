#include "perf.h"

#include <Arduino.h>

namespace {
uint32_t lastPrint = 0;
uint32_t framesInWindow = 0;
float coverageAccum = 0.0f;
uint32_t lastPixels = 0;
uint32_t lastFrameMsValue = 0;
float lastCoverage = 0.0f;
}

void PerfMonitor::begin() {
    lastPrint = millis();
    framesInWindow = 0;
    coverageAccum = 0.0f;
    lastPixels = 0;
    lastFrameMsValue = 0;
    lastCoverage = 0.0f;
}

void PerfMonitor::loop() {
    // Deprecated: perf reporting now happens via onFrame().
}

void PerfMonitor::onFrame(uint32_t frameMs, uint32_t pixelCount, float coveragePercent) {
    if (lastPrint == 0) lastPrint = millis();

    ++framesInWindow;
    coverageAccum += coveragePercent;
    lastPixels = pixelCount;
    lastFrameMsValue = frameMs;
    lastCoverage = coveragePercent;

    uint32_t now = millis();
    uint32_t elapsed = now - lastPrint;
    if (elapsed < 1000) return;

    float fps = elapsed ? (framesInWindow * 1000.0f) / elapsed : 0.0f;
    float avgCoverage = framesInWindow ? coverageAccum / framesInWindow : lastCoverage;

    Serial.print(F("[PERF] pixels="));
    Serial.print(lastPixels);
    Serial.print(F(" frame_ms="));
    Serial.print(lastFrameMsValue);
    Serial.print(F(" fps="));
    Serial.print(fps, 1);
    Serial.print(F(" coverage="));
    Serial.print(avgCoverage, 1);
    Serial.println(F("%"));

    lastPrint = now;
    framesInWindow = 0;
    coverageAccum = 0.0f;
}

PerfMonitor perf;

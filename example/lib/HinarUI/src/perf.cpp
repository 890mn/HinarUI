#include "perf.h"

#include <Arduino.h>

namespace {
uint32_t lastPrint = 0;
uint32_t lastPixels = 0;
float avgFrameMs = 16.0f;
float avgCoverage = 0.0f;
bool hasFrame = false;
const float kSmoothing = 0.12f;
}

void PerfMonitor::begin() {
    lastPrint = millis();
    lastPixels = 0;
    avgFrameMs = 16.0f;
    avgCoverage = 0.0f;
    hasFrame = false;
}

void PerfMonitor::loop() {
    if (lastPrint == 0) lastPrint = millis();
    const uint32_t now = millis();
    if (now - lastPrint < 500) return;

    if (hasFrame) {
        float fps = avgFrameMs > 0.0f ? (1000.0f / avgFrameMs) : 0.0f;
        Serial.print(F("[PERF] pixels="));
        Serial.print(lastPixels);
        Serial.print(F(" frame_ms="));
        Serial.print(avgFrameMs, 1);
        Serial.print(F(" fps="));
        Serial.print(fps, 1);
        Serial.print(F(" coverage="));
        Serial.print(avgCoverage, 1);
        Serial.println(F("%"));
    }

    lastPrint = now;
}

void PerfMonitor::onFrame(uint32_t frameMs, uint32_t pixelCount,
                          float coveragePercent) {
    if (lastPrint == 0) lastPrint = millis();
    hasFrame = true;
    avgFrameMs =
        (1.0f - kSmoothing) * avgFrameMs + kSmoothing * frameMs;
    avgCoverage =
        (1.0f - kSmoothing) * avgCoverage + kSmoothing * coveragePercent;
    lastPixels = pixelCount;
}

PerfMonitor perf;

#include "perf.h"

#include <Arduino.h>

#if defined(ARDUINO_ARCH_ESP32)
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/queue.h>
#endif

namespace {
struct FrameSample {
    uint32_t frameMs;
    uint32_t pixels;
    float coverage;
    uint32_t timestamp;
};

#if defined(ARDUINO_ARCH_ESP32)
QueueHandle_t frameQueue = nullptr;
TaskHandle_t perfTaskHandle = nullptr;
#endif

uint32_t lastWindowStart = 0;
uint32_t framesInWindow = 0;
float coverageAccum = 0.0f;
uint32_t lastPixels = 0;
float lastFrameMsValue = 0.0f;
PerfMonitor* gPerf = nullptr;
}  // namespace

void PerfMonitor::begin() {
    lastWindowStart = millis();
    framesInWindow = 0;
    coverageAccum = 0.0f;
    lastPixels = 0;
    lastFrameMsValue = 0.0f;
    fpsLabel_ = "N/A";
    currentFps_ = 0.0f;
    gPerf = this;

#if defined(ARDUINO_ARCH_ESP32)
    if (!frameQueue) {
        frameQueue = xQueueCreate(8, sizeof(FrameSample));
    }
    if (!perfTaskHandle && frameQueue) {
        auto task = [](void*) {
            FrameSample sample;
            for (;;) {
                if (xQueueReceive(frameQueue, &sample, pdMS_TO_TICKS(50)) == pdTRUE) {
                    // accumulate
                    ++framesInWindow;
                    coverageAccum += sample.coverage;
                    lastPixels = sample.pixels;
                    lastFrameMsValue = sample.frameMs;
                }

                uint32_t now = millis();
                uint32_t elapsed = now - lastWindowStart;
                if (elapsed >= 500 && framesInWindow > 0) {
                    if (gPerf) {
                        gPerf->currentFps_ = (framesInWindow * 1000.0f) / elapsed;
                        float avgCoverage = coverageAccum / framesInWindow;

                        char buf[16];
                        dtostrf(gPerf->currentFps_, 0, 1, buf);
                        gPerf->fpsLabel_ = String(buf) + "fps";
                        gPerf->avgCoverage_ = avgCoverage;
                    }

                    // reset window
                    lastWindowStart = now;
                    framesInWindow = 0;
                    coverageAccum = 0.0f;
                }

                vTaskDelay(pdMS_TO_TICKS(10));
            }
        };
        xTaskCreatePinnedToCore(task, "perf_task", 2048, nullptr, 2, &perfTaskHandle, 0);
    }
#endif
}

void PerfMonitor::loop() {
    // On ESP32 we rely on the background task. On other platforms we do nothing.
#if !defined(ARDUINO_ARCH_ESP32)
    uint32_t now = millis();
    uint32_t elapsed = now - lastWindowStart;
    if (elapsed >= 500 && framesInWindow > 0) {
        currentFps_ = (framesInWindow * 1000.0f) / elapsed;
        float avgCoverage = coverageAccum / framesInWindow;

        char buf[16];
        dtostrf(currentFps_, 0, 1, buf);
        fpsLabel_ = String(buf) + "fps";

        lastWindowStart = now;
        framesInWindow = 0;
        coverageAccum = 0.0f;
    }
#endif
}

void PerfMonitor::onFrame(uint32_t frameMs, uint32_t pixelCount, float coveragePercent) {
#if defined(ARDUINO_ARCH_ESP32)
    if (frameQueue) {
        FrameSample sample{frameMs, pixelCount, coveragePercent, millis()};
        xQueueSend(frameQueue, &sample, 0);
    }
#else
    ++framesInWindow;
    coverageAccum += coveragePercent;
    lastPixels = pixelCount;
    lastFrameMsValue = frameMs;
#endif
}

PerfMonitor perf;

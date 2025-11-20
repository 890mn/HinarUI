#include "HinarUI/core/FrameBufferManager.h"

#include <string.h>

#include "modules/module_oled.h"
#include "perf.h"

#if defined(ARDUINO_ARCH_ESP32)
#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>
#include <freertos/task.h>
#endif

FrameBufferManager::FrameBufferManager(HinarUIDisplay& display)
    : display_(display) {
    frontBuffer_.fill(0);
    totalPixels_ = display_.width() * display_.height();
#if defined(ARDUINO_ARCH_ESP32)
    frameQueue_ = xQueueCreate(8, sizeof(FrameRequest));
    xTaskCreatePinnedToCore(
        [](void* param) {
            FrameBufferManager* mgr = static_cast<FrameBufferManager*>(param);
            TickType_t last = xTaskGetTickCount();
            uint32_t lastDisplayMs = millis();
            for (;;) {
                FrameRequest req{};
                if (mgr->dequeueFrame(req) && req.buffer) {
                    mgr->display_.useExternalBuffer(req.buffer);
                    if (req.full) {
                        mgr->display_.commitFrame();
                    } else {
                        mgr->display_.flushRegion(req.x, req.y, req.w, req.h, req.buffer);
                    }

                    uint32_t nowMs = millis();
                    uint32_t frameMs = nowMs - lastDisplayMs;
                    lastDisplayMs = nowMs;
                    mgr->lastFrameMs_ = frameMs;
                    mgr->lastPixelCount_ = req.full ? mgr->totalPixels_ : (req.w * req.h);
                    mgr->coveragePercent_ = mgr->totalPixels_ ? (100.0f * mgr->lastPixelCount_) / mgr->totalPixels_ : 0.0f;
                    ++mgr->frameCounter_;
                    perf.onFrame(frameMs, mgr->lastPixelCount_, mgr->coveragePercent_);

                    mgr->display_.releaseFrameBuffer(req.buffer);
                }
                TickType_t period = pdMS_TO_TICKS(1000 / mgr->targetFps_);
                vTaskDelayUntil(&last, period);
            }
        },
        "disp_task", 2048, this, 2, nullptr, 0);
#else
    pendingValid_ = false;
#endif
}

void FrameBufferManager::beginFrame() {
    frameStartMs_ = millis();
    drawingBuffer_ = acquireBackBuffer();
    if (drawingBuffer_) {
        display_.useExternalBuffer(drawingBuffer_);
    }
}

void FrameBufferManager::endFrame() {
    uint8_t* backBuffer = drawingBuffer_ ? drawingBuffer_ : display_.getBuffer();
    if (!backBuffer) {
        drawingBuffer_ = nullptr;
        return;
    }

    // 如果当前缓冲不是外部独立缓冲，则直接推屏
    if (backBuffer == display_.getBuffer()) {
        const int16_t width = display_.width();
        const int16_t height = display_.height();
        display_.commitFrame();
        lastPixelCount_ = width * height;
        lastFrameMs_ = millis() - frameStartMs_;
        coveragePercent_ = totalPixels_ ? (100.0f * lastPixelCount_) / totalPixels_ : 0.0f;
        fullRefreshRequested_ = false;
        ++frameCounter_;
        perf.onFrame(lastFrameMs_, lastPixelCount_, coveragePercent_);
    } else {
        submitFrame(backBuffer, true);
        fullRefreshRequested_ = false;
    }
    drawingBuffer_ = nullptr;
}

void FrameBufferManager::forceFullRefresh() {
    fullRefreshRequested_ = true;
}

bool FrameBufferManager::needsFullRefreshHint() const {
    return fullRefreshRequested_;
}

#if defined(ARDUINO_ARCH_ESP32)
uint8_t* FrameBufferManager::acquireBackBuffer() {
    return display_.acquireFrameBuffer();
}

void FrameBufferManager::submitFrame(uint8_t* buf, bool full, int16_t x, int16_t y, int16_t w, int16_t h) {
    if (!buf) return;
    if (frameQueue_) {
        FrameRequest req{buf, full, x, y, w, h};
        if (xQueueSend((QueueHandle_t)frameQueue_, &req, 0) != pdTRUE) {
            FrameRequest dropped{};
            xQueueReceive((QueueHandle_t)frameQueue_, &dropped, 0);
            display_.releaseFrameBuffer(dropped.buffer);
            xQueueSend((QueueHandle_t)frameQueue_, &req, 0);
        }
    } else {
        display_.releaseFrameBuffer(buf);
    }
}

bool FrameBufferManager::dequeueFrame(FrameRequest& outReq) {
    if (frameQueue_ && xQueueReceive((QueueHandle_t)frameQueue_, &outReq, 0) == pdTRUE) {
        return true;
    }
    return false;
}
#else

uint8_t* FrameBufferManager::acquireBackBuffer() {
    return display_.acquireFrameBuffer();
}

void FrameBufferManager::submitFrame(uint8_t* buf, bool full, int16_t x, int16_t y, int16_t w, int16_t h) {
    if (pendingValid_) {
        display_.releaseFrameBuffer(pendingReq_.buffer);
    }
    pendingReq_ = {buf, full, x, y, w, h};
    pendingValid_ = true;
}

bool FrameBufferManager::dequeueFrame(FrameRequest& outReq) {
    if (!pendingValid_) {
        return false;
    }
    outReq = pendingReq_;
    pendingValid_ = false;
    return true;
}
#endif

FrameBufferManager frameBuffer(display);

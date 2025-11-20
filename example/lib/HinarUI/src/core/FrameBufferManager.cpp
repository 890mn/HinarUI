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
    frameQueue_ = xQueueCreate(8, sizeof(uint8_t*));
    xTaskCreatePinnedToCore(
        [](void* param) {
            FrameBufferManager* mgr = static_cast<FrameBufferManager*>(param);
            const TickType_t period = pdMS_TO_TICKS(1000 / 60);
            TickType_t last = xTaskGetTickCount();
            uint32_t lastDisplayMs = millis();
            for (;;) {
                uint8_t* nextBuf = nullptr;
                if (mgr->dequeueFrame(nextBuf) && nextBuf) {
                    mgr->display_.useExternalBuffer(nextBuf);
                    mgr->display_.commitFrame();

                    uint32_t nowMs = millis();
                    uint32_t frameMs = nowMs - lastDisplayMs;
                    lastDisplayMs = nowMs;
                    mgr->lastFrameMs_ = frameMs;
                    mgr->lastPixelCount_ = mgr->totalPixels_;
                    mgr->coveragePercent_ = 100.0f;
                    ++mgr->frameCounter_;
                    perf.onFrame(frameMs, mgr->lastPixelCount_, mgr->coveragePercent_);

                    mgr->display_.releaseFrameBuffer(nextBuf);
                }
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
        submitFrame(backBuffer);
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

void FrameBufferManager::submitFrame(uint8_t* buf) {
    if (!buf) return;
    if (frameQueue_) {
        if (xQueueSend((QueueHandle_t)frameQueue_, &buf, 0) != pdTRUE) {
            uint8_t* dropped = nullptr;
            xQueueReceive((QueueHandle_t)frameQueue_, &dropped, 0);
            display_.releaseFrameBuffer(dropped);
            xQueueSend((QueueHandle_t)frameQueue_, &buf, 0);
        }
    } else {
        display_.releaseFrameBuffer(buf);
    }
}

bool FrameBufferManager::dequeueFrame(uint8_t*& outBuf) {
    if (frameQueue_ && xQueueReceive((QueueHandle_t)frameQueue_, &outBuf, 0) == pdTRUE) {
        return true;
    }
    outBuf = nullptr;
    return false;
}
#else

uint8_t* FrameBufferManager::acquireBackBuffer() {
    return display_.acquireFrameBuffer();
}

void FrameBufferManager::submitFrame(uint8_t* buf) {
    if (pendingValid_) {
        display_.releaseFrameBuffer(pendingFrame_);
    }
    pendingFrame_ = buf;
    pendingValid_ = true;
}

bool FrameBufferManager::dequeueFrame(uint8_t*& outBuf) {
    if (!pendingValid_) {
        outBuf = nullptr;
        return false;
    }
    outBuf = pendingFrame_;
    pendingValid_ = false;
    return true;
}
#endif

FrameBufferManager frameBuffer(display);

#include "HinarUI/core/FrameBufferManager.h"

#include <string.h>

#include "modules/module_oled.h"
#include "perf.h"

#if defined(ARDUINO_ARCH_ESP32)
#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>
#endif

FrameBufferManager::FrameBufferManager(HinarUIDisplay& display)
    : display_(display) {
    frontBuffer_.fill(0);
    totalPixels_ = display_.width() * display_.height();
#if defined(ARDUINO_ARCH_ESP32)
    frameQueue_ = xQueueCreate(8, sizeof(uint8_t*));
#else
    pendingValid_ = false;
#endif
}

void FrameBufferManager::beginFrame() {
    frameStartMs_ = millis();
}

void FrameBufferManager::endFrame() {
    uint8_t* backBuffer = display_.getBuffer();
    if (!backBuffer) return;

    const int16_t width = display_.width();
    const int16_t height = display_.height();
    const int16_t pages = (height + 7) / 8;
    const size_t byteCount = width * pages;

    memcpy(frontBuffer_.data(), backBuffer, byteCount);
    display_.commitFrame();

    lastPixelCount_ = width * height;
    lastFrameMs_ = millis() - frameStartMs_;
    coveragePercent_ = totalPixels_ ? (100.0f * lastPixelCount_) / totalPixels_ : 0.0f;
    fullRefreshRequested_ = false;
    perf.onFrame(lastFrameMs_, lastPixelCount_, coveragePercent_);
    ++frameCounter_;
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

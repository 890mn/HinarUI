#include "HinarUI/core/FrameBufferManager.h"

#include <string.h>

#include "modules/module_oled.h"

FrameBufferManager::FrameBufferManager(HinarUIDisplay& display)
    : display_(display) {
    frontBuffer_.fill(0);
    totalPixels_ = display_.width() * display_.height();
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
    display_.display();

    lastPixelCount_ = width * height;
    lastFrameMs_ = millis() - frameStartMs_;
    coveragePercent_ = totalPixels_ ? (100.0f * lastPixelCount_) / totalPixels_ : 0.0f;
    fullRefreshRequested_ = false;
}

void FrameBufferManager::forceFullRefresh() {
    fullRefreshRequested_ = true;
}

bool FrameBufferManager::needsFullRefreshHint() const {
    return fullRefreshRequested_;
}

FrameBufferManager frameBuffer(display);

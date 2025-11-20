#ifndef HINARUI_FRAMEBUFFER_MANAGER_H
#define HINARUI_FRAMEBUFFER_MANAGER_H

#include <Arduino.h>
#include <array>

#include "HinarUI/core/HinarUIDisplay.h"

class FrameBufferManager {
public:
    explicit FrameBufferManager(HinarUIDisplay& display);

    void beginFrame();
    void endFrame();

    void forceFullRefresh();
    bool needsFullRefreshHint() const;

    uint32_t lastPixelCount() const { return lastPixelCount_; }
    uint32_t lastFrameTimeMs() const { return lastFrameMs_; }
    float lastCoveragePercent() const { return coveragePercent_; }
    uint32_t totalPixels() const { return totalPixels_; }
    uint32_t totalFrames() const { return frameCounter_; }

    // 新增：渲染线程申请/提交帧，显示线程消费。
    uint8_t* acquireBackBuffer();
    void submitFrame(uint8_t* buf);
    bool dequeueFrame(uint8_t*& outBuf);

private:
    HinarUIDisplay& display_;
    static const size_t kBufferSize = 1024;
    std::array<uint8_t, kBufferSize> frontBuffer_;
    bool fullRefreshRequested_ = true;
    uint32_t frameStartMs_ = 0;
    uint32_t lastPixelCount_ = 0;
    uint32_t lastFrameMs_ = 0;
    float coveragePercent_ = 0.0f;
    uint32_t totalPixels_ = 0;
    uint32_t frameCounter_ = 0;

#if defined(ARDUINO_ARCH_ESP32)
    void* frameQueue_ = nullptr;  // QueueHandle_t*
#else
    uint8_t* pendingFrame_ = nullptr;
    bool pendingValid_ = false;
#endif
};

extern FrameBufferManager frameBuffer;

#endif

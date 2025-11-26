#ifndef HINARUI_FRAMEBUFFER_MANAGER_H
#define HINARUI_FRAMEBUFFER_MANAGER_H

#include <Arduino.h>
#include <array>

#include "HinarUI/core/HinarUIDisplay.h"

struct FrameRequest {
    uint8_t* buffer;
    bool full;
    int16_t x;
    int16_t y;
    int16_t w;
    int16_t h;
};

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
    void submitFrame(uint8_t* buf, bool full = true, int16_t x = 0, int16_t y = 0, int16_t w = 0, int16_t h = 0);
    bool dequeueFrame(FrameRequest& outReq);

    void setTargetFps(uint8_t fps) { targetFps_ = fps > 60 ? 60 : (fps == 0 ? 1 : fps); }
    uint8_t targetFps() const { return targetFps_; }

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
    uint8_t* drawingBuffer_ = nullptr;
    uint8_t targetFps_ = 60;

#if defined(ARDUINO_ARCH_ESP32)
    void* frameQueue_ = nullptr;  // QueueHandle_t*
#else
    FrameRequest pendingReq_{};
    bool pendingValid_ = false;
#endif
};

extern FrameBufferManager frameBuffer;

#endif

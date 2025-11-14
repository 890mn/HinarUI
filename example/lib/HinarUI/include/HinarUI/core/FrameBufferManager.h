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
};

extern FrameBufferManager frameBuffer;

#endif

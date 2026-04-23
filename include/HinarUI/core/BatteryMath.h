#pragma once

#include <math.h>

namespace HinarUI {

inline float batteryVoltageFromRaw(int raw, float referenceVoltage = 3.3f,
                                   float dividerRatio = 2.0f, float calibrationOffset = 0.27f) {
    return raw * referenceVoltage / 4095.0f * dividerRatio + calibrationOffset;
}

inline int batteryPercentFromVoltage(float voltage) {
    float percent = 100.3f / (1.0f + expf(-20.0f * (voltage - 3.7f)));
    if (percent > 100.0f) return 100;
    if (percent < 0.0f) return 0;
    return static_cast<int>(percent);
}

}  // namespace HinarUI

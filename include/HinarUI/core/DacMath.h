#pragma once

#include <stdint.h>

namespace HinarUI {

inline float clampDacVoltage(float value, float minVoltage = 0.0f, float maxVoltage = 2.0f) {
    if (value < minVoltage) {
        return minVoltage;
    }
    if (value > maxVoltage) {
        return maxVoltage;
    }
    return value;
}

inline uint8_t dacValueFromVoltage(float volts, float referenceVoltage = 3.3f,
                                   float minVoltage = 0.0f, float maxVoltage = 2.0f) {
    float ratio = clampDacVoltage(volts, minVoltage, maxVoltage) / referenceVoltage;
    if (ratio < 0.0f) {
        ratio = 0.0f;
    }
    if (ratio > 1.0f) {
        ratio = 1.0f;
    }
    return static_cast<uint8_t>(ratio * 255.0f + 0.5f);
}

}  // namespace HinarUI

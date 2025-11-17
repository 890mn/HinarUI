#pragma once

struct MenuConfig {
    static constexpr int kMaxModules = 6;

    int stepCount           = 12;
    int flowSpeedFastPlus   = 5;
    int flowSpeedFast       = 20;
    int flowSpeedNormal     = 25;
    int flowSpeedSlow       = 35;
    int moduleMax           = kMaxModules;
    int moduleOffset        = 45;
    int moduleDirection     = -1;
    int moduleForward       = 3;
    int radiusPall          = 2;
    int radiusRect          = 5;

    constexpr int moduleBackward() const {
        return moduleMax - moduleForward;
    }
};

inline MenuConfig DefaultMenuConfig() {
    return MenuConfig{};
}

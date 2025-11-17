#pragma once

#include "HinarUI/core/MenuTypes.h"

class Menu;

class MenuAnimator {
public:
    explicit MenuAnimator(Menu& owner);

    void renderDynamic(int keyCycleState, bool isForward);
    void renderForward();
    void renderBackward();

    float easeInOut(float t) const;
    String getFlowSpeed() const;

    bool isAnimating() const;

private:
    Menu& menu_;
};

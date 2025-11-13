#include "HinarUI/core/MenuAnimator.h"

#include "HinarUI_Core.h"

MenuAnimator::MenuAnimator(Menu& owner) : menu_(owner) {}

bool MenuAnimator::isAnimating() const {
    return menu_.isAnimating;
}

String MenuAnimator::getFlowSpeed() const {
    if (menu_.flowSpeed == menu_.config.flowSpeedFast)      return "FAST";
    if (menu_.flowSpeed == menu_.config.flowSpeedFastPlus)  return "FAST_PLUS";
    if (menu_.flowSpeed == menu_.config.flowSpeedNormal)    return "NORMAL";
    if (menu_.flowSpeed == menu_.config.flowSpeedSlow)      return "SLOW";
    return "ERROR";
}

void MenuAnimator::renderDynamic(int keyCycleState, bool isForward) {
    if (keyCycleState == LOW) {
        if (menu_.CyclePress == 0) {
            menu_.CyclePress = menu_.currentTime;
        } else {
            menu_.flowSpeed = ((menu_.currentTime - menu_.CyclePress) > menu_.Threshold)
                                ? menu_.config.flowSpeedFastPlus
                                : menu_.config.flowSpeedNormal;
            menu_.isAnimating = true;
            if (isForward) renderForward();
            else renderBackward();
        }
    } else if (keyCycleState == HIGH && menu_.CyclePress > 0) {
        menu_.CyclePress = 0;
        menu_.currentState = isForward ? MenuState::Idle : MenuState::BackwardSelected;
    }
}

void MenuAnimator::renderForward() {
    menu_.curStep = 0;
    PAGE_NAME = menu_.forwardPointer == menu_.config.moduleForward - 1 ? "BACKWARD" : "FORWARD";

    if (menu_.forwardPointer == menu_.config.moduleForward) {
        menu_.tmpLabel = menu_.registry.label(menu_.config.moduleForward);
        menu_.tmpIcons = menu_.registry.icon(menu_.config.moduleForward);
   
        menu_.registry.label(menu_.config.moduleForward) = menu_.registry.label(menu_.i_back);
        menu_.registry.icon(menu_.config.moduleForward)  = menu_.registry.icon(menu_.i_back);
    }

    while (menu_.curStep < menu_.totalStep) {
        menu_.draw(-menu_.config.moduleOffset / menu_.totalStep, false, true);
        menu_.curStep++;
        delay(menu_.flowSpeed);
    }

    if (menu_.forwardPointer == menu_.config.moduleForward) {
        menu_.forwardPointer = 0;
        menu_.i_back = menu_.backwardPointer;

        menu_.registry.label(menu_.config.moduleForward) = menu_.tmpLabel;
        menu_.registry.icon(menu_.config.moduleForward)  = menu_.tmpIcons;

        menu_.isBackward = false;
        menu_.Icon = ModuleVisual(10, 25, 20, 30);
        menu_.draw(0, true, true);
        menu_.isAnimating = false;
        return;
    }

    ++menu_.forwardPointer;
    menu_.isAnimating = false;
}

void MenuAnimator::renderBackward() {
    menu_.curStep = 0;

    while (menu_.curStep < menu_.totalStep) {
        menu_.draw(1, false, false);
        menu_.curStep++;
        delay(menu_.flowSpeed);
    }

    if (menu_.i_back == menu_.config.moduleMax - 1) {
        menu_.i_back = menu_.backwardPointer;
    } else {
        ++menu_.i_back;
    }

    menu_.isAnimating = false;
}

float MenuAnimator::easeInOut(float t) const {
    if (t < 0.7) {
        return 2 * t * t;
    }
    return -1 + (4 - 2 * t) * t;
}


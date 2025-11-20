#include "HinarUI_Core.h"

String PAGE_NAME = "FORWARD";
String UI_NAME   = "HinarUI";

Menu::Menu()
    : config(DefaultMenuConfig()),
      registry(),
      animator(*this),
      renderer(*this) {
    wordStep = config.stepCount / 2;
    totalStep = config.stepCount;
    backwardPointer = config.moduleForward;
    i_back = backwardPointer;
    flowSpeed = config.flowSpeedNormal;
    tmpLabel = registry.label(config.moduleForward);
    tmpIcons = registry.icon(config.moduleForward);
}

Menu menu;

void Menu::create() {
    if (!board_init()) return;
    frameBuffer.setTargetFps(60);
    draw(0, true, true);
}

void Menu::loop() {
    static int prevKeyEnterState = HIGH;
    static int prevKeyCycleState = HIGH;
    static int prevKeyBackState  = HIGH;
    static int prevKeyOffState   = HIGH;

    static MenuState previousState = MenuState::Idle;
    currentTime = millis();

    if (!animator.isAnimating()) {
        int keyEnterState = digitalRead(KEY_ENTER);
        int keyCycleState = digitalRead(KEY_CYCLE);
        int keyBackState  = digitalRead(KEY_BACK);
        int keyOffState   = digitalRead(KEY_OFF);

        if (keyEnterState != prevKeyEnterState) {
            //Serial.print("Enter Status Changed: ");
            //Serial.println(keyEnterState == 0 ? "Pressed" : "Exited");
            prevKeyEnterState = keyEnterState;
        }
        if (keyCycleState != prevKeyCycleState) {
            //Serial.print("Cycle Status Changed: ");
            //Serial.println(keyCycleState == 0 ? "Pressed" : "Exited");
            prevKeyCycleState = keyCycleState;
        }
        if (keyBackState != prevKeyBackState) {
            //Serial.print("Back Status Changed: ");
            //Serial.println(keyBackState == 0 ? "Pressed" : "Exited");
            prevKeyBackState = keyBackState;
        }
        if (keyOffState != prevKeyOffState) {
            //Serial.print("Off Status Changed: ");
            //Serial.println(keyOffState == 0 ? "Pressed" : "Exited");
            prevKeyOffState = keyOffState;
        }
        if (currentState != previousState) {
            Serial.print("State Changed to: ");
            Serial.println(stateToString());
            previousState = currentState;
        }

        switch (currentState) {
            case MenuState::Idle:
                frameBuffer.setTargetFps(10);
                if (keyEnterState == LOW && forwardPointer == config.moduleForward) {
                    isBackward = true;
                    frameBuffer.beginFrame();
                    display.fillRoundRect(89, 35, 33, 8, config.radiusPall, SELECTED_COLOR);
                    frameBuffer.endFrame();
                    currentState = MenuState::Backward;
                } else if (keyCycleState == LOW) {
                    currentState = MenuState::Forward;
                } else if (keyEnterState == LOW) {
                    if (auto handler = registry.handler(forwardPointer)) {
                        frameBuffer.forceFullRefresh();
                        handler();
                    }
                    currentState = MenuState::Module;
                }
                break;

            case MenuState::Forward:
                frameBuffer.setTargetFps(60);
                animator.renderDynamic(keyCycleState, true);
                break;

            case MenuState::Backward:
                frameBuffer.setTargetFps(60);
                if (keyEnterState == LOW) {
                    isUP = true;
                    while (keyEnterState == LOW) {
                        keyEnterState = digitalRead(KEY_ENTER);
                        delay(3);
                    }
                    currentState = MenuState::BackwardSelected;
                }
                break;

            case MenuState::BackwardSelected:
                frameBuffer.setTargetFps(60);
                animator.renderDynamic(keyCycleState, false);
                if (keyBackState == LOW) {
                    frameBuffer.beginFrame();
                    display.fillRect(90, 36, 31, 6, UNSELECTED_COLOR);
                    frameBuffer.endFrame();
                    isBackward = false;
                    isUP = false;
                    currentState = MenuState::Idle;
                } else if (keyEnterState == LOW) {
                    if (auto handler = registry.handler(i_back)) {
                        frameBuffer.forceFullRefresh();
                        handler();
                    }
                    currentState = MenuState::Module;
                }
                break;

            case MenuState::Module: {
                frameBuffer.setTargetFps(30);
                static unsigned long lastUpdateTime = 0;
                unsigned long now = millis();
                if (now - lastUpdateTime >= 1000) {
                    lastUpdateTime = now;
                    int targetIndex = forwardPointer != config.moduleForward ? forwardPointer : i_back;
                    if (auto handler = registry.handler(targetIndex)) {
                        frameBuffer.forceFullRefresh();
                        handler();
                    }
                }
                if (keyBackState == LOW && forwardPointer == config.moduleForward) {
                    while (keyBackState == LOW) {
                        keyBackState = digitalRead(KEY_BACK);
                        delay(3);
                    }
                    PAGE_NAME = "BACKWARD";
                    UI_NAME   = "HinarUI";
                    --i_back;

                    curStep = totalStep - 1;
                    draw(1, false, false);

                    ++i_back;
                    currentState = MenuState::BackwardSelected;
                } else if (keyBackState == LOW && forwardPointer != config.moduleForward) {
                    while (keyBackState == LOW) {
                        keyBackState = digitalRead(KEY_BACK);
                        delay(3);
                    }
                    --forwardPointer;

                    curStep = totalStep - 1;
                    draw(1, false, true);

                    ++forwardPointer;
                    currentState = MenuState::Idle;
                }
                if (keyCycleState == LOW) {
                    while (keyCycleState == LOW) {
                        keyCycleState = digitalRead(KEY_CYCLE);
                        delay(3);
                    }
                    //Serial.println(i_back);
                    //Serial.println(module_UICORE_page);
                    if (i_back == backwardPointer) {
                        module_UICORE_page = (module_UICORE_page + 1) % module_UICORE_totalPages;
                    }
                }
                break;
            }
        }
    }
}

void Menu::draw(int offset, bool init, bool isForward) {
    frameBuffer.beginFrame();
    renderer.drawMenu(offset, init, isForward);
    frameBuffer.endFrame();
}

void Menu::drawFrame() {
    renderer.drawFrame();
}

void Menu::drawTopBar(String page, String ui) {
    renderer.drawTopBar(page, developerModeEnabled ? perf.fpsLabel() : ui);
}

String Menu::getFlowSpeed() {
    return animator.getFlowSpeed();
}

String Menu::stateToString() {
    switch (currentState) {
        case MenuState::Idle:              return "IDLE";
        case MenuState::Forward:           return "FORWARD";
        case MenuState::Backward:          return "BACKWARD";
        case MenuState::BackwardSelected:  return "BACKWARD_SELECTED";
        case MenuState::Module:            return "MODULE";
    }
    return "ERROR";
}

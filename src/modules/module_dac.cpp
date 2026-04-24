#include "Profile.h"
#include "HinarUI_Core.h"
#include "HinarUI/core/DacMath.h"
#include "modules/module_dac.h"

#include <math.h>

namespace {
constexpr int KEY_ACTIVE_LEVEL = LOW;
constexpr unsigned long DEBOUNCE_MS = 40;
constexpr unsigned long BACK_EXIT_MS = 650;
constexpr unsigned long CYCLE_FAST_START_MS = 450;
constexpr unsigned long CYCLE_FAST_REPEAT_MS = 90;

constexpr float OUTPUT_MIN_V = 0.0f;
constexpr float OUTPUT_MAX_V = 2.0f;

constexpr float STEP_PRESETS[] = {0.05f, 0.10f, 0.20f, 0.50f};
constexpr size_t STEP_COUNT = sizeof(STEP_PRESETS) / sizeof(STEP_PRESETS[0]);

struct ButtonState {
    int pin = -1;
    int stableState = HIGH;
    int lastReading = HIGH;
    unsigned long lastChangeMs = 0;
};

ButtonState btnEnter;
ButtonState btnCycle;
ButtonState btnOff;

float targetVoltage = 0.0f;
size_t stepIndex = 1;
bool outputEnabled = false;
bool displayDirty = true;
bool buttonsReady = false;

void initButton(ButtonState& button, int pin) {
    button.pin = pin;
    int reading = digitalRead(pin);
    button.stableState = reading;
    button.lastReading = reading;
    button.lastChangeMs = millis();
}

void ensureButtonsReady() {
    if (buttonsReady) {
        return;
    }
    initButton(btnEnter, KEY_ENTER);
    initButton(btnCycle, KEY_CYCLE);
    initButton(btnOff, KEY_OFF);
    buttonsReady = true;
}

bool buttonPressed(ButtonState& button) {
    int reading = digitalRead(button.pin);
    if (reading != button.lastReading) {
        button.lastChangeMs = millis();
        button.lastReading = reading;
    }

    if ((millis() - button.lastChangeMs) > DEBOUNCE_MS && reading != button.stableState) {
        button.stableState = reading;
        return button.stableState == KEY_ACTIVE_LEVEL;
    }

    return false;
}

bool cyclePressedOrRepeated() {
    static bool isPressed = false;
    static unsigned long pressStartMs = 0;
    static unsigned long lastRepeatMs = 0;

    bool active = digitalRead(KEY_CYCLE) == KEY_ACTIVE_LEVEL;
    unsigned long now = millis();
    if (active && !isPressed) {
        isPressed = true;
        pressStartMs = now;
        lastRepeatMs = now;
        return true;
    }
    if (active && isPressed && now - pressStartMs >= CYCLE_FAST_START_MS &&
        now - lastRepeatMs >= CYCLE_FAST_REPEAT_MS) {
        lastRepeatMs = now;
        return true;
    }
    if (!active) {
        isPressed = false;
    }
    return false;
}

float clampVoltage(float value) {
    return HinarUI::clampDacVoltage(value, OUTPUT_MIN_V, OUTPUT_MAX_V);
}

uint8_t voltageToDac(float volts) {
    return HinarUI::dacValueFromVoltage(volts, 3.3f, OUTPUT_MIN_V, OUTPUT_MAX_V);
}

void applyOutput() {
    uint8_t dacValue = outputEnabled ? voltageToDac(targetVoltage) : 0;
    dacWrite(HINARUI_DAC_PIN, dacValue);
}

bool backShortPressed() {
    static bool isPressed = false;
    static bool longPressSeen = false;
    static unsigned long pressStartMs = 0;

    bool active = digitalRead(KEY_BACK) == KEY_ACTIVE_LEVEL;
    unsigned long now = millis();
    if (active && !isPressed) {
        isPressed = true;
        longPressSeen = false;
        pressStartMs = now;
        return false;
    }
    if (active && isPressed) {
        if (now - pressStartMs >= BACK_EXIT_MS) {
            longPressSeen = true;
        }
        return false;
    }
    if (!active && isPressed) {
        unsigned long heldMs = now - pressStartMs;
        isPressed = false;
        return !longPressSeen && heldMs > DEBOUNCE_MS;
    }
    return false;
}

void drawRightAlignedText(const char* text, int right, int y) {
    int16_t x1 = 0;
    int16_t y1 = 0;
    uint16_t w = 0;
    uint16_t h = 0;
    display.getTextBounds(text, 0, y, &x1, &y1, &w, &h);
    display.setCursor(right - static_cast<int>(w), y);
    display.print(text);
}

void renderDacScreen() {
    frameBuffer.beginFrame();
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SELECTED_COLOR);
    SET_FONT_DEFAULT;

    char stepText[12];
    snprintf(stepText, sizeof(stepText), "%.2fV", STEP_PRESETS[stepIndex]);

    menu.drawTopBar("DAC-25", stepText);
    menu.drawFrame();

    const int rightEdge = 119;
    const int contentX = 8;

    float ratio = clampVoltage(targetVoltage) / OUTPUT_MAX_V;
    if (ratio < 0.0f) {
        ratio = 0.0f;
    }
    if (ratio > 1.0f) {
        ratio = 1.0f;
    }
    
    const int barX = contentX;
    const int barY = 22;
    const int barW = rightEdge - barX;
    const int barH = 12;
    const int barRadius = 5;
    display.drawRoundRect(barX, barY, barW, barH, barRadius, SELECTED_COLOR);

    int fillW = static_cast<int>((barW - 2) * ratio);
    if (fillW > 0) {
        int innerX = barX + 1;
        int innerY = barY + 1;
        int innerH = barH - 2;
        int cappedFillW = fillW > barW - 2 ? barW - 2 : fillW;
        display.fillRect(innerX, innerY, cappedFillW, innerH, SELECTED_COLOR);
        if (cappedFillW >= barRadius) {
            display.fillCircle(innerX + barRadius - 1, barY + barH / 2, barRadius - 1, SELECTED_COLOR);
        }
        if (cappedFillW >= barW - 2 - barRadius) {
            display.fillCircle(barX + barW - barRadius, barY + barH / 2, barRadius - 1, SELECTED_COLOR);
        }
    }

    display.setTextSize(2);
    display.setCursor(contentX, 44);
    display.print(targetVoltage, 2);
    display.print("V");

    const int gearMaxW = 36;
    const int gearUnitW = gearMaxW / static_cast<int>(STEP_COUNT);
    const int gearH = 5;
    const int gearX = rightEdge - gearMaxW;
    const int gearY = 43;
    const int gearW = gearUnitW * (static_cast<int>(stepIndex) + 1);
    display.fillRect(gearX, gearY, gearW, gearH, SELECTED_COLOR);

    const char* stateText = outputEnabled ? "ON" : "OFF";
    int16_t x1 = 0;
    int16_t y1 = 0;
    uint16_t stateW = 0;
    uint16_t stateH = 0;
    display.setTextSize(1);
    display.getTextBounds(stateText, 0, 53, &x1, &y1, &stateW, &stateH);
    display.setCursor(gearX, 53);
    display.print(stateText);

    display.setTextColor(SELECTED_COLOR);
    SET_FONT_DEFAULT;
    frameBuffer.endFrame();
}
}

bool DAC_Setup(void) {
#if defined(BOARD_ESP_WROVER_KIT)
    Serial.println(F("[INFO] Using ESP_WROVER_KIT DAC Module"));
    return ESP_WROVER_KIT_DAC_Setup();
#endif
    Serial.println(F("[ERROR] Board Not Supported DAC Module"));
    return false;
}

void module_DAC() {
    ensureButtonsReady();

    bool changed = false;
    if (cyclePressedOrRepeated()) {
        targetVoltage = clampVoltage(targetVoltage + STEP_PRESETS[stepIndex]);
        changed = true;
    }
    if (backShortPressed()) {
        targetVoltage = clampVoltage(targetVoltage - STEP_PRESETS[stepIndex]);
        changed = true;
    }
    if (buttonPressed(btnEnter)) {
        outputEnabled = !outputEnabled;
        changed = true;
    }
    if (buttonPressed(btnOff)) {
        stepIndex = (stepIndex + 1) % STEP_COUNT;
        changed = true;
    }

    if (changed) {
        applyOutput();
        displayDirty = true;
    }

    if (displayDirty || frameBuffer.needsFullRefreshHint()) {
        renderDacScreen();
        displayDirty = false;
    }
}

void module_DAC_ForceOff(void) {
    outputEnabled = false;
    applyOutput();
    displayDirty = true;
}

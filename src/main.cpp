#include <Wire.h>
#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "UI.h"

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
Menu menu;

const int KEY_LEFT = 18;
const int KEY_RIGHT = 19;
bool isAnimating = false;
unsigned long lastButtonPress = 0;
const unsigned long debounceDelay = 200;

void setup() {
    Serial.begin(115200);

    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
        Serial.println(F("SSD1306 allocation failed"));
        while (1);  // 停止程序
    }

    pinMode(KEY_LEFT, INPUT_PULLUP);
    pinMode(KEY_RIGHT, INPUT_PULLUP);

    menu.init();
}

void loop() {
    unsigned long currentTime = millis();

    // 防抖和动画状态检查
    if (!isAnimating && (currentTime - lastButtonPress) > debounceDelay) {
        int keyLeftState = digitalRead(KEY_LEFT);
        int keyRightState = digitalRead(KEY_RIGHT);

        // 处理左键按下，向左切换模块
        if (keyLeftState == LOW && menu.getModuleNum() > 0) {
            menu.animateSelection(false);  // false 表示向左
            menu.selectModule(menu.getModuleNum() - 1);
            isAnimating = true;
            lastButtonPress = currentTime;
        }
        // 处理右键按下，向右切换模块
        else if (keyRightState == LOW && menu.getModuleNum() < 4) {
            menu.animateSelection(true);  // true 表示向右
            menu.selectModule(menu.getModuleNum() + 1);
            isAnimating = true;
            lastButtonPress = currentTime;
        }
    }

    // 检查动画是否完成
    if (isAnimating && menu.isAnimationComplete()) {
        isAnimating = false;
    }

    menu.draw();  // 更新显示
}

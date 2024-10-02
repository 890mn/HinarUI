#include <Wire.h>
#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "UI.h"

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
Menu menu;

const int KEY_LEFT = 18; 
const int KEY_RIGHT = 19;

void setup() {
    Serial.begin(115200);

    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
        Serial.println(F("SSD1306 allocation failed"));
        while (1);
    }

    pinMode(KEY_LEFT, INPUT_PULLUP);
    pinMode(KEY_RIGHT, INPUT_PULLUP);

    menu.init();
}

void loop() {
    static unsigned long lastDebounceTime = 0;
    const unsigned long debounceDelay = 200;  // 去抖动延迟，200ms

    int keyLeftState = digitalRead(KEY_LEFT);
    int keyRightState = digitalRead(KEY_RIGHT);

    unsigned long currentTime = millis();

    // 按键去抖动处理
    if ((currentTime - lastDebounceTime) > debounceDelay) {
        if (keyLeftState == LOW) {
            if (menu.getModuleNum() > 0) {  // 防止越界
                menu.animateSelection(-1);  // 向左切换动画
                menu.selectModule(menu.getModuleNum() - 1);  // 更新选中的模块
            }
            lastDebounceTime = currentTime;  // 更新去抖时间
        }

        if (keyRightState == LOW) {
            if (menu.getModuleNum() < 4) {  // 防止越界，假设有5个模块
                menu.animateSelection(1);  // 向右切换动画
                menu.selectModule(menu.getModuleNum() + 1);  // 更新选中的模块
            }
            lastDebounceTime = currentTime;  // 更新去抖时间
        }
    }

    menu.draw();  // 持续绘制界面
}

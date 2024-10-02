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
    if (digitalRead(KEY_LEFT) == LOW) {
        if (menu.getModuleNum() > 0) { // 防止越界
            menu.animateSelection(-1); // 向左切换动画
            menu.selectModule(menu.getModuleNum() - 1); // 更新选中的模块
        }
        delay(20);
    }

    if (digitalRead(KEY_RIGHT) == LOW) {
        if (menu.getModuleNum() < 2) { // 防止越界，假设有3个场景
            menu.animateSelection(1); // 向右切换动画
            menu.selectModule(menu.getModuleNum() + 1); // 更新选中的模块
        }
        delay(20);
    }

    menu.draw();
}
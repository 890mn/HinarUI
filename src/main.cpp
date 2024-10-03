#include <Wire.h>
#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "UI.h"

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
Menu menu;

const int KEY_LEFT = 18;
const int KEY_RIGHT = 19;
bool isAnimating = false;   // 动画状态标志
unsigned long lastButtonPress = 0;  // 上次按钮按下的时间
const unsigned long debounceDelay = 200;  // 按键防抖延迟

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
    unsigned long currentTime = millis();  // 当前时间
    int keyLeftState = digitalRead(KEY_LEFT);   // 检测左键状态
    int keyRightState = digitalRead(KEY_RIGHT); // 检测右键状态

    // 检测按键是否被按下，且没有动画正在进行，并且防止按钮抖动
    if (!isAnimating && (currentTime - lastButtonPress) > debounceDelay) {
        if (keyLeftState == LOW) {
            // 如果当前不是第一个模块
            if (menu.getModuleNum() > 0) {
                menu.animateSelection(-1);  // 向左切换动画
                menu.selectModule(menu.getModuleNum() - 1);  // 更新选中的模块
                isAnimating = true;  // 动画开始
            }
            lastButtonPress = currentTime;  // 记录按键按下时间
        }
        else if (keyRightState == LOW) {
            // 如果当前不是最后一个模块
            if (menu.getModuleNum() < 4) {  // 假设有5个模块
                menu.animateSelection(1);  // 向右切换动画
                menu.selectModule(menu.getModuleNum() + 1);  // 更新选中的模块
                isAnimating = true;  // 动画开始
            }
            lastButtonPress = currentTime;  // 记录按键按下时间
        }
    }

    // 处理动画完成后，允许按键再次触发
    if (isAnimating) {
        // 检测动画是否完成
        if (menu.isAnimationComplete()) {
            isAnimating = false;  // 动画完成，重置标志
        }
    }

    menu.draw();  // 绘制界面
}

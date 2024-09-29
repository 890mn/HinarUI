#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "UI.h"

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
Menu menu;

const int encoderPinA = 5; // EC11旋转编码器的OUT引脚

int lastEncoderState = HIGH;

void setup() {
    Serial.begin(115200);

    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
        Serial.println(F("SSD1306 allocation failed"));
        while (1);
    }

    // 配置EC11的A引脚为输入模式
    pinMode(encoderPinA, INPUT_PULLUP);

    // 初始化菜单
    menu.init();
}

void loop() {
    int currentEncoderState = digitalRead(encoderPinA);

    if (currentEncoderState != lastEncoderState) {
        if (digitalRead(encoderPinA) == LOW) {
            // 检测到右转
            menu.selectModule((menu.getModuleNum() + 1) % 4);
            menu.animateSelection(1);  // 动画传入方向参数 1 表示向右
        } else {
            // 检测到左转
            menu.selectModule((menu.getModuleNum() - 1 + 4) % 4);
            menu.animateSelection(-1);  // 动画传入方向参数 -1 表示向左
        }
        delay(200); // 消除抖动
    }

    lastEncoderState = currentEncoderState;

    // 渲染菜单
    menu.draw();
}

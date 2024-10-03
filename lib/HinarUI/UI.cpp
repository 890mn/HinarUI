#include "UI.h"

extern Adafruit_SSD1306 display;

// 添加全局变量用于处理动画锁定和去抖
extern bool isAnimating;
const unsigned long debounceDelay = 200;  // 去抖动时间

void Menu::init() {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);

    drawTopBar();
    drawModuleIcons(0);
    drawFrame();
    display.display();
}

void Menu::drawFrame() {
    display.drawLine(0, 0, 127, 0, SELECTED_COLOR); // 顶1
    display.drawLine(0, 0, 0, 15, SELECTED_COLOR); // 左1
    display.drawLine(127, 0, 127, 5, SELECTED_COLOR); // 右1
    display.drawLine(0, 63, 30, 63, SELECTED_COLOR); // 底2-1
    display.drawLine(105, 63, 127, 63, SELECTED_COLOR); // 底2-2
    display.drawLine(0, 40, 0, 63, SELECTED_COLOR); // 左2
    display.drawLine(127, 16, 127, 63, SELECTED_COLOR); // 右2
    display.drawLine(0, 15, 43, 15, SELECTED_COLOR);  // 底1
    display.drawLine(98, 16, 127, 16, SELECTED_COLOR); // 顶2
}

void Menu::drawTopBar() {
    display.setCursor(4, 4);
    display.print(PAGE_NAME);

    int16_t x1, y1;
    uint16_t w, h;
    display.getTextBounds(UI_NAME, 0, 0, &x1, &y1, &w, &h);
    display.setCursor(SCREEN_WIDTH - w - 4, 4);
    display.print(UI_NAME);
}

void Menu::drawSelectedIcon(IconWithLabel& icon) {
    icon.width = 30;
    display.drawRect(icon.x, icon.y, icon.width, icon.height, SELECTED_COLOR);

    // 绘制类根号
    display.drawLine(icon.x + icon.width - 1, icon.y + icon.height - 1, icon.x + icon.width + 8, icon.y + icon.height - 10, SELECTED_COLOR);
    display.drawLine(icon.x + icon.width + 8, icon.y + icon.height - 10, icon.x + icon.width + 28, icon.y + icon.height - 10, SELECTED_COLOR);

    // 绘制文字
    display.setCursor(icon.x + icon.width + 10, icon.y + 10);
    display.print(icon.label);
}

void Menu::drawUnselectedIcon(IconWithLabel& icon) {
    icon.width = 20;

    int slantOffset = 10;
    display.drawLine(icon.x, icon.y, icon.x + icon.width, icon.y, SELECTED_COLOR);
    display.drawLine(icon.x - slantOffset, icon.y + icon.height, icon.x + icon.width - slantOffset, icon.y + icon.height, SELECTED_COLOR);
    display.drawLine(icon.x, icon.y, icon.x - slantOffset, icon.y + icon.height, SELECTED_COLOR);
    display.drawLine(icon.x + icon.width, icon.y, icon.x + icon.width - slantOffset, icon.y + icon.height, SELECTED_COLOR);
}

void Menu::drawModuleIcons(int offset) {
    int startX = 10 + offset; 
    bool next = false;

    for (int i = 0; i < 5; ++i) {
        int xPos = startX + i * 40;
        Icon.x = xPos;
        Icon.y = 25;
        Icon.label = modules[i];

        if (i == currentModulePointer) {  // 指针指向的模块为主选框
            drawSelectedIcon(Icon);
            next = true;
        } else {  
            Icon.x += next == true ? 40 : 0;  
            drawUnselectedIcon(Icon);
        }
    }
}

void Menu::draw() {
    display.clearDisplay();
    drawTopBar();
    drawModuleIcons(0);
    drawFrame();
    display.display();
}

int Menu::getModuleNum() {
    return currentModulePointer;
}

void Menu::selectModule(int index) {
    currentModulePointer = index;  // 更新指针到当前索引
}

void Menu::animateSelection(bool toRight) {
    isAnimating = true;
    animationStep = 0;  // 重置动画步数
    int offset = toRight ? 40 : -40;  // 根据方向调整偏移量

    while (animationStep < totalSteps) {  // 不超过总步数
        float progress = (float)animationStep / totalSteps;  // 计算当前动画进度
        int currentOffset = offset * progress;  // 动态计算当前的偏移量

        // 清屏并根据偏移量重新绘制模块
        display.clearDisplay();
        drawTopBar();
        drawModuleIcons(currentOffset); 
        drawFrame();
        display.display();

        animationStep++;
        delay(50);  // 控制每帧之间的延迟，确保动画流畅
    }

    // 动画结束，直接设置最终状态并绘制
    //if (toRight)
    //    currentModulePointer = (currentModulePointer + 1) % 5;
    //else 
    //    currentModulePointer = (currentModulePointer - 1 + 5) % 5;

    // 最后一步，直接绘制到最终状态
    //draw();
    
    isAnimating = false;
}

bool Menu::isAnimationComplete() {
    return animationStep >= totalSteps;  // 当步数超过总步数时，动画完成
}

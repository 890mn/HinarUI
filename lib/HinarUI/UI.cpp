#include "UI.h"

extern Adafruit_SSD1306 display;
extern bool             isAnimating;

//public
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

void Menu::draw(int offset) {
    display.clearDisplay();
    drawTopBar();
    drawModuleIcons(offset);
    drawFrame();
    display.display();
}

int Menu::getModuleNum() {
    return modulePointer;
}

void Menu::updatePointer(int index) {
    modulePointer = index;
}

void Menu::animateSelection(bool toRight) {
    isAnimating = true;
    animationStep = 0; 
    int offset = toRight ? 35 : -35;

    while (animationStep < totalSteps) {
        float progress = (float)animationStep / totalSteps;
        int currentOffset = offset * progress; 

        draw(currentOffset);

        animationStep++;
        delay(50);
    }
    isAnimating = false;
}

//Private
void Menu::drawFrame() {
    display.drawLine(0  , 0 , 127, 0 , SELECTED_COLOR); // TOP    1
    display.drawLine(0  , 0 , 0  , 15, SELECTED_COLOR); // LEFT   1
    display.drawLine(127, 0 , 127, 5 , SELECTED_COLOR); // RIGHT  1
    display.drawLine(0  , 15, 43 , 15, SELECTED_COLOR); // BOTTOM 1

    display.drawLine(98 , 16, 127, 16, SELECTED_COLOR); // TOP    2
    display.drawLine(0  , 40, 0  , 63, SELECTED_COLOR); // LEFT   2
    display.drawLine(127, 16, 127, 63, SELECTED_COLOR); // RIGHT  2
    display.drawLine(0  , 63, 30 , 63, SELECTED_COLOR); // BOTTOM 2-1
    display.drawLine(105, 63, 127, 63, SELECTED_COLOR); // BOTTOM 2-2
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

    // ROOT
    display.drawLine(icon.x + icon.width - 1, icon.y + icon.height - 1, icon.x + icon.width + 8, icon.y + icon.height - 10, SELECTED_COLOR);
    display.drawLine(icon.x + icon.width + 8, icon.y + icon.height - 10, icon.x + icon.width + 28, icon.y + icon.height - 10, SELECTED_COLOR);

    // NAME
    display.setCursor(icon.x + icon.width + 10, icon.y + 10);
    display.print(icon.label);
}

void Menu::drawUnselectedIcon(IconWithLabel& icon) {
    icon.width = 20;

    display.drawLine(icon.x, icon.y, icon.x + icon.width, icon.y, SELECTED_COLOR);
    display.drawLine(icon.x - UNSELECTED_OFFSET, icon.y + icon.height, icon.x + icon.width - UNSELECTED_OFFSET, icon.y + icon.height, SELECTED_COLOR);
    display.drawLine(icon.x, icon.y, icon.x - UNSELECTED_OFFSET, icon.y + icon.height, SELECTED_COLOR);
    display.drawLine(icon.x + icon.width, icon.y, icon.x + icon.width - UNSELECTED_OFFSET, icon.y + icon.height, SELECTED_COLOR);
}

void Menu::drawModuleIcons(int offset) {
    int startX = 10 + offset; 
    bool next = false;
    bool prev = false;

    for (int i = 0; i < 5; ++i) {
        int xPos = startX + i * 40;
        Icon.x = xPos;
        Icon.y = 25;
        Icon.label = modules[i];

        // 判断当前模块是否为主选模块
        if (i == modulePointer) {  
            // 当前模块为主选模块，进行动画处理
            if (animationStep < totalSteps / 2) {
                drawSelectedIcon(Icon);  // 主选变为未选的前半段动画
                wordShrink(Icon);        // 隐藏文字和类根号
            } else {
                rectTrans(Icon);         // 未选变为主选的后半段动画
            }
            next = true;
        }
        // 判断左侧未选模块变为主选模块的情况
        else if (i == (modulePointer - 1 + 5) % 5 && !next) {
            if (animationStep < totalSteps / 2) {
                rectTrans(Icon);  // 未选模块变为主选块的动画（平行四边形变正方形）
            } else {
                drawSelectedIcon(Icon);  // 动画完成后变为主选模块
                wordGrow(Icon);          // 逐步生成类根号和文字
            }
            prev = true;
        }
        else {
            // 处理未选模块逻辑
            Icon.width = 20;  // 确保未选模块是平行四边形的宽度
            if (next) {
                Icon.x += 35;  // 主选右侧的未选模块位移
            }
            if (prev) {
                Icon.x -= 35;  // 主选左侧的未选模块位移
            }
            drawUnselectedIcon(Icon);
        }
    }
}

void Menu::wordShrink(IconWithLabel& icon) {
    int wordStep = STEP_COUNT / 2;
    int shrinkI = animationStep;
    float shrinkX = 35.0 / wordStep;  // 缩减量

    // 修改覆盖区域，确保完全隐藏类根号
    display.fillRect(icon.x + 30 + shrinkX * (wordStep - shrinkI), 
                     icon.y + 10, 
                     35 - shrinkX * shrinkI,  // 覆盖剩余部分
                     25, UNSELECTED_COLOR);
}

void Menu::wordGrow(IconWithLabel& icon) {
    int wordStep = STEP_COUNT / 2;
    int growI = animationStep - totalSteps / 2;
    float growX = 35.0 / wordStep;

    if (modules[modulePointer] == icon.label) {
        int startX = icon.x + icon.width;  // 正方形的右下角
        int startY = icon.y + icon.height;

        int endX1 = startX + growX * growI;
        int endY1 = startY - growX * growI;

        display.drawLine(startX, startY, endX1, endY1, SELECTED_COLOR);  // 斜线
        if (growI > wordStep / 2) {
            int endX2 = endX1 + growX * (growI - wordStep / 2);
            display.drawLine(endX1, endY1, endX2, endY1, SELECTED_COLOR);  // 水平线
        }

        // 显示文字
        if (growI == wordStep - 1) {
            display.setCursor(icon.x + icon.width + 5, icon.y + 5);
            display.setTextColor(SELECTED_COLOR);
            display.print(icon.label);
        }
    }
}

void Menu::rectTrans(IconWithLabel& icon) {
    int rectStep = STEP_COUNT / 2;
    int transI = animationStep - totalSteps / 2;
    float transX = 10.0 / rectStep;  // 每一步的缩放量

    int x1 = icon.x;
    int y1 = icon.y;
    int x2 = icon.x + icon.width;
    int y2 = icon.y;
    int x3 = icon.x + icon.width;
    int y3 = icon.y + icon.height;
    int x4 = icon.x;
    int y4 = icon.y + icon.height;

    // 确保右侧宽度逐步缩减，从30缩到20
    int newX3 = x3 - (transX * transI);  
    int newX4 = x4 - (transX * transI);

    display.drawLine(x1, y1, x2, y2, SELECTED_COLOR); 
    display.drawLine(x2, y2, newX3, y3, SELECTED_COLOR); 
    display.drawLine(newX3, y3, newX4, y4, SELECTED_COLOR); 
    display.drawLine(newX4, y4, x1, y1, SELECTED_COLOR);
}

#include "UI.h"

extern Adafruit_SSD1306 display;
extern bool             isAnimating;

//public
void Menu::init() {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SELECTED_COLOR);
    display.setCursor(0, 0);

    drawTopBar();
    drawModuleIcons(0, true);
    drawFrame();
    display.display();
}

void Menu::draw(int offset) {
    display.clearDisplay();
    drawTopBar();
    drawModuleIcons(offset, false);
    drawFrame();
    display.display();
}

void Menu::animateSelection(bool toRight) {
    if ((modulePointer == 0 && toRight) || (modulePointer == MODULE_MAX - 1 && !toRight)) {
        reboundAnimation();
        //display.setCursor(4, 4);
        //display.print("ERROR");
        //display.display();
        isAnimating = false;
        return;
    }

    isAnimating = true;
    animationStep = 0; 
    int offset = toRight ? 35 : -35;
    int currentOffset = offset / totalStep; 

    while (animationStep < totalStep) {
        draw(currentOffset);
        animationStep++;
        delay(50);
    }
    toRight ? --modulePointer : ++modulePointer;
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

void Menu::drawModuleIcons(int offset, bool init) {
    IconTrans = Icon;
    IconTrans.x += offset; 
    Icon = IconTrans;
    bool next = false;
    //bool prev = false;

    for (int i = 0; i < MODULE_MAX; ++i) {
        IconTrans.label = modules[i];

        if (i == modulePointer + 1 && init) {
            IconTrans.x += 40;
            //prev = true;
            continue;
        }

        // MainModule
        if (i == modulePointer) {  
            //display.drawCircle(IconTrans.x + 15, IconTrans.y + 15, 2, SELECTED_COLOR); //Debug
            if (animationStep < totalStep / 2) {
                drawSelectedIcon(IconTrans);  // module
                wordShrink(IconTrans);        // shrink word-root
            } else {
                //if (prev) IconTrans.x += 40;
                rectTransPall(IconTrans);         // rect->pall
            }
        }
        // MainModule right first
        else if (i == modulePointer + 1) {
            int tmp = IconTrans.x + 10;
            IconTrans.x += (totalStep - animationStep) * (30 / totalStep);

            if (animationStep < totalStep / 2) {
                pallTransRect(IconTrans);  // pall->rect
            } else {
                IconTrans.height = IconTrans.width = 30;
                display.drawRect(IconTrans.x, IconTrans.y, IconTrans.width, IconTrans.height, SELECTED_COLOR); // module
                wordGrow(IconTrans);          // grow word-root
            }
            next = true;
            IconTrans.x = tmp;
        }
        else {
            IconTrans.width = 20;  // 确保未选模块是平行四边形的宽度
            if (next) {
                IconTrans.x += 40;  // 主选右侧的未选模块位移
            }
            //if (prev) {
            //    Icon.x -= 35;  // 主选左侧的未选模块位移
            //}
            drawUnselectedIcon(IconTrans);
        }
        //if (i == 0) Icon = IconTrans;
        IconTrans.x += 35;
    }
}

void Menu::wordShrink(IconWithLabel& icon) {
    int wordStep = STEP_COUNT / 2;
    int shrinkI = animationStep;
    float shrinkX = 35.0 / wordStep;

    display.fillRect(icon.x + 30 + shrinkX * (wordStep - shrinkI), icon.y + 10, 
                     shrinkX * shrinkI, 20, UNSELECTED_COLOR);
}

void Menu::wordGrow(IconWithLabel& icon) {
    int wordStep = STEP_COUNT / 2;
    int growI = animationStep - totalStep / 2;
    int progress = growI;

    int startX = icon.x + icon.width - 1;
    int startY = icon.y + icon.height - 1;
    
    int endX1 = icon.x + icon.width + 8;
    int endY1 = icon.y + icon.height - 10;

    int currentEndX1 = startX + 18 / wordStep * progress;  // 9 * 2
    int currentEndY1 = startY - 18 / wordStep * progress;

    int horizontalProgress = growI - wordStep / 2;
    int currentEndX2 = endX1 + 60 / wordStep * horizontalProgress; 

    if (modules[modulePointer + 1] == icon.label) {
        if (growI >= wordStep / 2) {
            display.drawLine(startX, startY, endX1, endY1, SELECTED_COLOR); 
            display.drawLine(endX1, endY1, currentEndX2, endY1, SELECTED_COLOR);
        }
        else {
            display.drawLine(startX, startY, currentEndX1, currentEndY1, SELECTED_COLOR); 
        }

        if (animationStep > totalStep - 4) {
            display.setCursor(icon.x + icon.width + 10, icon.y + 10); 
            display.setTextColor(SELECTED_COLOR);
            display.print(icon.label);
        }
    }
}

void Menu::pallTransRect(IconWithLabel& icon) {
    int rectStep = STEP_COUNT / 2;
    int transI = animationStep - totalStep / 2;
    float transX = 10.0 / rectStep;  // 每一步的缩放量

    int x1 = icon.x;
    int y1 = icon.y;
    int x2 = icon.x + icon.width;
    int y2 = icon.y;
    int x3 = icon.x + icon.width;
    int y3 = icon.y + icon.height;
    int x4 = icon.x;
    int y4 = icon.y + icon.height;

    // 逐步调整左上角（x1）和右下角（x3）的横坐标
    int newX1 = x1 - (transX * transI);  // 左上角向左拉
    int newX3 = x3 + (transX * transI);  // 右下角向右拉

    // 绘制平行四边形逐步变为正方形的过程
    display.drawLine(newX1, y1, x2, y2, SELECTED_COLOR);  // 上边
    display.drawLine(x2, y2, newX3, y3, SELECTED_COLOR);  // 右边
    display.drawLine(newX3, y3, x4, y4, SELECTED_COLOR);  // 下边
    display.drawLine(x4, y4, newX1, y1, SELECTED_COLOR);  // 左边
}

void Menu::rectTransPall(IconWithLabel& icon) {
    int rectStep = STEP_COUNT / 2;
    int transI = animationStep - totalStep / 2;

    int x1 = icon.x;
    int y1 = icon.y;
    int x2 = icon.x + 30;
    int y2 = icon.y;
    int x3 = icon.x + 25;
    int y3 = icon.y + icon.height;
    int x4 = icon.x;
    int y4 = icon.y + icon.height;

    // 确保右侧宽度逐步缩减，从30缩到20
    int newX2 = x2 - (10.0 / rectStep) * transI;
    int newX3 = x3 - (15.0 / rectStep) * transI;
    int newX4 = x4 - (10.0 / rectStep) * transI;

    display.drawLine(x1, y1, newX2, y2, SELECTED_COLOR); 
    display.drawLine(newX2, y2, newX3, y3, SELECTED_COLOR); 
    display.drawLine(newX3, y3, newX4, y4, SELECTED_COLOR); 
    display.drawLine(newX4, y4, x1, y1, SELECTED_COLOR);
}

void Menu::reboundAnimation() {
    // 预留回弹动画的逻辑，可以在这里实现回弹效果
}

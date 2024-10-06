#include "UI.h"

extern Adafruit_SSD1306 display;
extern bool             isAnimating;

//public
void Menu::init() {
    display.setTextSize(1);
    display.setTextColor(SELECTED_COLOR);
    display.setCursor(0, 0);
    draw(0, true);
}

void Menu::draw(int offset, bool init) {
    display.clearDisplay();
    drawTopBar();
    drawModules(offset, init);
    drawFrame();
    display.display();
}

void Menu::renderStart() {
    curStep = 0;

    while (curStep < totalStep) {
        draw(-45 / totalStep, false);
        curStep++;
        delay(flowSpeed);
    }

    if (modulePointer == MODULE_MAX - 1) {
        modulePointer = 0;
        Icon = {.x = 10, .y = 25, .width = 20, .height = 30};                                                                                                                                   
        draw(0, true);
        isAnimating = false;
        return;
    }
    ++modulePointer;
    isAnimating = false;
}

//Private
float Menu::easeInOut(float t) {
    if (t < 0.4) {
        return 2 * t * t;
    } else {
        return -1 + (4 - 2 * t) * t;
    }
}

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

void Menu::drawSeleModule(IconWithLabel& icon) {
    icon.width = 30;
    display.drawRoundRect(icon.x, icon.y, icon.width, icon.height, RADIUS_RECT, SELECTED_COLOR);

    // 1 - ROOT
    display.drawLine(icon.x + icon.width - 1, icon.y + icon.height - 1,
                     icon.x + icon.width + 8, icon.y + icon.height - 10, SELECTED_COLOR);
    display.drawLine(icon.x + icon.width + 8, icon.y + icon.height - 10,
                     icon.x + icon.width + 28, icon.y + icon.height - 10, SELECTED_COLOR);

    // 2 - NAME
    display.setCursor(icon.x + icon.width + 10, icon.y + 10);
    display.print(icon.label);
}

void Menu::drawUnseleModule(IconWithLabel& icon) {
    icon.width = 20;

    // 1 - LEFT-TOP
    display.drawCircleHelper(icon.x + 2, icon.y + 2,
                             RADIUS_PALL, 1, SELECTED_COLOR);

    // 2 - LEFT-TOP -> LEFT-BOTTOM
    display.drawLine(icon.x, icon.y + 2,
                     icon.x - 10, icon.y + icon.height - 2, SELECTED_COLOR);          

    // 2 - LEFT-TOP -> RIGHT-TOP
    display.drawLine(icon.x + 2, icon.y,
                     icon.x + icon.width - 2, icon.y, SELECTED_COLOR);

    // 3 - LEFT-BOTTOM
    display.drawCircleHelper(icon.x - 10 + 2, icon.y + icon.height - 2,
                             RADIUS_PALL, 8, SELECTED_COLOR);

    // 3 - RIGHT-TOP
    display.drawCircleHelper(icon.x + icon.width - 2, icon.y + 2,
                             RADIUS_PALL, 2, SELECTED_COLOR);          

    // 4 - RIGHT-TOP -> RIGHT-BOTTOM
    display.drawLine(icon.x + icon.width, icon.y + 2,
                     icon.x + icon.width - 10, icon.y + icon.height - 2, SELECTED_COLOR);

    // 4 - LEFT-BOTTOM -> RIGHT-BOTTOM
    display.drawLine(icon.x - 10 + 2, icon.y + icon.height,
                     icon.x + icon.width - 10 - 2, icon.y + icon.height, SELECTED_COLOR);

    // 5 - RIGHT-BOTTOM
    display.drawCircleHelper(icon.x + icon.width - 10 - 2, icon.y + icon.height - 2,
                             RADIUS_PALL, 4, SELECTED_COLOR);     
}

void Menu::drawModules(int offset, bool init) {
    IconTrans = Icon;
    IconTrans.x += offset; 
    Icon = IconTrans;
    bool next = false;

    if (init) {
        for (int i = 0; i < MODULE_MAX; ++i) {
            IconTrans.label = modules[i];
            if (i == 0) {
                drawSeleModule(IconTrans);
                IconTrans.x += 75;
            } else {
                drawUnseleModule(IconTrans);
                IconTrans.x += 35;
            }
        } 
        return;
    }

    for (int i = 0; i < MODULE_MAX; ++i) {
        IconTrans.label = modules[i];

        // MainModule
        if (i == modulePointer) {  
            //MainModule Debug
            //display.drawCircle(IconTrans.x + 15, IconTrans.y + 15, 2, SELECTED_COLOR); 

            if (curStep < totalStep / 2) {
                drawSeleModule(IconTrans);    // module-selected
                wordShrink(IconTrans);        // shrink word-root
            } else {
                rectTransPall(IconTrans);     // rect->pall
            }
        }

        // MainModule right first
        else if (i == modulePointer + 1) {
            int tmp = IconTrans.x + 10;
            IconTrans.x += (totalStep - curStep) * (30 / totalStep);

            if (curStep < totalStep / 2) {
                pallTransRect(IconTrans);     // pall->rect
            } else {
                IconTrans.height = IconTrans.width = 30;
                display.drawRoundRect(IconTrans.x, IconTrans.y,
                                      IconTrans.width, IconTrans.height, RADIUS_RECT, SELECTED_COLOR); 
                wordGrow(IconTrans);         // grow word-root

                if (i == MODULE_MAX - 1 && curStep > totalStep - 2) {
                    display.drawCircleHelper(24, 60, 65, 0x2, SELECTED_COLOR);
                    display.fillRect(47, 1, 28, 20, UNSELECTED_COLOR);

                    display.setCursor(44, 21);
                    display.print(modules[0]);
                    display.setCursor(53, 52);
                    display.print(modules[MODULE_MAX-2]);
                }
            }
            next = true;
            IconTrans.x = tmp;
        }
        else {
            if (next) IconTrans.x += 40;
            drawUnseleModule(IconTrans);
        }
        IconTrans.x += 35;
    }
}

void Menu::wordShrink(IconWithLabel& icon) {
    int wordStep = STEP_COUNT / 2;
    int shrinkI = curStep;
    float shrinkX = 35.0 / wordStep;

    display.fillRect(icon.x + 30 + shrinkX * (wordStep - shrinkI), icon.y + 10, 
                     shrinkX * shrinkI, 20, UNSELECTED_COLOR);
}

void Menu::wordGrow(IconWithLabel& icon) {
    int wordStep = STEP_COUNT / 2;
    int growI = curStep - totalStep / 2;

    int startX = icon.x + icon.width - 1;
    int startY = icon.y + icon.height - 1;
    
    int endX1 = icon.x + icon.width + 8;
    int endY1 = icon.y + icon.height - 10;

    int currentEndX1 = startX + 18 / wordStep * growI;  // 9 * 2
    int currentEndY1 = startY - 18 / wordStep * growI;

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

        if (curStep > totalStep - 4) {
            display.setCursor(icon.x + icon.width + 10, icon.y + 10); 
            display.setTextColor(SELECTED_COLOR);
            display.print(icon.label);
        }
    }
}

void Menu::pallTrans(IconWithLabel& icon, int leftTopX, int rightTopX, int rightBottomX, int leftBottomX) {
    // 1 - LEFT-TOP
    display.drawCircleHelper(leftTopX + RADIUS_PALL, icon.y + RADIUS_PALL,
                             RADIUS_PALL, 1, SELECTED_COLOR);

    // 2 - LEFT-BOTTOM -> LEFT-TOP
    display.drawLine(leftBottomX, icon.y + icon.height - RADIUS_PALL,
                     leftTopX, icon.y + RADIUS_PALL, SELECTED_COLOR);    

    // 2 - LEFT-TOP -> RIGHT-TOP
    display.drawLine(leftTopX + RADIUS_PALL, icon.y,
                     rightTopX - RADIUS_PALL, icon.y, SELECTED_COLOR); 

    // 3 - RIGHT-TOP
    display.drawCircleHelper(rightTopX - RADIUS_PALL, icon.y + RADIUS_PALL,
                             RADIUS_PALL, 2, SELECTED_COLOR);

    // 3 - LEFT-BOTTOM
    display.drawCircleHelper(leftBottomX + RADIUS_PALL, icon.y + icon.height - RADIUS_PALL,
                             RADIUS_PALL, 8, SELECTED_COLOR);

    // 4 - RIGHT-TOP -> RIGHT-BOTTOM
    display.drawLine(rightTopX, icon.y + RADIUS_PALL,
                             rightBottomX, icon.y + icon.height - RADIUS_PALL, SELECTED_COLOR); 

    // 4 - RIGHT-BOTTOM -> LEFT-BOTTOM
    display.drawLine(rightBottomX - RADIUS_PALL, icon.y + icon.height,
                             leftBottomX + RADIUS_PALL, icon.y + icon.height, SELECTED_COLOR); 

    // 5 - RIGHT-BOTTOM
    display.drawCircleHelper(rightBottomX - RADIUS_PALL, icon.y + icon.height - RADIUS_PALL,
                             RADIUS_PALL, 4, SELECTED_COLOR);
}

void Menu::pallTransRect(IconWithLabel& icon) {
    int rectStep = STEP_COUNT / 2;
    int transI = curStep - totalStep / 2;
    float transX = 10.0 / rectStep;

    int leftTopX = icon.x - (transX * transI);
    int rightTopX = icon.x + icon.width;
    int rightBottomX = icon.x + icon.width + (transX * transI);
    int leftBottomX = icon.x;

    pallTrans(icon, leftTopX, rightTopX, rightBottomX, leftBottomX);
}

void Menu::rectTransPall(IconWithLabel& icon) {
    int rectStep = STEP_COUNT / 2;
    float progress = (float)(curStep - totalStep / 2) / rectStep;
    float easedProgress = easeInOut(progress);

    int leftTopX = icon.x;
    int rightTopX = icon.x + 30 - 10.0 * easedProgress;
    int rightBottomX = icon.x + 25 - 15.0 * easedProgress;
    int leftBottomX = icon.x - 10.0 * easedProgress;

    pallTrans(icon, leftTopX, rightTopX, rightBottomX, leftBottomX);
}

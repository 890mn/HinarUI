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

    for (int i = 0; i < 5; ++i) {
        int xPos = startX + i * 40;
        Icon.x = xPos;
        Icon.y = 25;
        Icon.label = modules[i];

        if (i == modulePointer) { 
            drawSelectedIcon(Icon);
            next = true;
        } else {  
            Icon.x += next == true ? 35 : 0;  
            drawUnselectedIcon(Icon);
        }
    }
}

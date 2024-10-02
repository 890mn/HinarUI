#include "UI.h"

extern Adafruit_SSD1306 display;

void Menu::init() {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);

    drawTopBar();
    drawModuleIcons();
    drawFrame();
    display.display();
}

void Menu::drawFrame() {
    display.drawLine(0, 0, 127, 0, SELECTED_COLOR); //顶1
    display.drawLine(0, 0, 0, 15, SELECTED_COLOR); //左1
    display.drawLine(127, 0, 127, 5, SELECTED_COLOR); //右1

    display.drawLine(0, 63, 30, 63, SELECTED_COLOR); //底2-1
    display.drawLine(105, 63, 127, 63, SELECTED_COLOR); //底2-2
    display.drawLine(0, 40, 0, 63, SELECTED_COLOR); //左2
    display.drawLine(127, 16, 127, 63, SELECTED_COLOR); //右2

    display.drawLine(0, 15, 43, 15, SELECTED_COLOR);  //底1
    display.drawLine(98, 16, 127, 16, SELECTED_COLOR); //顶2
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

void Menu::drawSelectedIcon(int x, int y) {
    x += 5;
    y += 5;

    display.drawRect(x, y, 30, 30, SELECTED_COLOR);
    display.drawLine(x + 29, y + 29, x + 38, y + 20, SELECTED_COLOR); // 斜线
    display.drawLine(x + 38, y + 20, x + 58, y + 20, SELECTED_COLOR); // 水平线

    display.setCursor(x + 40, y + 10); 
    display.print(modules[selectedModule]);
}

void Menu::drawUnselectedIcon(int x, int y) {
    x += 5;
    y += 5;

    int height = 30;
    int width = 20;
    int slantOffset = 10;

    display.drawLine(x, y, x + width, y, SELECTED_COLOR);                       
    display.drawLine(x - slantOffset, y + height, x + width - slantOffset, y + height, SELECTED_COLOR); 
    display.drawLine(x, y, x - slantOffset, y + height, SELECTED_COLOR);        
    display.drawLine(x + width, y, x + width - slantOffset, y + height, SELECTED_COLOR); 
}

void Menu::drawModuleIcons() {
    int startX = 64;  // 起始X基于中心点计算
    int spacing = 40; // 每个模块之间的距离
    int yPos = 20;    // 固定Y位置

    for (int i = -2; i <= 2; i++) {
        int moduleIndex = (selectedModule + i + 5) % 5;  // 确保索引在0-4范围内循环
        int x = startX + i * spacing;                    // 基于相对中心位置计算X

        if (i == 0) {
            drawSelectedIcon(x - 15, yPos); // 中间主选块，以中心点为基准
        } else {
            drawUnselectedIcon(x - 10, yPos); // 其他未选模块
        }
    }
}

void Menu::draw() {
    display.clearDisplay(); 
    drawTopBar();
    drawModuleIcons();
    drawFrame();
    display.display();
}

void Menu::selectModule(int index) {
    selectedModule = index;
}

void Menu::animateSelection(int direction) {
    int steps = 10; // 动画分成10步
    int delta = (direction == 1) ? 1 : -1; // 控制方向

    int selectedStartX = 64; // 主选块中心起始X位置
    int spacing = 40;        // 每个模块之间的距离

    for (int i = 0; i < steps; i++) {
        display.clearDisplay();
        drawTopBar();

        // 线性插值移动位置和形状变化
        int stepOffset = (i * spacing) / steps;
        int selectedX = selectedStartX + delta * stepOffset;

        // 主选块横向压缩并斜向拉伸为平行四边形
        for (int j = -2; j <= 2; j++) {
            int x = selectedX + j * spacing;
            if (j == 0) {
                drawIconTransition(x - 15, 30 - (i * 2), true);  // 正方形变为平四
            } else if (j == delta) {
                drawIconTransition(x - 10, 20 + (i * 2), false); // 平四变为正方形
            } else {
                drawUnselectedIcon(x - 10, 20);                 // 其他未选模块移动
            }
        }

        drawFrame();
        display.display();
        delay(50); // 每步动画间隔
    }

    // 更新最终状态
    selectedModule = (selectedModule + direction + 5) % 5; // 保证模块在0-4之间循环
    draw();
}

void Menu::drawIconTransition(int x, int size, bool isSquare) {
    int y = 20; // 固定Y位置
    if (isSquare) {
        display.drawRect(x, y, size, size, SELECTED_COLOR);
        display.setCursor(x + size + 10, y + 10);
        display.print(modules[selectedModule]);
    } else {
        int slantOffset = 10;
        int height = 40;
        int width = size;

        display.drawLine(x, y, x + width, y, SELECTED_COLOR);
        display.drawLine(x - slantOffset, y + height, x + width - slantOffset, y + height, SELECTED_COLOR);
        display.drawLine(x, y, x - slantOffset, y + height, SELECTED_COLOR);
        display.drawLine(x + width, y, x + width - slantOffset, y + height, SELECTED_COLOR);
    }
}

int Menu::getModuleNum() {
    return selectedModule;
}

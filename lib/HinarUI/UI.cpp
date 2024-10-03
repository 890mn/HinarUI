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
    drawModuleIcons();
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
    int startX = 10;  // 起始位置

    for (int i = 0; i < 5; ++i) {
        int xPos = startX + i * 40;
        if (i == selectedModule) {
            drawSelectedIcon(xPos, 20);
        } else {
            drawUnselectedIcon(xPos, 20);
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

void Menu::animateMainToUnselected(int index, float progress) {
    int xPos = 10 + index * 40;

    // 确保动画只进行一次
    if (progress < 0.5) {
        drawIconTransition(xPos, progress * 2, 1, false); // 正方形线性变为平行四边形
        animateModuleCollapse(index);  // 收回类根号和文字
    } else {
        drawUnselectedIcon(xPos, 20);  // 渲染为平行四边形
    }
}

void Menu::animateUnselectedToMain(int index, float progress) {
    int xPos = 10 + index * 40;

    // 确保动画只进行一次
    if (progress < 0.5) {
        drawIconTransition(xPos, progress * 2, 1, true);  // 平行四边形变为正方形
    } else {
        animateModuleExpand(index);  // 展开类根号和文字
    }
}

void Menu::drawIconTransition(int x, float progress, int totalSteps, bool toSquare) {
    int startWidth = toSquare ? 20 : 30;
    int endWidth = toSquare ? 30 : 20;

    // 平滑过渡，减少清屏
    int currentWidth = startWidth + (endWidth - startWidth) * progress;

    int height = 30;
    int slantOffset = 10 * (1.0f - progress);  // 动态调整斜边偏移量

    display.drawLine(x, 20, x + currentWidth, 20, SELECTED_COLOR);
    display.drawLine(x - slantOffset, 50, x + currentWidth - slantOffset, 50, SELECTED_COLOR);
    display.drawLine(x, 20, x - slantOffset, 50, SELECTED_COLOR);
    display.drawLine(x + currentWidth, 20, x + currentWidth - slantOffset, 50, SELECTED_COLOR);
}

void Menu::animateModuleCollapse(int index) {
    int xPos = 10 + index * 40;
    display.fillRect(xPos + 40, 20, 30, 30, UNSELECTED_COLOR);  // 清除文字和类根号
}

void Menu::animateModuleExpand(int index) {
    int xPos = 10 + index * 40;
    drawSelectedIcon(xPos, 20);  // 绘制模块
}

int Menu::getModuleNum() {
    return selectedModule;
}

void Menu::animateSelection(int direction) {
    animationStep = 0;  // 重置动画步数
    
    while (animationStep < totalSteps) {
        // 执行动画的每一步
        drawIconTransition(/*x=*/0, animationStep, totalSteps, direction == 1);
        animationStep++;
        delay(50);  // 控制动画帧的延迟
    }
}

bool Menu::isAnimationComplete() {
    return animationStep >= totalSteps;  // 当步数超过总步数时，动画完成
}
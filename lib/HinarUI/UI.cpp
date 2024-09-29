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
    // 调整坐标，使图标避免覆盖边框
    x += 5;
    y += 5;

    // 绘制主选中的正方形图标框
    display.drawRect(x, y, 30, 30, SELECTED_COLOR);

    // 绘制类根号图形，先斜上45度再水平延伸
    display.drawLine(x + 29, y + 29, x + 38, y + 20, SELECTED_COLOR); // 斜线
    display.drawLine(x + 38, y + 20, x + 58, y + 20, SELECTED_COLOR); // 水平线

    // 在类根号上方绘制模块名称
    display.setCursor(x + 40, y + 10); 
    display.print(modules[selectedModule]);
}

void Menu::drawUnselectedIcon(int x, int y) {
    // 调整未选中图标的起始坐标，避免与边框重叠
    x += 5;
    y += 5;

    // 绘制未选中的平行四边形图标
    int height = 30;
    int width = 20;
    int slantOffset = 10; // 斜边的偏移量

    // 反转方向绘制平行四边形
    display.drawLine(x, y, x + width, y, SELECTED_COLOR);                       // 顶边
    display.drawLine(x - slantOffset, y + height, x + width - slantOffset, y + height, SELECTED_COLOR); // 底边
    display.drawLine(x, y, x - slantOffset, y + height, SELECTED_COLOR);        // 左斜边
    display.drawLine(x + width, y, x + width - slantOffset, y + height, SELECTED_COLOR); // 右斜边
}


void Menu::drawModuleIcons() {
    // 主选中的模块图标
    drawSelectedIcon(10, 20);  // 选中模块适当偏移避免与边框冲突

    // 绘制右侧的未选中模块图标
    for (int i = 0; i < 4; i++) {
        int xOffset = 90 + i * 30;  // 调整xOffset, 避免与右侧边框冲突
        drawUnselectedIcon(xOffset, 20);  // y坐标微调，保证统一高度
    }
}

void Menu::draw() {
    display.clearDisplay(); // 每次绘制时先清除屏幕
    drawTopBar();
    drawModuleIcons();
    drawFrame();
    display.display();
}

void Menu::selectModule(int index) {
    selectedModule = index;
    //animateSelection();
}

void Menu::animateSelection(int direction) {

}

int Menu::getModuleNum() {
    return selectedModule;
}
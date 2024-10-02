#include "UI.h"

extern Adafruit_SSD1306 display;

// 添加全局变量用于处理动画锁定和去抖
bool isAnimating = false;
unsigned long lastButtonPress = 0;
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

// 改进后的 animateSelection 函数，确保一次按键只会执行一次模块切换
void Menu::animateSelection(int direction) {
    if (isAnimating) {
        return;  // 动画正在进行，阻止新的操作
    }

    // 去抖动处理，防止一次按键触发多次
    unsigned long currentTime = millis();
    if (currentTime - lastButtonPress < debounceDelay) {
        return;  // 如果按键在去抖时间内再次被按下，忽略
    }
    lastButtonPress = currentTime;

    // 限制模块索引范围
    int nextModule = selectedModule + direction;
    if (nextModule < 0 || nextModule >= 5) {
        return;  // 超出边界，不执行动画
    }

    // 防止直接跳过模块：确保相邻模块才会进行动画切换
    if (abs(nextModule - selectedModule) != 1) {
        return;  // 防止跳过非相邻模块
    }

    isAnimating = true;  // 锁定动画开始

    // 动画逻辑，确保每次只进行一次模块切换
    for (int step = 0; step <= STEP_COUNT; ++step) {
        float progress = (float)step / STEP_COUNT;  // 线性进度

        display.clearDisplay();  // 清屏

        drawTopBar();  // 保持顶部UI的稳定绘制
        drawFrame();   // 保持边框稳定绘制

        // 主选模块变为未选模块的动画（只执行一次）
        if (progress <= 1.0f) {
            animateMainToUnselected(selectedModule, progress);
        }

        // 未选模块变为主选模块的动画（只执行一次）
        if (progress <= 1.0f) {
            animateUnselectedToMain(nextModule, progress);
        }

        // 其余未选模块的动画处理
        for (int i = 0; i < 5; ++i) {
            if (i != selectedModule && i != nextModule) {
                drawUnselectedIcon(10 + i * 40, 20);  // 不变的未选模块保持不动
            }
        }

        display.display();  // 刷新显示
        delay(15);  // 控制动画速度
    }

    // 确保状态在动画结束时更新
    selectedModule = nextModule;  // 更新选中模块状态
    isAnimating = false;  // 动画完成，解锁
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

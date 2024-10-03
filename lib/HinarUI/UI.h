#ifndef UI_H
#define UI_H

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define STEP_COUNT 5  // 动画帧数控制

#define PAGE_NAME "Home"
#define UI_NAME "HinarUI"
#define SELECTED_COLOR SSD1306_WHITE
#define UNSELECTED_COLOR SSD1306_BLACK

class Menu {
public:
    void init();
    void draw();
    void selectModule(int index);
    void animateSelection(bool toRight);  // 动画控制模块切换
    int getModuleNum();
    bool isAnimationComplete();  // 动画完成检测
    
private:
    String modules[5] = {"Mod1", "Mod2", "Mod3", "Mod4", "Mod5"};  // 模块名
    int animationStep = 0;  // 当前动画步数
    int totalSteps = STEP_COUNT;  // 动画总步数
    int currentModulePointer = 0;
    
    struct IconWithLabel {
        int x;
        int y;
        int width;
        int height;
        String label;
    };
    IconWithLabel Icon = { .x = 0, .y = 0, .width = 20, .height = 30};

    // UI 相关绘制函数
    void drawTopBar();
    void drawFrame();
    void drawSelectedIcon(IconWithLabel& icon);
    void drawUnselectedIcon(IconWithLabel& icon);
    void drawModuleIcons(int offset);
};

#endif

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
    void animateSelection(int direction);
    int getModuleNum();
    bool isAnimationComplete();  // 动画完成检测
    
private:
    int selectedModule = 0;
    const char* modules[5] = {"Mod1", "Mod2", "Mod3", "Mod4", "Mod5"};
    int animationStep = 0;  // 动画当前步数
    const int totalSteps = 10;  // 动画总步数
    
    void drawTopBar();
    void drawFrame();
    void drawSelectedIcon(int x, int y);
    void drawUnselectedIcon(int x, int y);
    void drawModuleIcons();
    void drawIconTransition(int x, float progress, int totalSteps, bool toSquare);
    
    void animateModuleCollapse(int index);  // 收回类根号和文字
    void animateModuleExpand(int index);    // 展开类根号和文字
    void animateMainToUnselected(int index, float progress); // 主选变未选
    void animateUnselectedToMain(int index, float progress); // 未选变主选
};

#endif

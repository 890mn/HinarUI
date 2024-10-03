#ifndef UI_H
#define UI_H

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define DEBOU_DELAY       200
#define UNSELECTED_OFFSET 10
#define SCREEN_WIDTH      128
#define SCREEN_HEIGHT     64
#define STEP_COUNT        8 
#define MODULE_MAX        5

#define PAGE_NAME "Home"
#define UI_NAME   "HinarUI"
#define SELECTED_COLOR   SSD1306_WHITE
#define UNSELECTED_COLOR SSD1306_BLACK
class Menu {
public:
    void   init();
    void   draw(int offset);
    void   updatePointer(int index);
    void   animateSelection(bool toRight);
    int    getModuleNum();

private:
    String modules[MODULE_MAX] = {"Mod1", "Mod2", "Mod3", "Mod4", "Mod5"};
    int    animationStep       = 0;
    int    totalSteps          = STEP_COUNT;
    int    modulePointer       = 0;
    struct IconWithLabel {
        int    x;
        int    y;
        int    width;
        int    height;
        String label;
    };
    IconWithLabel Icon = { .x = 0, .y = 0, .width = 20, .height = 30};

    void drawTopBar();
    void drawFrame();

    void drawSelectedIcon(IconWithLabel& icon);
    void drawUnselectedIcon(IconWithLabel& icon);
    void drawModuleIcons(int offset);

    void wordShrink(IconWithLabel& icon);
    void wordGrow(IconWithLabel& icon);
    void rectTrans(IconWithLabel& icon);
};

#endif

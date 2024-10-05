#ifndef UI_H
#define UI_H

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define DEBOU_DELAY       200
#define UNSELECTED_OFFSET 10
#define SCREEN_WIDTH      128
#define SCREEN_HEIGHT     64
#define STEP_COUNT        12 
#define MODULE_MAX        5

#define PAGE_NAME "Home"
#define UI_NAME   "HinarUI"
#define SELECTED_COLOR   SSD1306_WHITE
#define UNSELECTED_COLOR SSD1306_BLACK
class Menu {
public:
    void   init();
    void   draw(int offset);
    void   animateSelection(bool toRight);

private:
    String modules[MODULE_MAX] = {"Mod1", "Mod2", "Mod3", "Mod4", "Mod5"};
    int    animationStep       = 0;
    int    totalStep          = STEP_COUNT;
    int    modulePointer       = 0;
    bool   status_1            = true;
    struct IconWithLabel {
        int    x;
        int    y;
        int    width;
        int    height;
        String label;
    };
    IconWithLabel Icon      = {.x = 10, .y = 25, .height = 30};
    IconWithLabel IconTrans = {         .y = 25, .height = 30};

    void drawTopBar();
    void drawFrame();

    void drawSelectedIcon(IconWithLabel& icon);
    void drawUnselectedIcon(IconWithLabel& icon);
    void drawModuleIcons(int offset, bool init);

    void wordShrink(IconWithLabel& icon);
    void wordGrow(IconWithLabel& icon);
    void pallTransRect(IconWithLabel& icon);
    void rectTransPall(IconWithLabel& icon);
    void reboundAnimation();
};

#endif

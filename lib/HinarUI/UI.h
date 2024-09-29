#ifndef UI_H
#define UI_H

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>


#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

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
    
private:
    int selectedModule = 0;
    const char* modules[5] = {"Mod1", "Mod2", "Mod3", "Mod4", "Mod5"};
    void drawTopBar();
    void drawFrame();
    void drawSelectedIcon(int x, int y);
    void drawUnselectedIcon(int x, int y);
    void drawModuleIcons();
    void drawCompressedIcon(int x, int y, int step, int totalSteps);
    void drawExpandedIcon(int x, int y, int step, int totalSteps);
};

#endif
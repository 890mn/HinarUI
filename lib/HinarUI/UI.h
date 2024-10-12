#ifndef __UI_H
#define __UI_H

#include "resource/icon.h"

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// OLED Setting
#define SCREEN_WIDTH        128
#define SCREEN_HEIGHT       64
#define SELECTED_COLOR      SSD1306_WHITE
#define UNSELECTED_COLOR    SSD1306_BLACK

// Iteration Depth
#define STEP_COUNT          12 

// Render FlowSpeed
#define FLOWSPEED_FAST_PLUS 5
#define FLOWSPEED_FAST      25
#define FLOWSPEED_NORMAL    30
#define FLOWSPEED_SLOW      35

// Module Setting
#define MODULE_MAX          6
#define MODULE_OFFSET       45
#define MODULE_DIRECTION    -1
#define MODULE_FORWARD      3
#define MODULE_BACKWARD     MODULE_MAX - MODULE_FORWARD

#define RADIUS_PALL         2
#define RADIUS_RECT         5

#define KEY_ENTER           18
#define KEY_RIGHT           27
#define KEY_BACK            19


// TopBar Setting
#define UI_NAME             "HinarUI"
class Menu {
public:
    void create();
    void loop();

private:
    String PAGE_NAME           = "FORWARD";
    String modules[MODULE_MAX] = {"LIGHT", "TIME", "DHT11",
                                  "UICORE", "GITHUB", "ABOUT"};
    
    unsigned char* icons[MODULE_MAX] = {bitmap_diode, bitmap_clock, bitmap_data, 
                                        bitmap_chip, bitmap_github, bitmap_cube};

    int backMartix[MODULE_BACKWARD + 1] = {MODULE_FORWARD};

    int    curStep             = 0;
    int    wordStep            = STEP_COUNT / 2;
    int    totalStep           = STEP_COUNT;

    int    forwardPointer      = 0;
    int    backwardPointer     = MODULE_FORWARD;
    int    flowSpeed           = FLOWSPEED_NORMAL;

    bool   isAnimating         = false;
    bool   isBackward          = false;
    bool   isUP                = false;

    int    currentTime         = 0;
    int    UpPT                = 0;
    int    RightPT             = 0;
    int    Threshold           = 400;
    struct Module {
        int x;
        int y;
        int width;
        int height;
        String label;
        unsigned char* icon;
    };
    Module Icon      = {.x = 10, .y = 25, .width = 20, .height = 30, .label = "INIT"};
    Module IconTrans = {         .y = 25, .width = 20, .height = 30, .label = "INIT"};

    void renderForward();
    void renderBackward();

    void draw(int offset, bool init, bool isForward);
    void drawTopBar();
    void drawFrame();

    void drawSeleModule(Module& icon);
    void drawUnseleModule(Module& icon);
    void drawForwardModules(int offset, bool init);
    void drawBackwardModules();

    void wordShrink(Module& icon);
    void wordGrow(Module& icon); 
    
    void pallTrans(Module& icon, int leftTopX, int rightTopX, int rightBottomX, int leftBottomX);
    void pallTransRect(Module& icon);
    void rectTransPall(Module& icon);

    float easeInOut(float t);    
};

#endif

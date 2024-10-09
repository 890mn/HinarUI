#ifndef UI_H
#define UI_H

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
#define MODULE_MAX          9
#define MODULE_OFFSET       45
#define MODULE_DIRECTION    -1
#define MODULE_FORWARD      3
#define MODULE_BACKWARD     MODULE_MAX - MODULE_FORWARD

#define RADIUS_PALL         2
#define RADIUS_RECT         5

#define KEY_UP              18
#define KEY_RIGHT           19

// TopBar Setting
#define UI_NAME             "HinarUI"
class Menu {
public:
    void create();
    void loop();

private:
    String PAGE_NAME           = "FORWARD";
    String modules[MODULE_MAX] = {"Mod-1", "Mod-2", "Mod-3",
                                  "BACK1", "BACK2", "BACK3", "BACK4", "BACK5", "BACK6"};

    int    backMartix[MODULE_BACKWARD + 1] = {MODULE_FORWARD};

    int    curStep             = 0;
    int    wordStep            = STEP_COUNT / 2;
    int    totalStep           = STEP_COUNT;

    int    forwardPointer      = 0;
    int    backwardPointer     = MODULE_FORWARD;
    int    flowSpeed           = FLOWSPEED_NORMAL;

    bool   isAnimating         = false;
    bool   isbackward          = false;

    int    currentTime         = 0;
    int    UpPT                = 0;
    int    RightPT             = 0;
    int    Threshold           = 400;

    
    struct IconWithLabel {
        int    x;
        int    y;
        int    width;
        int    height;
        String label;
    };
    IconWithLabel Icon      = {.x = 10, .y = 25, .width = 20, .height = 30, .label = "INIT"};
    IconWithLabel IconTrans = {         .y = 25, .width = 20, .height = 30, .label = "INIT"};

    void renderForward();
    void renderBackward();

    void draw(int offset, bool init, bool isForward);
    void drawTopBar();
    void drawFrame();

    void drawSeleModule(IconWithLabel& icon);
    void drawUnseleModule(IconWithLabel& icon);
    void drawForwardModules(int offset, bool init);
    void drawBackwardModules();

    void wordShrink(IconWithLabel& icon);
    void wordGrow(IconWithLabel& icon); 
    
    void pallTrans(IconWithLabel& icon, int leftTopX, int rightTopX, int rightBottomX, int leftBottomX);
    void pallTransRect(IconWithLabel& icon);
    void rectTransPall(IconWithLabel& icon);

    float easeInOut(float t);    
};

#endif

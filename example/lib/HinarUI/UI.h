#ifndef __UI_H
#define __UI_H

#include "resource/icon.h"
#include "resource/oled.h"
#include "resource/module.h"

// Iteration Depth
#define STEP_COUNT          12 

// Render FlowSpeed
#define FLOWSPEED_FAST_PLUS 5
#define FLOWSPEED_FAST      20
#define FLOWSPEED_NORMAL    25
#define FLOWSPEED_SLOW      35

#define KEY_ENTER           18
#define KEY_BACK            19
#define KEY_CYCLE           27

// TopBar Setting
#define UI_NAME             "HinarUI"

typedef void (*entry)();

class Menu {
public:
    void create();
    void loop();
    void drawTopBar();
    void drawFrame();

private:
    String PAGE_NAME           = "FORWARD";

    int backMartix[MODULE_BACKWARD + 1] = {MODULE_FORWARD};
    int backMackup[MODULE_BACKWARD + 1] = {MODULE_FORWARD};

    unsigned char* iconBaset   = icons[MODULE_FORWARD];
    String         labelBaset  = labels[MODULE_FORWARD];

    String labels[MODULE_MAX] = {"LIGHT", "TIME", "DHT11",
                                 "UICORE", "GITHUB", "ABOUT"};
    
    unsigned char* icons[MODULE_MAX] = {bitmap_diode, bitmap_clock, bitmap_data, 
                                        bitmap_chip, bitmap_github, bitmap_cube};

    entry modules[MODULE_MAX] = {module_LIGHT, module_TIME, module_DHT11,
                                 module_UICORE, module_github, module_ABOUT};

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
    int    CyclePress          = 0;
    int    Threshold           = 500;
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

    enum MenuState {
        IDLE,                // Base     Status
        FORWARD,             // Forward  Cycle
        BACKWARD,            // Backward Status
        BACKWARD_SELECTED,   // Backward Cycle
        MODULE               // Module   Status
    };
    MenuState currentState = IDLE;

    void renderDynamic(int keyCycleState, bool isForward);
    void renderForward();
    void renderBackward();

    void draw(int offset, bool init, bool isForward);
    void drawSeleModule(Module& icon);
    void drawUnseleModule(Module& icon);
    
    void drawForwardModules(int offset, bool init);
    void drawBackwardModules();

    void wordShrink(Module& icon);
    void wordGrow(Module& icon); 
    
    void pallTrans(Module& icon, int leftTopX, int rightTopX, int rightBottomX, int leftBottomX);
    void pallTransRect(Module& icon);
    void rectTransPall(Module& icon);

    void backwardTrans();
    void backwardSave();
    void backwardLoad();

    float easeInOut(float t);    
};
extern Menu menu;

#endif

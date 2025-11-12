#ifndef HINARUI_CORE_H
#define HINARUI_CORE_H

#include "icon.h"
#include "Profile.h"

// Iteration Depth
#define STEP_COUNT          12 

// Render FlowSpeed
#define FLOWSPEED_FAST_PLUS 5
#define FLOWSPEED_FAST      20
#define FLOWSPEED_NORMAL    25
#define FLOWSPEED_SLOW      35

#define MODULE_MAX          6
#define MODULE_OFFSET       45
#define MODULE_DIRECTION    -1
#define MODULE_FORWARD      3
#define MODULE_BACKWARD     MODULE_MAX - MODULE_FORWARD

#define RADIUS_PALL         2
#define RADIUS_RECT         5

void module_serial();
void module_battery();
void module_SHT30();
void module_UICORE();
void module_github();
void module_ABOUT();

static String PAGE_NAME = "FORWARD";
static String UI_NAME   = "HinarUI";

typedef void (*entry)();
class Menu {
public:
    void create();
    void loop();
    void drawTopBar(String page = PAGE_NAME, String ui = UI_NAME);
    void drawFrame();
    int getIback() { return i_back; }

    String getFlowSpeed();

private:
    String tmpLabel = labels[MODULE_FORWARD];
    unsigned char* tmpIcons = icons[MODULE_FORWARD];

    String labels[MODULE_MAX] = {"SERIAL", "VBAT", "SHT30",
                                 "UICORE", "GITHUB", "ABOUT"};
    
    unsigned char* icons[MODULE_MAX] = {bitmap_serial, bitmap_battery, bitmap_sht, 
                                        bitmap_chip, bitmap_github, bitmap_about};

    entry modules[MODULE_MAX] = {module_serial, module_battery, module_SHT30,
                                 module_UICORE, module_github, module_ABOUT};

    int    curStep             = 0;
    int    wordStep            = STEP_COUNT / 2;
    int    totalStep           = STEP_COUNT;

    int    forwardPointer      = 0;
    int    backwardPointer     = MODULE_FORWARD;
    int    i_back              = backwardPointer;
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
    String stateToString();

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

    float easeInOut(float t);
};
extern Menu menu;

#endif

#ifndef HINARUI_CORE_H
#define HINARUI_CORE_H

#include "Profile.h"
#include "icon.h"
#include "perf.h"

#include "HinarUI/core/FrameBufferManager.h"
#include "HinarUI/core/MenuAnimator.h"
#include "HinarUI/core/MenuConfig.h"
#include "HinarUI/core/MenuModuleRegistry.h"
#include "HinarUI/core/MenuRenderer.h"
#include "HinarUI/core/MenuTypes.h"

extern String PAGE_NAME;
extern String UI_NAME;

class Menu {
public:
    Menu();

    void create();
    void loop();
    void drawTopBar(String page = PAGE_NAME, String ui = UI_NAME);
    void drawFrame();
    int getIback() const { return i_back; }

    String getFlowSpeed();
    int getStepCount() const { return config.stepCount; }
    bool isDeveloperModeEnabled() const { return developerModeEnabled; }
    void setDeveloperMode(bool enabled) { developerModeEnabled = enabled; }

private:
    friend class MenuAnimator;
    friend class MenuRenderer;

    MenuConfig config;
    MenuModuleRegistry registry;
    MenuAnimator animator;
    MenuRenderer renderer;

    String tmpLabel;
    unsigned char* tmpIcons = nullptr;

    int    curStep             = 0;
    int    wordStep            = 0;
    int    totalStep           = 0;

    int    forwardPointer      = 0;
    int    backwardPointer     = 0;
    int    i_back              = 0;
    int    flowSpeed           = 0;

    bool   isAnimating         = false;
    bool   isBackward          = false;
    bool   isUP                = false;
    MenuState stateBeforeSleep = MenuState::Idle;

    int    currentTime         = 0;
    int    CyclePress          = 0;
    int    Threshold           = 500;

    ModuleVisual Icon      {10, 25, 20, 30, "INIT"};
    ModuleVisual IconTrans {0, 25, 20, 30, "INIT"};

    MenuState currentState = MenuState::Idle;
    bool developerModeEnabled = true;

    String stateToString();
    void draw(int offset, bool init, bool isForward);
};
extern Menu menu;

#endif

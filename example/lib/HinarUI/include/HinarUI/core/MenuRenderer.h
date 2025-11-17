#pragma once

#include "HinarUI/core/MenuConfig.h"
#include "HinarUI/core/MenuTypes.h"

class Menu;

class MenuRenderer {
public:
    explicit MenuRenderer(Menu& owner);

    void drawTopBar(const String& page, const String& ui);
    void drawFrame() const;
    void drawMenu(int offset, bool init, bool isForward);

private:
    void drawSelectedModule(ModuleVisual& icon);
    void drawUnselectedModule(ModuleVisual& icon);
    void drawForwardModules(int offset, bool init);
    void drawBackwardModules();

    void wordShrink(ModuleVisual& icon);
    void wordGrow(ModuleVisual& icon);

    void pallTrans(ModuleVisual& icon, int leftTopX, int rightTopX, int rightBottomX, int leftBottomX);
    void pallTransRect(ModuleVisual& icon);
    void rectTransPall(ModuleVisual& icon);

    Menu& menu_;
};

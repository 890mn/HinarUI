#include "HinarUI/core/MenuRenderer.h"

#include <math.h>

#include "HinarUI_Core.h"

MenuRenderer::MenuRenderer(Menu& owner) : menu_(owner) {}

void MenuRenderer::drawMenu(int offset, bool init, bool isForward) {
    display.clearDisplay();
    drawTopBar(PAGE_NAME, menu_.isDeveloperModeEnabled() ? perf.fpsLabel() : UI_NAME);
    if (isForward) {
        drawForwardModules(offset, init);
    } else {
        drawBackwardModules();
    }
    drawFrame();
}

void MenuRenderer::drawFrame() const {
    display.drawLine(0  , 0 , 127, 0 , SELECTED_COLOR); // TOP    1
    display.drawLine(0  , 0 , 0  , 15, SELECTED_COLOR); // LEFT   1
    display.drawLine(127, 0 , 127, 5 , SELECTED_COLOR); // RIGHT  1
    display.drawLine(0  , 15, 43 , 15, SELECTED_COLOR); // BOTTOM 1

    display.drawLine(98 , 16, 127, 16, SELECTED_COLOR); // TOP    2
    display.drawLine(0  , 40, 0  , 63, SELECTED_COLOR); // LEFT   2
    display.drawLine(127, 16, 127, 63, SELECTED_COLOR); // RIGHT  2
    display.drawLine(0  , 63, 30 , 63, SELECTED_COLOR); // BOTTOM 2-1
    display.drawLine(115, 63, 127, 63, SELECTED_COLOR); // BOTTOM 2-2
}

void MenuRenderer::drawTopBar(const String& page, const String& ui) {
    display.setCursor(4, 5);
    display.print(page);

    int16_t x1, y1;
    uint16_t w, h;
    display.getTextBounds(ui, 0, 0, &x1, &y1, &w, &h);
    display.setCursor(SCREEN_WIDTH - w - 4, 5);
    display.print(ui);
}

void MenuRenderer::drawSelectedModule(ModuleVisual& icon) {
    icon.width = 30;
    display.drawRoundRect(icon.x, icon.y, icon.width, icon.height, menu_.config.radiusRect, SELECTED_COLOR);
    display.drawFastVLine(icon.x + icon.width - 1, icon.y + 2, icon.height - 2, UNSELECTED_COLOR);

    // 1 - ROOT
    display.drawLine(icon.x + icon.width - 4, icon.y + icon.height - 1,
                     icon.x + icon.width + 5, icon.y + icon.height - 10, SELECTED_COLOR);
    display.drawLine(icon.x + icon.width + 5, icon.y + icon.height - 10,
                     icon.x + icon.width + 25, icon.y + icon.height - 10, SELECTED_COLOR);

    // 2 - NAME
    display.setCursor(icon.x + icon.width + 8, icon.y + 9);
    display.print(icon.label);
}

void MenuRenderer::drawUnselectedModule(ModuleVisual& icon) {
    icon.width = 20;

     // 1 - LEFT-TOP
    display.drawCircleHelper(icon.x + 2, icon.y + 2,
                             menu_.config.radiusPall, 1, SELECTED_COLOR);

    // 2 - LEFT-TOP -> LEFT-BOTTOM
    display.drawLine(icon.x, icon.y + 2,
                     icon.x - 10, icon.y + icon.height - 2, SELECTED_COLOR);          

    // 2 - LEFT-TOP -> RIGHT-TOP
    display.drawLine(icon.x + 2, icon.y,
                     icon.x + icon.width - 2, icon.y, SELECTED_COLOR);

    // 3 - LEFT-BOTTOM
    display.drawCircleHelper(icon.x - 10 + 2, icon.y + icon.height - 2,
                             menu_.config.radiusPall, 8, SELECTED_COLOR);

    // 3 - RIGHT-TOP
    display.drawCircleHelper(icon.x + icon.width - 2, icon.y + 2,
                             menu_.config.radiusPall, 2, SELECTED_COLOR);          

    // 4 - RIGHT-TOP -> RIGHT-BOTTOM
    //display.drawLine(icon.x + icon.width, icon.y + 2,
    //                 icon.x + icon.width - 10, icon.y + icon.height - 2, SELECTED_COLOR);

    // 4 - LEFT-BOTTOM -> RIGHT-BOTTOM
    display.drawLine(icon.x - 10 + 2, icon.y + icon.height,
                     icon.x + icon.width - 10 - 2, icon.y + icon.height, SELECTED_COLOR);

    // 5 - RIGHT-BOTTOM
    display.drawCircleHelper(icon.x + icon.width - 10 - 2, icon.y + icon.height - 2,
                             menu_.config.radiusPall, 4, SELECTED_COLOR);   
}

void MenuRenderer::drawForwardModules(int offset, bool init) {
    menu_.IconTrans = menu_.Icon;
    menu_.IconTrans.x += offset; 
    menu_.Icon = menu_.IconTrans;
    bool next = false;

    if (init) {
        for (int i = 0; i < menu_.config.moduleForward; ++i) {
            menu_.IconTrans.label = menu_.registry.label(i);
            if (i == 0) {
                drawSelectedModule(menu_.IconTrans);
                display.drawBitmap(menu_.IconTrans.x + 3, menu_.IconTrans.y + 3, menu_.registry.icon(0), 24, 24, SELECTED_COLOR, UNSELECTED_COLOR);
                menu_.IconTrans.x += 85;
            } else {
                drawUnselectedModule(menu_.IconTrans);
                display.drawBitmap(menu_.IconTrans.x, menu_.IconTrans.y + 3, menu_.registry.icon(1), 24, 24, SELECTED_COLOR, UNSELECTED_COLOR);
                menu_.IconTrans.x += 50;
            }
        }
        return;
    }
    
    for (int i = 0; i <= menu_.config.moduleForward; ++i) {
        menu_.IconTrans.label = menu_.registry.label(i);
        menu_.IconTrans.icon = menu_.registry.icon(i);

        // MainModule
        if (i == menu_.forwardPointer) {  
            //MainModule Debug
            //display.drawCircle(IconTrans.x + 15, IconTrans.y + 15, 2, SELECTED_COLOR); 
            display.drawBitmap(menu_.IconTrans.x + 3, menu_.IconTrans.y + 3, menu_.IconTrans.icon, 24, 24, SELECTED_COLOR);

            if (menu_.curStep < menu_.totalStep / 2) {
                drawSelectedModule(menu_.IconTrans);    // module-selected
                wordShrink(menu_.IconTrans);        // shrink word-root
            } else {
                rectTransPall(menu_.IconTrans);     // rect->pall
            }

            if (menu_.forwardPointer == menu_.config.moduleForward) {
                menu_.IconTrans.label = menu_.registry.label(0);
                menu_.IconTrans.x += (menu_.totalStep - menu_.curStep) * (30 / menu_.totalStep) + 40;
                
                if (menu_.curStep < menu_.totalStep / 2) {
                    pallTransRect(menu_.IconTrans);     // pall->rect
                } else {
                    menu_.IconTrans.height = menu_.IconTrans.width = 30;
                    display.drawRoundRect(menu_.IconTrans.x, menu_.IconTrans.y,
                                          menu_.IconTrans.width, menu_.IconTrans.height, menu_.config.radiusRect, SELECTED_COLOR); 
                    display.drawFastVLine(menu_.IconTrans.x + menu_.IconTrans.width - 1, menu_.IconTrans.y + 2, menu_.IconTrans.height - 2, UNSELECTED_COLOR);                    
                    wordGrow(menu_.IconTrans);         // grow word-root
                }
            }
        }

        // MainModule right first
        else if (i == menu_.forwardPointer + 1) {
            int tmp = menu_.IconTrans.x + 10;
            menu_.IconTrans.x += (menu_.totalStep - menu_.curStep) * (30 / menu_.totalStep);
            display.drawBitmap(menu_.IconTrans.x + 3, menu_.IconTrans.y + 3, menu_.IconTrans.icon, 24, 24, SELECTED_COLOR);

            if (menu_.curStep < menu_.totalStep / 2) {
                pallTransRect(menu_.IconTrans);     // pall->rect
            } else {
                menu_.IconTrans.height = menu_.IconTrans.width = 30;
                display.drawRoundRect(menu_.IconTrans.x, menu_.IconTrans.y,
                                      menu_.IconTrans.width, menu_.IconTrans.height, menu_.config.radiusRect, SELECTED_COLOR); 
                display.drawFastVLine(menu_.IconTrans.x + menu_.IconTrans.width - 1, menu_.IconTrans.y + 2, menu_.IconTrans.height - 2, UNSELECTED_COLOR);
                wordGrow(menu_.IconTrans);         // grow word-root

                if (i == menu_.config.moduleForward && menu_.curStep > menu_.totalStep - 2) {
                    display.drawRoundRect(89, 35, 33, 8, menu_.config.radiusPall, SELECTED_COLOR);

                    if (menu_.curStep == menu_.totalStep - 1) {
                        delay(30);

                        //UP
                        display.setCursor(60, 21);
                        display.print(menu_.registry.label(menu_.config.moduleForward - 1));
                        display.drawRoundRect(100, 21, 33, 8, menu_.config.radiusPall, SELECTED_COLOR);

                        //DOWN
                        display.setCursor(61, 50);
                        display.print(menu_.registry.label(menu_.config.moduleForward + 1));
                        display.drawRoundRect(99, 51, 33, 8, menu_.config.radiusPall, SELECTED_COLOR);
                    }              
                    menu_.isBackward = true;
                }
            }
            next = true;
            menu_.IconTrans.x = tmp;
        }
        else {
            if (next) {
                menu_.IconTrans.x += 40;
                menu_.IconTrans.icon = menu_.registry.icon(menu_.forwardPointer + 2);
                display.drawBitmap(menu_.IconTrans.x - 1, menu_.IconTrans.y + 3, menu_.IconTrans.icon, 24, 24, SELECTED_COLOR);
            }
            drawUnselectedModule(menu_.IconTrans);
        }
        menu_.IconTrans.x += 35;
    }
}

void MenuRenderer::drawBackwardModules() {
    menu_.IconTrans = ModuleVisual(
        10, 25, 30, 30,
        menu_.registry.label(menu_.config.moduleForward - 1),
        menu_.registry.icon(menu_.config.moduleForward - 1));

    display.drawRoundRect(menu_.IconTrans.x, menu_.IconTrans.y, menu_.IconTrans.width, menu_.IconTrans.height, menu_.config.radiusRect, SELECTED_COLOR);

    // 1 - ROOT
    display.drawLine(menu_.IconTrans.x + menu_.IconTrans.width - 4 , menu_.IconTrans.y + menu_.IconTrans.height - 1,
                     menu_.IconTrans.x + menu_.IconTrans.width + 5 , menu_.IconTrans.y + menu_.IconTrans.height - 10, SELECTED_COLOR);
    display.drawLine(menu_.IconTrans.x + menu_.IconTrans.width + 5 , menu_.IconTrans.y + menu_.IconTrans.height - 10,
                     menu_.IconTrans.x + menu_.IconTrans.width + 25, menu_.IconTrans.y + menu_.IconTrans.height - 10, SELECTED_COLOR);
    
    int num = menu_.i_back;
    for (int cnt = 0; cnt < 3; ++cnt) {
        menu_.IconTrans.label = menu_.registry.label(num);
        
        // MIDDLE -> UP
        if (cnt == 0) {  
            int offsetX = menu_.curStep;
            int offsetY = 4 * pow(menu_.curStep, 0.5);

            display.setCursor(49 + offsetX, 35 - offsetY);
            display.print(menu_.IconTrans.label);

            if (menu_.curStep < menu_.totalStep / 4) {
                display.fillRoundRect(89 + offsetX, 35 - offsetY, 33, 8, menu_.config.radiusPall, SELECTED_COLOR);
            } else {
                display.drawRoundRect(89 + offsetX, 35 - offsetY, 33, 8, menu_.config.radiusPall, SELECTED_COLOR);
            }
/*
            if (curStep == totalStep - 1) {
                Serial.print("\nsetCursor\n");
                Serial.print(49 + offsetX); // 60
                Serial.print("\n");
                Serial.print(35 - offsetY); // 22

                Serial.print("\ndisplay.drawRoundRect\n");
                Serial.print(89 + offsetX); // 100
                Serial.print("\n");
                Serial.print(35 - offsetY); // 22
            }
*/
        }

        // LOW -> MIDDLE
        else if (cnt == 1) { 
            int offsetX = menu_.curStep;
            int offsetY = 0.125 * pow(menu_.curStep, 2);

            display.setCursor(60 - offsetX, 51 - offsetY);
            display.print(menu_.IconTrans.label);

            menu_.IconTrans.icon = menu_.registry.icon(num);
        
            display.drawRect(menu_.IconTrans.x + 3, menu_.IconTrans.y + 3, 24, 24, UNSELECTED_COLOR);
            display.drawFastVLine(menu_.IconTrans.x + menu_.IconTrans.width - 1, menu_.IconTrans.y + 2, menu_.IconTrans.height - 6, UNSELECTED_COLOR);
            display.drawBitmap(menu_.IconTrans.x + 3, menu_.IconTrans.y + 3, menu_.IconTrans.icon, 24, 24, SELECTED_COLOR);

            if (menu_.curStep == menu_.totalStep - 1) {
                display.fillRoundRect(100 - offsetX, 50 - offsetY, 33, 8, menu_.config.radiusPall, SELECTED_COLOR);
            } else {
                display.drawRoundRect(100 - offsetX, 50 - offsetY, 33, 8, menu_.config.radiusPall, SELECTED_COLOR);
            }
/*
            if (curStep == totalStep - 1) {
                Serial.print("\nsetCursor\n"); 
                Serial.print(60 - offsetX); // 49
                Serial.print("\n");
                Serial.print(51 - offsetY); // 36

                Serial.print("\ndisplay.drawRoundRect\n");
                Serial.print(100 - offsetX); // 89
                Serial.print("\n");
                Serial.print(50 - offsetY); // 35
            }
*/            
        }

        // OUT -> LOW
        else if (cnt == 2) { 
            int offsetX = menu_.curStep;
            int offsetY = 0.125 * pow(menu_.curStep, 2);

            display.setCursor(72 - offsetX, 65 - offsetY);
            display.print(menu_.IconTrans.label);
            display.drawRoundRect(110 - offsetX, 66 - offsetY, 33, 8, menu_.config.radiusPall, SELECTED_COLOR);
/*
            if (curStep == totalStep - 1) {
                Serial.print("\nsetCursor\n");
                Serial.print(72 - offsetX); // 61
                Serial.print("\n");
                Serial.print(65 - offsetY); // 50

                Serial.print("\ndisplay.drawRoundRect\n");
                Serial.print(110 - offsetX); // 99
                Serial.print("\n");
                Serial.print(66 - offsetY); // 51
            }
*/
        }
        ++num;
        if (num == menu_.config.moduleMax) num = menu_.backwardPointer;
    }
}

void MenuRenderer::wordShrink(ModuleVisual& icon) {
    float shrinkOffset = 45.0 / menu_.wordStep;
    display.fillRect(icon.x + 30 + shrinkOffset * (menu_.wordStep - menu_.curStep), icon.y + 9,
                     shrinkOffset * menu_.curStep, 20, UNSELECTED_COLOR);
}

void MenuRenderer::wordGrow(ModuleVisual& icon) {
    int growOffset = menu_.curStep - menu_.wordStep;

    int startX = icon.x + icon.width - 4;
    int startY = icon.y + icon.height - 1;
    
    int endX1 = icon.x + icon.width + 6;
    int endY1 = icon.y + icon.height - 11;

    int currentEndX1 = startX + 20 / menu_.wordStep * growOffset;  // (END - START) * 2
    int currentEndY1 = startY - 20 / menu_.wordStep * growOffset;

    int currentEndX2 = endX1 + 40 / menu_.wordStep * (growOffset - menu_.wordStep * 3 / 7); 

    //if (modules[forwardPointer + 1] == icon.label) {
        if (growOffset >= menu_.wordStep * 3 / 7) {
            display.drawLine(startX, startY, endX1, endY1, SELECTED_COLOR); 
            display.drawLine(endX1, endY1, currentEndX2, endY1, SELECTED_COLOR);
        }
        else {
            display.drawLine(startX, startY, currentEndX1, currentEndY1, SELECTED_COLOR); 
        }

        if (menu_.curStep > menu_.totalStep - 4) {
            display.setCursor(icon.x + icon.width + 10, icon.y + 9); 
            display.setTextColor(SELECTED_COLOR);
            display.print(icon.label);
        }
    //}
}

void MenuRenderer::pallTrans(ModuleVisual& icon, int leftTopX, int rightTopX, int rightBottomX, int leftBottomX) {
     // 1 - LEFT-TOP
    display.drawCircleHelper(leftTopX + menu_.config.radiusPall, icon.y + menu_.config.radiusPall,
                             menu_.config.radiusPall, 1, SELECTED_COLOR);

    // 2 - LEFT-BOTTOM -> LEFT-TOP
    display.drawLine(leftBottomX, icon.y + icon.height - menu_.config.radiusPall,
                     leftTopX, icon.y + menu_.config.radiusPall, SELECTED_COLOR);    

    // 2 - LEFT-TOP -> RIGHT-TOP
    display.drawLine(leftTopX + menu_.config.radiusPall, icon.y,
                     rightTopX - menu_.config.radiusPall, icon.y, SELECTED_COLOR); 

    // 3 - RIGHT-TOP
    display.drawCircleHelper(rightTopX - menu_.config.radiusPall, icon.y + menu_.config.radiusPall,
                             menu_.config.radiusPall, 2, SELECTED_COLOR);

    // 3 - LEFT-BOTTOM
    display.drawCircleHelper(leftBottomX + menu_.config.radiusPall, icon.y + icon.height - menu_.config.radiusPall,
                             menu_.config.radiusPall, 8, SELECTED_COLOR);

    // 4 - RIGHT-TOP -> RIGHT-BOTTOM
    //display.drawLine(rightTopX, icon.y + menu_.config.radiusPall,
    //                         rightBottomX, icon.y + icon.height - menu_.config.radiusPall, SELECTED_COLOR); 

    // 4 - RIGHT-BOTTOM -> LEFT-BOTTOM
    display.drawLine(rightBottomX - menu_.config.radiusPall, icon.y + icon.height,
                             leftBottomX + menu_.config.radiusPall, icon.y + icon.height, SELECTED_COLOR); 

    // 5 - RIGHT-BOTTOM
    display.drawCircleHelper(rightBottomX - menu_.config.radiusPall, icon.y + icon.height - menu_.config.radiusPall,
                             menu_.config.radiusPall, 4, SELECTED_COLOR);
}

void MenuRenderer::pallTransRect(ModuleVisual& icon) {
    int transOffset = (menu_.curStep - menu_.wordStep) * (10 / menu_.wordStep);
    icon.height -= 1;

    pallTrans(icon, icon.x - transOffset, icon.x + icon.width,
              icon.x + icon.width + transOffset, icon.x);
}

void MenuRenderer::rectTransPall(ModuleVisual& icon) {
    float transOffset = static_cast<float>(menu_.curStep - menu_.wordStep) / menu_.wordStep;
    float progress = menu_.animator.easeInOut(transOffset);

    pallTrans(icon, icon.x, icon.x + 30 - 10.0 * progress,
              icon.x + 25 - 15.0 * progress, icon.x - 10.0 * progress);
}

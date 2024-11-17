#include "UI.h"

Menu menu;
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void Menu::create() {
    Serial.begin(115200);

    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
        Serial.println(F("SSD1306 allocation failed"));
        while (1);
    }

    pinMode(KEY_ENTER, INPUT_PULLUP);
    pinMode(KEY_CYCLE, INPUT_PULLUP);
    pinMode(KEY_BACK , INPUT_PULLUP);

    Menu();

    display.setTextSize(1);
    display.setTextColor(SELECTED_COLOR);
    display.setCursor(0, 0);
    draw(0, true, true);
}

void Menu::loop() {
    currentTime = millis();

    if (!isAnimating) {
        int keyEnterState = digitalRead(KEY_ENTER);
        int keyCycleState = digitalRead(KEY_CYCLE);
        int keyBackState  = digitalRead(KEY_BACK);

        switch (currentState) {
            case IDLE:
                // IDLE -> BACKWARD
                if (keyEnterState == LOW && forwardPointer == MODULE_FORWARD) {
                    isBackward = true;
                    display.fillRoundRect(89, 35, 33, 8, RADIUS_PALL, SELECTED_COLOR);
                    display.display();
                    currentState = BACKWARD;
                }

                // IDLE -> FORWARD
                else if (keyCycleState == LOW) {

                    currentState = FORWARD;
                }

                // IDLE -> MODULE
                else if (keyEnterState == LOW) {
                    modules[forwardPointer]();
                    currentState = MODULE;
                }
                break;

            case FORWARD:
                // FORWARD -> FORWARD
                renderDynamic(keyCycleState, true);
                break;

            case BACKWARD:
                // BACKWARD -> BACKWARD_SELECTED
                if (keyEnterState == LOW) {
                    isUP = true;
                    while (keyEnterState == LOW) {
                        keyEnterState = digitalRead(KEY_ENTER);
                        delay(3);
                    }
                    currentState = BACKWARD_SELECTED;
                }
                break;

            case BACKWARD_SELECTED:
                // BACKWARD_SELECTED -> BACKWARD_SELECTED
                renderDynamic(keyCycleState, false);

                // BACKWARD_SELECTED -> IDLE
                if (keyBackState == LOW) {
                    display.fillRect(90, 36, 31, 6, UNSELECTED_COLOR);
                    display.display();
                    isBackward = false;
                    isUP = false;     
                    currentState = IDLE;     
                }

                // BACKWARD_SELECTED -> MODULE
                if (keyEnterState == LOW) {
                    modules[i_back]();
                    currentState = MODULE;
                }
                break;
            
            case MODULE:
                // MODULE -> BACKWARD_SELECTED
                if (keyBackState == LOW && forwardPointer == MODULE_FORWARD) {
                    while (keyBackState == LOW) {
                        keyBackState = digitalRead(KEY_BACK);
                        delay(3);
                    }

                    PAGE_NAME = "BACKWARD";
                    UI_NAME   = "HinarUI";
                    --i_back;

                    curStep = totalStep - 1;
                    draw(1, false, false);

                    ++i_back;
                    currentState = BACKWARD_SELECTED;
                }

                // MODULE -> IDLE
                if (keyBackState == LOW && forwardPointer != MODULE_FORWARD) {
                    --forwardPointer;

                    curStep = totalStep - 1;
                    draw(1, false, true);

                    ++forwardPointer;
                    currentState = IDLE;
                }
                break;
        }
    }
}

void Menu::draw(int offset, bool init, bool isForward) {
    display.clearDisplay();
    drawTopBar();
    if (isForward) {
        drawForwardModules(offset, init);
    } else {
        drawBackwardModules();
    }
    drawFrame();
    display.display();
}

void Menu::drawFrame() {
    display.drawLine(0  , 0 , 127, 0 , SELECTED_COLOR); // TOP    1
    display.drawLine(0  , 0 , 0  , 15, SELECTED_COLOR); // LEFT   1
    display.drawLine(127, 0 , 127, 5 , SELECTED_COLOR); // RIGHT  1
    display.drawLine(0  , 15, 43 , 15, SELECTED_COLOR); // BOTTOM 1

    display.drawLine(98 , 16, 127, 16, SELECTED_COLOR); // TOP    2
    display.drawLine(0  , 40, 0  , 63, SELECTED_COLOR); // LEFT   2
    display.drawLine(127, 16, 127, 63, SELECTED_COLOR); // RIGHT  2
    display.drawLine(0  , 63, 30 , 63, SELECTED_COLOR); // BOTTOM 2-1
    display.drawLine(105, 63, 127, 63, SELECTED_COLOR); // BOTTOM 2-2
}

void Menu::drawTopBar() {
    display.setCursor(4, 5);
    display.print(PAGE_NAME);

    int16_t x1, y1;
    uint16_t w, h;
    display.getTextBounds(UI_NAME, 0, 0, &x1, &y1, &w, &h);
    display.setCursor(SCREEN_WIDTH - w - 4, 5);
    display.print(UI_NAME);
}

void Menu::drawSeleModule(Module& icon) {
    icon.width = 30;
    display.drawRoundRect(icon.x, icon.y, icon.width, icon.height, RADIUS_RECT, SELECTED_COLOR);
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

void Menu::drawUnseleModule(Module& icon) {
    icon.width = 20;

    // 1 - LEFT-TOP
    display.drawCircleHelper(icon.x + 2, icon.y + 2,
                             RADIUS_PALL, 1, SELECTED_COLOR);

    // 2 - LEFT-TOP -> LEFT-BOTTOM
    display.drawLine(icon.x, icon.y + 2,
                     icon.x - 10, icon.y + icon.height - 2, SELECTED_COLOR);          

    // 2 - LEFT-TOP -> RIGHT-TOP
    display.drawLine(icon.x + 2, icon.y,
                     icon.x + icon.width - 2, icon.y, SELECTED_COLOR);

    // 3 - LEFT-BOTTOM
    display.drawCircleHelper(icon.x - 10 + 2, icon.y + icon.height - 2,
                             RADIUS_PALL, 8, SELECTED_COLOR);

    // 3 - RIGHT-TOP
    display.drawCircleHelper(icon.x + icon.width - 2, icon.y + 2,
                             RADIUS_PALL, 2, SELECTED_COLOR);          

    // 4 - RIGHT-TOP -> RIGHT-BOTTOM
    //display.drawLine(icon.x + icon.width, icon.y + 2,
    //                 icon.x + icon.width - 10, icon.y + icon.height - 2, SELECTED_COLOR);

    // 4 - LEFT-BOTTOM -> RIGHT-BOTTOM
    display.drawLine(icon.x - 10 + 2, icon.y + icon.height,
                     icon.x + icon.width - 10 - 2, icon.y + icon.height, SELECTED_COLOR);

    // 5 - RIGHT-BOTTOM
    display.drawCircleHelper(icon.x + icon.width - 10 - 2, icon.y + icon.height - 2,
                             RADIUS_PALL, 4, SELECTED_COLOR);     
}

void Menu::drawForwardModules(int offset, bool init) {
    IconTrans = Icon;
    IconTrans.x += offset; 
    Icon = IconTrans;
    bool next = false;

    if (init) {
        for (int i = 0; i < MODULE_FORWARD; ++i) {
            IconTrans.label = labels[i];
            if (i == 0) {
                drawSeleModule(IconTrans);
                display.drawBitmap(IconTrans.x + 3, IconTrans.y + 3, bitmap_diode, 24, 24, SELECTED_COLOR, UNSELECTED_COLOR);
                IconTrans.x += 85;
            } else {
                drawUnseleModule(IconTrans);
                display.drawBitmap(IconTrans.x, IconTrans.y + 3, bitmap_clock, 24, 24, SELECTED_COLOR, UNSELECTED_COLOR);
                IconTrans.x += 50;
            }
        }
        return;
    }

    for (int i = 0; i <= MODULE_FORWARD; ++i) {
        IconTrans.label = labels[i];
        IconTrans.icon = icons[i];

        // MainModule
        if (i == forwardPointer) {  
            //MainModule Debug
            //display.drawCircle(IconTrans.x + 15, IconTrans.y + 15, 2, SELECTED_COLOR); 
            display.drawBitmap(IconTrans.x + 3, IconTrans.y + 3, IconTrans.icon, 24, 24, SELECTED_COLOR);

            if (curStep < totalStep / 2) {
                drawSeleModule(IconTrans);    // module-selected
                wordShrink(IconTrans);        // shrink word-root
            } else {
                rectTransPall(IconTrans);     // rect->pall
            }

            if (forwardPointer == MODULE_FORWARD) {
                IconTrans.label = labels[0];
                IconTrans.x += (totalStep - curStep) * (30 / totalStep) + 40;
                
                if (curStep < totalStep / 2) {
                    pallTransRect(IconTrans);     // pall->rect
                } else {
                    IconTrans.height = IconTrans.width = 30;
                    display.drawRoundRect(IconTrans.x, IconTrans.y,
                                          IconTrans.width, IconTrans.height, RADIUS_RECT, SELECTED_COLOR); 
                    display.drawFastVLine(IconTrans.x + IconTrans.width - 1, IconTrans.y + 2, IconTrans.height - 2, UNSELECTED_COLOR);                    
                    wordGrow(IconTrans);         // grow word-root
                }
            }
        }

        // MainModule right first
        else if (i == forwardPointer + 1) {
            int tmp = IconTrans.x + 10;
            IconTrans.x += (totalStep - curStep) * (30 / totalStep);
            display.drawBitmap(IconTrans.x + 3, IconTrans.y + 3, IconTrans.icon, 24, 24, SELECTED_COLOR);

            if (curStep < totalStep / 2) {
                pallTransRect(IconTrans);     // pall->rect
            } else {
                IconTrans.height = IconTrans.width = 30;
                display.drawRoundRect(IconTrans.x, IconTrans.y,
                                      IconTrans.width, IconTrans.height, RADIUS_RECT, SELECTED_COLOR); 
                display.drawFastVLine(IconTrans.x + IconTrans.width - 1, IconTrans.y + 2, IconTrans.height - 2, UNSELECTED_COLOR);
                wordGrow(IconTrans);         // grow word-root

                if (i == MODULE_FORWARD && curStep > totalStep - 2) {

                    
                    display.drawRoundRect(89, 35, 33, 8, RADIUS_PALL, SELECTED_COLOR);

                    if (curStep == totalStep - 1) {
                        delay(30);
                        display.setCursor(56, 53);
                        display.print(labels[MODULE_FORWARD+1]);

                        display.drawRoundRect(96, 52, 33, 8, RADIUS_PALL, SELECTED_COLOR);
                    }   
                
                    for (int i = 1; i < MODULE_BACKWARD; ++i) {
                        backMartix[i] = backMartix[i - 1] + 1;
                    }
                    backMartix[MODULE_BACKWARD] = MODULE_FORWARD;
                    
                    isBackward = true;
                }
            }
            next = true;
            IconTrans.x = tmp;
        }
        else {
            if (next) {
                IconTrans.x += 40;
                IconTrans.icon = icons[forwardPointer + 2];
                display.drawBitmap(IconTrans.x - 1, IconTrans.y + 3, IconTrans.icon, 24, 24, SELECTED_COLOR);
            }
            drawUnseleModule(IconTrans);
        }
        IconTrans.x += 35;
    }
}

void Menu::drawBackwardModules() {
    IconTrans = {.x = 10, .y = 25, .width = 30, .height = 30, .label = labels[MODULE_FORWARD-1]};  
    display.drawRoundRect(IconTrans.x, IconTrans.y, IconTrans.width, IconTrans.height, RADIUS_RECT, SELECTED_COLOR);

    // 1 - ROOT
    display.drawLine(IconTrans.x + IconTrans.width - 4 , IconTrans.y + IconTrans.height - 1,
                     IconTrans.x + IconTrans.width + 5 , IconTrans.y + IconTrans.height - 10, SELECTED_COLOR);
    display.drawLine(IconTrans.x + IconTrans.width + 5 , IconTrans.y + IconTrans.height - 10,
                     IconTrans.x + IconTrans.width + 25, IconTrans.y + IconTrans.height - 10, SELECTED_COLOR);
    
    int num = i_back;
    for (int cnt = 0; cnt < 3; ++cnt) {
        IconTrans.label = labels[num];
        
        // MIDDLE -> UP
        if (cnt == 0) {  
            int offsetX = curStep;
            int offsetY = 4 * pow(curStep, 0.5);

            display.setCursor(49 + offsetX, 35 - offsetY);
            display.print(IconTrans.label);

            if (curStep < totalStep / 4) {
                display.fillRoundRect(89 + offsetX, 35 - offsetY, 33, 8, RADIUS_PALL, SELECTED_COLOR);
            } else {
                display.drawRoundRect(89 + offsetX, 35 - offsetY, 33, 8, RADIUS_PALL, SELECTED_COLOR);
            }
        }

        // LOW -> MIDDLE
        else if (cnt == 1) { 
            int offsetX = curStep;
            int offsetY = 0.125 * pow(curStep, 2);

            display.setCursor(60 - offsetX, 51 - offsetY);
            display.print(IconTrans.label);

            IconTrans.icon = icons[num];
        
            display.drawRect(IconTrans.x + 3, IconTrans.y + 3, 24, 24, UNSELECTED_COLOR);
            display.drawFastVLine(IconTrans.x + IconTrans.width - 1, IconTrans.y + 2, IconTrans.height - 6, UNSELECTED_COLOR);
            display.drawBitmap(IconTrans.x + 3, IconTrans.y + 3, IconTrans.icon, 24, 24, SELECTED_COLOR);

            if (curStep == totalStep - 1) {
                display.fillRoundRect(100 - offsetX, 50 - offsetY, 33, 8, RADIUS_PALL, SELECTED_COLOR);
            } else {
                display.drawRoundRect(100 - offsetX, 50 - offsetY, 33, 8, RADIUS_PALL, SELECTED_COLOR);
            }
        }

        // OUT -> LOW
        else if (cnt == 2) { 
            int offsetX = curStep;
            int offsetY = 0.125 * pow(curStep, 2);

            display.setCursor(72 - offsetX, 65 - offsetY);
            display.print(IconTrans.label);
            display.drawRoundRect(110 - offsetX, 66 - offsetY, 33, 8, RADIUS_PALL, SELECTED_COLOR);
        }
        ++num;
        if (num == MODULE_MAX) num = backwardPointer;
    }
}

void Menu::wordShrink(Module& icon) {
    float shrinkOffset = 45.0 / wordStep;

    display.fillRect(icon.x + 30 + shrinkOffset * (wordStep - curStep), icon.y + 9, 
                     shrinkOffset * curStep, 20, UNSELECTED_COLOR);
}

void Menu::wordGrow(Module& icon) {
    int growOffset = curStep - wordStep;

    int startX = icon.x + icon.width - 4;
    int startY = icon.y + icon.height - 1;
    
    int endX1 = icon.x + icon.width + 6;
    int endY1 = icon.y + icon.height - 11;

    int currentEndX1 = startX + 20 / wordStep * growOffset;  // (END - START) * 2
    int currentEndY1 = startY - 20 / wordStep * growOffset;

    int currentEndX2 = endX1 + 40 / wordStep * (growOffset - wordStep * 3 / 7); 

    //if (modules[forwardPointer + 1] == icon.label) {
        if (growOffset >= wordStep * 3 / 7) {
            display.drawLine(startX, startY, endX1, endY1, SELECTED_COLOR); 
            display.drawLine(endX1, endY1, currentEndX2, endY1, SELECTED_COLOR);
        }
        else {
            display.drawLine(startX, startY, currentEndX1, currentEndY1, SELECTED_COLOR); 
        }

        if (curStep > totalStep - 4) {
            display.setCursor(icon.x + icon.width + 10, icon.y + 9); 
            display.setTextColor(SELECTED_COLOR);
            display.print(icon.label);
        }
    //}
}

void Menu::pallTrans(Module& icon, int leftTopX, int rightTopX, int rightBottomX, int leftBottomX) {
    // 1 - LEFT-TOP
    display.drawCircleHelper(leftTopX + RADIUS_PALL, icon.y + RADIUS_PALL,
                             RADIUS_PALL, 1, SELECTED_COLOR);

    // 2 - LEFT-BOTTOM -> LEFT-TOP
    display.drawLine(leftBottomX, icon.y + icon.height - RADIUS_PALL,
                     leftTopX, icon.y + RADIUS_PALL, SELECTED_COLOR);    

    // 2 - LEFT-TOP -> RIGHT-TOP
    display.drawLine(leftTopX + RADIUS_PALL, icon.y,
                     rightTopX - RADIUS_PALL, icon.y, SELECTED_COLOR); 

    // 3 - RIGHT-TOP
    display.drawCircleHelper(rightTopX - RADIUS_PALL, icon.y + RADIUS_PALL,
                             RADIUS_PALL, 2, SELECTED_COLOR);

    // 3 - LEFT-BOTTOM
    display.drawCircleHelper(leftBottomX + RADIUS_PALL, icon.y + icon.height - RADIUS_PALL,
                             RADIUS_PALL, 8, SELECTED_COLOR);

    // 4 - RIGHT-TOP -> RIGHT-BOTTOM
    //display.drawLine(rightTopX, icon.y + RADIUS_PALL,
    //                         rightBottomX, icon.y + icon.height - RADIUS_PALL, SELECTED_COLOR); 

    // 4 - RIGHT-BOTTOM -> LEFT-BOTTOM
    display.drawLine(rightBottomX - RADIUS_PALL, icon.y + icon.height,
                             leftBottomX + RADIUS_PALL, icon.y + icon.height, SELECTED_COLOR); 

    // 5 - RIGHT-BOTTOM
    display.drawCircleHelper(rightBottomX - RADIUS_PALL, icon.y + icon.height - RADIUS_PALL,
                             RADIUS_PALL, 4, SELECTED_COLOR);
}

void Menu::pallTransRect(Module& icon) {
    int transOffset = (curStep - wordStep) * (10 / wordStep); 
    icon.height -= 1;

    pallTrans(icon, icon.x - transOffset, icon.x + icon.width,
              icon.x + icon.width + transOffset, icon.x);
}

void Menu::rectTransPall(Module& icon) {
    float transOffset = (float)(curStep - wordStep) / wordStep;
    float progress = easeInOut(transOffset);

    pallTrans(icon, icon.x, icon.x + 30 - 10.0 * progress,
              icon.x + 25 - 15.0 * progress, icon.x - 10.0 * progress);
}

void Menu::renderDynamic(int keyCycleState, bool isForward) {
    if (keyCycleState == LOW) {
        if (CyclePress == 0) { // BEGIN
            CyclePress = currentTime;
        }
        else {
            flowSpeed = ((currentTime - CyclePress) > Threshold) 
                        ? FLOWSPEED_FAST_PLUS  // Long Press
                        : FLOWSPEED_NORMAL;   // Short Press
            isAnimating = true;
            if (isForward) renderForward();
            else           renderBackward();
        }
    } 
    // Stop Press
    else if (keyCycleState == HIGH && CyclePress > 0) {
        CyclePress = 0;
        if (isForward) currentState = IDLE; // FORWARD -> IDLE
        else           currentState = BACKWARD_SELECTED;
    }
}

void Menu::renderForward() {
    curStep = 0;
    PAGE_NAME = forwardPointer == MODULE_FORWARD - 1 ? "BACKWARD" : "FORWARD";

    // Save last Backward
    if (forwardPointer == MODULE_FORWARD) {
        tmpLabel = labels[MODULE_FORWARD];
        tmpIcons = icons[MODULE_FORWARD]; // change Shallow copy to Deep Copy

        labels[MODULE_FORWARD] = labels[i_back];
        icons[MODULE_FORWARD] = icons[i_back];
    }

    while (curStep < totalStep) {
        draw(-45 / totalStep, false, true);
        curStep++;
        delay(flowSpeed);
    }
    
    if (forwardPointer == MODULE_FORWARD) {
        forwardPointer = 0; 
        i_back = backwardPointer;

        labels[MODULE_FORWARD] = tmpLabel;
        icons[MODULE_FORWARD] = tmpIcons;
        
        isBackward = false;
        Icon = {.x = 10, .y = 25, .width = 20, .height = 30};                                                                                                                                   
        draw(0, true, true);
        isAnimating = false;
        return;
    }
    ++forwardPointer;
    isAnimating = false;
}

void Menu::renderBackward() {
    curStep = 0;

    while (curStep < totalStep) {  
        draw(1, false, false);
        curStep++;
        delay(flowSpeed);
    }

    if (i_back == MODULE_MAX - 1) i_back = backwardPointer;
    else ++i_back;
    isAnimating = false;
}

void Menu::backwardTrans() {
    for (int i = 1; i < MODULE_BACKWARD + 1; ++i) {
        backMartix[i - 1] = backMartix[i];
    }

    if (backMartix[MODULE_BACKWARD - 1] == MODULE_MAX - 1) {
        backMartix[MODULE_BACKWARD] = MODULE_BACKWARD;
    } else {
        backMartix[MODULE_BACKWARD] = backMartix[MODULE_BACKWARD - 1] + 1;
    }
}

void Menu::backwardSave() {
    for (int i = 0; i < MODULE_BACKWARD + 1; ++i) {
        backMackup[i] = backMartix[i];
    }
}

void Menu::backwardLoad() {
    for (int i = 0; i < MODULE_BACKWARD + 1; ++i) {
        backMartix[i] = backMackup[i];
    }
}

float Menu::easeInOut(float t) {
    if (t < 0.7) {
        return 2 * t * t;
    } else {
        return -1 + (4 - 2 * t) * t;
    }
}

void module_LIGHT() {
    display.clearDisplay();
    menu.drawTopBar();
    menu.drawFrame();

    display.setCursor(1, 20);
    display.print("LIGHT");
    display.display();
}

void module_TIME() {

}

void module_DHT11() {

}

void module_UICORE() {
    display.clearDisplay();
    PAGE_NAME = "UI-CORE";
    UI_NAME = "WROOM-32";

    menu.drawTopBar();
    menu.drawFrame();

    display.setTextSize(2);
    display.setCursor(8, 25);
    display.print("ESP-32");

    display.setTextSize(1);
    display.setCursor(10, 48);
    display.print("40MHz / 520KB SRAM");
    
    display.display();
}

void module_github() {
    display.clearDisplay();
    PAGE_NAME = "TEST RELEASE";
    UI_NAME = "V0.1";

    menu.drawTopBar();
    menu.drawFrame();

    display.setTextSize(2);
    display.setCursor(8, 25);
    display.print("HinarUI");

    display.drawBitmap(98, 23, bitmap_github, 24, 24, SELECTED_COLOR);
    display.setCursor(10, 48);

    display.setTextSize(1);
    display.print("@890mn ORIGINAL");
    
    display.display();
}

void module_ABOUT() {

}

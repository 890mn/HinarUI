#include "UI.h"
#include "resource/module.h"

void module_serial() {
    display.clearDisplay();
    menu.drawTopBar();
    menu.drawFrame();

    display.setCursor(1, 20);
    display.setTextSize(2);
    display.print("LIGHT");
    display.display();
    display.setTextSize(1);
}

void module_battery() {

}

void module_SHT30() {
    display.clearDisplay();
    menu.drawTopBar();
    menu.drawFrame();

    SET_FONT_MEDIUM;
    float t = SHT.readTemperature();
    float h = SHT.readHumidity();

    display.setCursor(1, 30);
    if (! isnan(t)) {  // check if 'is not a number'
        char* temper = (char*)malloc(20);
        sprintf(temper, "Temper: %.2f", t);
        display.print(temper); 
    } else { 
        display.print("Temper: NaN");
    }
    
    display.setCursor(1, 45);
    if (! isnan(h)) {  // check if 'is not a number'
        char* humi = (char*)malloc(20);
        sprintf(humi, "Humi: %.2f", h);
        display.print(humi);
    } else { 
        display.print("Humi: NaN");
    }

    display.display();
    SET_FONT_DEFAULT;
}

void module_UICORE() {
    display.clearDisplay();
    menu.drawTopBar("UI-CORE", "1of2");
    menu.drawFrame();

    SET_FONT_LARGE;
    display.setCursor(17, 35);
    display.print("ESP32");

    SET_FONT_USMALL;
    display.setCursor(52, 45);
    display.print("ESPRESSIF");

    display.setCursor(52, 55);
    display.print("WROVER");
    display.setTextSize(1);
/*
    SET_FONT_USMALL;
    display.setTextSize(1);
    display.setCursor(8, 53);
    display.print("7/24.8% RAM/Flash");
*/
    SET_FONT_DEFAULT;
    display.drawLine(75, 58, 95, 58, SELECTED_COLOR);
    display.drawLine(95, 58, 105, 63, SELECTED_COLOR);
    display.display();
}

void module_github() {
    display.clearDisplay();
    menu.drawTopBar("VER_RELEASE", "0.2");
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
    display.clearDisplay();
    menu.drawTopBar("SETTING");
    menu.drawFrame();

    display.setTextSize(1);
    display.setCursor(3, 20);
    display.print("FLOW SPEED");
    display.setCursor(70, 20);
    display.print(menu.getFlowSpeed());
    display.display();
}
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

    float t = SHT.readTemperature();
    float h = SHT.readHumidity();

    display.drawBitmap(3, 25, bitmap_temper, 24, 24, SELECTED_COLOR, UNSELECTED_COLOR);
    display.drawBitmap(65, 25, bitmap_humi, 24, 24, SELECTED_COLOR, UNSELECTED_COLOR);
    
    display.fillRect(32, 25, 30, 24, BLACK);
    display.fillRect(93, 25, 30, 24, BLACK);

    display.setCursor(32, 29);
    if (!isnan(t)) {
        display.printf("%.2f", t);
    } else {
        display.print("NaN");
    }
    display.setCursor(32, 39);
    display.printf("C");

    display.setCursor(93, 29);
    if (!isnan(h)) {
        display.printf("%.2f", h);
    } else {
        display.print("NaN");
    }
    display.setCursor(93, 39);
    display.printf("%%");

    display.setCursor(52, 57);
    display.printf("SHT30");

    display.display();
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
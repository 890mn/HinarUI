#include "UI.h"
#include "resource/module.h"

void module_LIGHT() {
    display.clearDisplay();
    menu.drawTopBar();
    menu.drawFrame();

    display.setCursor(1, 20);
    display.setTextSize(2);
    display.print("LIGHT");
    display.display();
    display.setTextSize(1);
}

void module_TIME() {
    // Wait for DS3231
    while (true) {
        display.clearDisplay();
        menu.drawTopBar();
        menu.drawFrame();
        uint32_t total_seconds = RTC_Time();
    
        uint32_t hours = total_seconds / 3600;
        uint32_t minutes = (total_seconds % 3600) / 60;
        uint32_t seconds = total_seconds % 60;

        display.fillRect(17, 30, 100, 8, BLACK);
        display.setCursor(17, 30);
        display.setTextSize(2);
        display.printf("%02d:%02d:%02d", hours, minutes, seconds);

        display.display();
        display.setTextSize(1);
        if (digitalRead(KEY_BACK) == LOW) break;
    }
}

void module_DHT11() {
    display.clearDisplay();
    menu.drawTopBar();
    menu.drawFrame();

    display.setCursor(1, 20);
    display.setTextSize(2);
    display.print("DHT11");
    display.display();
    display.setTextSize(1);
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
    display.clearDisplay();
    PAGE_NAME = "SETTING";

    menu.drawTopBar();
    menu.drawFrame();

    display.setTextSize(1);
    display.setCursor(3, 20);
    display.print("FLOW SPEED");
    display.setCursor(70, 20);
    display.print(menu.getFlowSpeed());

    display.setCursor(3, 30);
    display.print("BOOT TIME");

    while (true) {
        uint32_t Boot = RTC_Time();

        display.fillRect(70, 30, 50, 8, BLACK);
        display.setCursor(70, 30);
        display.print(Boot);
        display.setCursor(100, 30);
        display.print("Sec");

        display.display();
        if (digitalRead(KEY_BACK) == LOW) break;
    }
}
#include "Profile.h"
#include "HinarUI_Core.h"

#include "HinarUI/core/FrameBufferManager.h"

int module_UICORE_page = 0;
const int module_UICORE_totalPages = 2;

void module_UICORE() {
    frameBuffer.beginFrame();
    display.clearDisplay();
    Serial.println("UI-CORE module loaded");
    char buf[16];
    sprintf(buf, "%dof%d", module_UICORE_page + 1, module_UICORE_totalPages);
    menu.drawTopBar("UICore", buf);
    menu.drawFrame();
    switch (module_UICORE_page) {
        case 0:
            SET_FONT_LARGE;
            display.setCursor(17, 35);
            display.print("ESP32");

            SET_FONT_USMALL;
            display.setCursor(52, 45);
            display.print("ESPRESSIF");

            display.setCursor(52, 55);
            display.print("WROVER");

            display.drawLine(75, 58, 95, 58, SELECTED_COLOR);
            display.drawLine(95, 58, 105, 63, SELECTED_COLOR);
            break;
        case 1:
            display.setCursor(5, 23);
            display.print("VSCode/PlatformIO");
            display.setCursor(5, 37);
            display.print("SSD1306/Adafruit_GFX");
            display.setCursor(5, 51);
            display.print("Adafruit_SHT31");
            break;
    }
    SET_FONT_DEFAULT;
    frameBuffer.endFrame();
}

void module_github() {
    frameBuffer.beginFrame();
    display.clearDisplay();
    menu.drawTopBar("VER_REFACTOR", "0.4");
    menu.drawFrame();

    display.setTextSize(2);
    display.setCursor(8, 25);
    display.print("HinarUI");

    display.drawBitmap(98, 23, bitmap_github, 24, 24, SELECTED_COLOR);
    display.setCursor(10, 48);

    display.setTextSize(1);
    display.print("@890mn ORIGINAL");
    
    frameBuffer.endFrame();
}

void module_ABOUT() {
    frameBuffer.beginFrame();
    display.clearDisplay();
    menu.drawTopBar("SETTING");
    menu.drawFrame();

    display.setTextSize(1);
    display.setCursor(3, 20);
    display.print("FLOW SPEED");
    display.setCursor(70, 20);
    display.print(menu.getFlowSpeed());

    display.setCursor(3, 30);
    display.print("STEP_COUNT");
    display.setCursor(70, 30);
    display.print(menu.getStepCount());

    display.setCursor(3, 40);
    display.print("DEV MODE");
    display.setCursor(70, 40);
    display.print(menu.isDeveloperModeEnabled() ? "ON" : "OFF");

    display.setCursor(3, 50);
    display.print("FPS");
    display.setCursor(70, 50);
    display.print(perf.currentFps(), 1);

    display.setCursor(3, 40);
    display.print("OLED  I2C");
    display.setCursor(70, 40);
    display.printf("0x%02X", OLED_ADDR);

    display.setCursor(3, 50);
    display.print("SHT30 I2C");
    display.setCursor(70, 50);
    display.printf("0x%02X", SHT30_ADDR);

    frameBuffer.endFrame();
}

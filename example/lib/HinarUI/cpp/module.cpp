#include "UI.h"
#include "resource/asset.h"
#include "resource/module.h"

int module_UICORE_page = 0;
const int module_UICORE_totalPages = 2;

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

    display.setCursor(3, 30);
    display.print("STEP_COUNT");
    display.setCursor(70, 30);
    display.print(STEP_COUNT);

    display.setCursor(3, 40);
    display.print("OLED  I2C");
    display.setCursor(70, 40);
    display.printf("0x%02X", OLED_ADDR);

    display.setCursor(3, 50);
    display.print("SHT30 I2C");
    display.setCursor(70, 50);
    display.printf("0x%02X", SHT30_ADDR);

    display.display();
}
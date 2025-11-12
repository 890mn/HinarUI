#include "profile.h"
#include "HinarUI_Core.h"

float readBatteryVoltage() {
    int raw = analogRead(VBAT_PIN);
    float voltage = raw * 3.3 / 4095.0 * 2.0 + 0.25;
    return voltage;
}

int calcBatteryPercent(float voltage) {
    float percent = 100.0 / (1 + exp(-20 * (voltage - 3.7)));
    if (percent > 100) return 100;
    if (percent < 0) return 0;
    return (int)percent;
}

void module_battery() {
    display.clearDisplay();
    menu.drawTopBar("BATTERY", "STATUS");
    menu.drawFrame();
    SET_FONT_MEDIUM;

    float voltage = readBatteryVoltage();
    int percent = calcBatteryPercent(voltage);

    int x = 10, y = 30, width = 40, height = 20;
    display.drawRect(x, y, width, height, WHITE);
    display.fillRect(x + width, y + height / 4, 4, height / 2, WHITE);

    int fillWidth = (width - 2) * percent / 100;
    display.fillRect(x + 1, y + 1, fillWidth, height - 2, percent < 20 ? WHITE : SELECTED_COLOR);

    display.setCursor(60, 35);
    display.printf("%.2fV", voltage);

    display.setCursor(60, 50);
    display.printf("%d%%", percent);

    display.display();
    SET_FONT_DEFAULT;
}
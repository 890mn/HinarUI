#include "Profile.h"
#include "HinarUI_Core.h"
#include "HinarUI/core/BatteryMath.h"
#include "modules/module_vbat.h"

float readBatteryVoltage() {
    return HinarUI::batteryVoltageFromRaw(analogRead(VBAT_PIN));
}

int calcBatteryPercent(float voltage) {
    return HinarUI::batteryPercentFromVoltage(voltage);
}

BatteryStatus batteryReadStatus(unsigned long now, unsigned long intervalMs, bool force) {
    static unsigned long lastReadMs = 0;
    static BatteryStatus cached{0.0f, 0, false};
    constexpr int chargeAdcThreshold = 400;
    if (force || now - lastReadMs >= intervalMs) {
        cached.voltage = readBatteryVoltage();
        cached.percent = calcBatteryPercent(cached.voltage);
        int chargeAdc = analogRead(CHARGE_SENSE_PIN);
        cached.charging = chargeAdc > chargeAdcThreshold;
        lastReadMs = now;
    }
    return cached;
}

void module_battery() {
    frameBuffer.beginFrame();
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

    frameBuffer.endFrame();
    SET_FONT_DEFAULT;
}

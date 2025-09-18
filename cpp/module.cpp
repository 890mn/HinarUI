#include "UI.h"
#include "resource/asset.h"
#include "resource/module.h"
#include <deque>

int module_UICORE_page = 0;
const int module_UICORE_totalPages = 2;

std::deque<String> comLogs;
const size_t MAX_LOG_LINES = 4;
int first_serial = 0;

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

void module_serial() {
    display.clearDisplay();
    char config[16];
    sprintf(config, "TX%d/RX%d", COM_TX_PIN, COM_RX_PIN);
    menu.drawTopBar(config, (String)COM_BAUD_RATE);
    menu.drawFrame();
    if (first_serial == 0) {
        comLogs.push_back("Wait for Data...");
        ++first_serial;
    }
    SET_FONT_USMALL;

    while (Serial1.available()) {
        if (first_serial == 1) {
            comLogs.clear();
            ++first_serial;
        }
        String line = Serial1.readStringUntil('\n');
        line.trim();
        if (line.length() > 0) {
            if (comLogs.size() >= MAX_LOG_LINES) {
                comLogs.pop_front();
            }
            comLogs.push_back(line);
        }
    }

    for (size_t i = 0; i < comLogs.size(); ++i) {
        display.setCursor(3, 25 + i * 10);
        display.print(comLogs[i]);
    }

    display.display();
    SET_FONT_DEFAULT;
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
    menu.drawTopBar("VER_RELEASE", "0.3");
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
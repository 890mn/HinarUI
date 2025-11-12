#include "profile.h"
#include "HinarUI_Core.h"
#include <deque>

std::deque<String> comLogs;
const size_t MAX_LOG_LINES = 4;
int first_serial = 0;

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
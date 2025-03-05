#include "resource/asset.h"

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
RTC_DATA_ATTR uint64_t rtc_time_start = 0;

bool OLED_Setup() {
    Wire.begin(OLED_SDA, OLED_SCL);

    if (!display.begin(SSD1306_SWITCHCAPVCC)) {
        Serial.println(F("-! Inital Failed == [ SSD1306 OLED ]"));
        return false;
    }
    display.setTextSize(1);
    display.setTextColor(SELECTED_COLOR);
    display.setCursor(0, 0);

    Serial.println(F("-- Inital Success == [ OLED ]"));
    return true;
}

bool RTC_Setup() {
    if (nvs_flash_init() != ESP_OK) {
        Serial.println(F("-! Inital Failed == [ NVS FLASH ]"));
        return false;
    }
    rtc_time_start = esp_timer_get_time();

    Serial.println(F("-- Inital Success == [ RTC ]"));
    return true;
}

bool KEY_Setup() {
    pinMode(KEY_ENTER, INPUT_PULLUP);
    pinMode(KEY_CYCLE, INPUT_PULLUP);
    pinMode(KEY_BACK , INPUT_PULLUP);

    Serial.println(F("-- Inital Success == [ KEY ]"));
    return true;
}

uint32_t RTC_Time() {
    return (esp_timer_get_time() - rtc_time_start) / 1000000;
}
#include "boards/esp_wrover_kit.h"
#include "Profile.h"

bool ESP_WROVER_KIT_Board_Init(void) {
    Serial.begin(115200);
    
#if defined(MODULE_OLED)
    Serial.println(F("[INFO] SUPPORT OLED Module"));
    if (!ESP_WROVER_KIT_OLED_Setup()) {
        return false;
    }
#endif
#if defined(MODULE_SHT30)
    Serial.println(F("[INFO] SUPPORT SHT30 Module"));
    if (!ESP_WROVER_KIT_SHT30_Setup()) {
        return false;
    }
#endif
#if defined(MODULE_VBAT)
    Serial.println(F("[INFO] SUPPORT VBAT Module"));
    if (!ESP_WROVER_KIT_VBAT_Setup()) {
        return false;
    }
#endif
#if defined(MODULE_COM)
    Serial.println(F("[INFO] SUPPORT SERIAL COMMUNICATION Module"));
    if (!ESP_WROVER_KIT_SERIAL_COMMUNICATION_Setup()) {
        return false;
    }
#endif
    if (!ESP_WROVER_KIT_KEY_Setup()) {
        return false;
    }
    Serial.println(F("[INFO] Inital Success == [ ESP_WROVER_KIT BOARD ]"));
    return true;
}

bool ESP_WROVER_KIT_OLED_Setup(void) {
    Wire.begin(I2C_SDA, I2C_SCL);

    if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
        Serial.println(F("[ERROR] Inital Failed == [ SSD1306 OLED ]"));
        return false;
    }
    display.setTextSize(1);
    display.setTextColor(SELECTED_COLOR);
    display.setCursor(0, 0);
    display.clearDisplay();
    SET_FONT_DEFAULT;
    return true;
}

bool ESP_WROVER_KIT_SHT30_Setup(void) {
    Wire1.begin(SHT30_SDA, SHT30_SCL);

    if (!SHT.begin(SHT30_ADDR)) {
        Serial.println(F("[ERROR] Inital Failed == [ SHT30 ]"));
        return false;
    }
    Serial.print("[SHT30] Heater Enabled State: ");
    if (SHT.isHeaterEnabled())
        Serial.println("ENABLED");
    else
        Serial.println("DISABLED");
    return true;
}

bool ESP_WROVER_KIT_VBAT_Setup(void) {
    pinMode(VBAT_PIN , INPUT_PULLDOWN);
    return true;
}

bool ESP_WROVER_KIT_SERIAL_COMMUNICATION_Setup(void) {
    Serial1.begin(COM_BAUD_RATE, SERIAL_8N1, COM_RX_PIN, COM_TX_PIN);
    return true;
}

bool ESP_WROVER_KIT_KEY_Setup(void) {
    pinMode(KEY_ENTER, INPUT_PULLUP);
    pinMode(KEY_CYCLE, INPUT_PULLUP);
    pinMode(KEY_BACK , INPUT_PULLUP);
    pinMode(KEY_OFF  , INPUT_PULLUP);
    return true;
}
#include "profile.h"

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

bool OLED_Setup(void) {
#if defined(BOARD_ESP_WROVER_KIT)
    Serial.println(F("[INFO] Using ESP_WROVER_KIT OLED Module"));
    return ESP_WROVER_KIT_OLED_Setup();
#endif
    Serial.println(F("[ERROR] Board Not Supported OLED Module"));
    return false;
}

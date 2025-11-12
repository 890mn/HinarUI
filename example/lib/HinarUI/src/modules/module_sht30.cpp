#include "profile.h"
#include "HinarUI_Core.h"

Adafruit_SHT31 SHT = Adafruit_SHT31(&Wire1);

bool SHT30_Setup(void) {
#if defined(BOARD_ESP_WROVER_KIT)
    Serial.println(F("[INFO] Using ESP_WROVER_KIT SHT30 Module"));
    return ESP_WROVER_KIT_SHT30_Setup();
#endif
    Serial.println(F("[ERROR] Board Not Supported SHT30 Module"));
    return false;
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
#include "profile.h"
#include "HinarUI_Core.h"

#include "HinarUI/core/FilteredValue.h"
#include "HinarUI/core/FrameBufferManager.h"

Adafruit_SHT31 SHT = Adafruit_SHT31(&Wire1);

static FilteredValue tempFilter(0.2f, 0.05f);
static FilteredValue humFilter(0.5f, 0.1f);

bool SHT30_Setup(void) {
#if defined(BOARD_ESP_WROVER_KIT)
    Serial.println(F("[INFO] Using ESP_WROVER_KIT SHT30 Module"));
    return ESP_WROVER_KIT_SHT30_Setup();
#endif
    Serial.println(F("[ERROR] Board Not Supported SHT30 Module"));
    return false;
}

void module_SHT30() {
    bool forceFull = frameBuffer.needsFullRefreshHint();
    frameBuffer.beginFrame();

    if (forceFull) {
        display.clearDisplay();
        menu.drawTopBar();
        menu.drawFrame();
        display.drawBitmap(3, 25, bitmap_temper, 24, 24, SELECTED_COLOR, UNSELECTED_COLOR);
        display.drawBitmap(65, 25, bitmap_humi, 24, 24, SELECTED_COLOR, UNSELECTED_COLOR);
    }

    float t = tempFilter.update(SHT.readTemperature());
    float h = humFilter.update(SHT.readHumidity());

    display.fillRect(32, 25, 30, 24, BLACK);
    display.fillRect(93, 25, 30, 24, BLACK);

    display.setCursor(32, 29);
    if (!isnan(t)) {
        display.printf("%.2f", t);
    } else {
        display.print("NaN");
    }
    display.setCursor(32, 39);
    display.print("C");

    display.setCursor(93, 29);
    if (!isnan(h)) {
        display.printf("%.2f", h);
    } else {
        display.print("NaN");
    }
    display.setCursor(93, 39);
    display.print("%");

    frameBuffer.endFrame();
}

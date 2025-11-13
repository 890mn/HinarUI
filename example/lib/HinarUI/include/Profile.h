#ifndef __PROFILE_H
#define __PROFILE_H

#include "fonts/font.h"
#include "icon.h"

#include "modules/module_default.h"

#if defined(BOARD_ESP_WROVER_KIT)
    #include "boards/esp_wrover_kit.h"
#endif

#if defined(MODULE_OLED)
    #include "modules/module_oled.h"
#endif

#if defined(MODULE_SHT30)
    #include "modules/module_sht30.h"
#endif

#if defined(MODULE_VBAT)
    #include "modules/module_vbat.h"
#endif

#if defined(MODULE_COM)
    #include "modules/module_com.h"
#endif

static bool board_init(void) {
    Serial.begin(115200);
#if defined(BOARD_ESP_WROVER_KIT)
    Serial.println(F("[INFO] Using ESP_WROVER_KIT Board"));
    return ESP_WROVER_KIT_Board_Init();
#endif
    Serial.println(F("[ERROR] No Support Board Registered"));
    return false;
}

#endif
#ifndef MODULE_OLED_H
#define MODULE_OLED_H

#include "HinarUI/core/HinarUIDisplay.h"
#include <Wire.h>

#define SCREEN_WIDTH        128
#define SCREEN_HEIGHT       64
#define SELECTED_COLOR      SSD1306_WHITE
#define UNSELECTED_COLOR    SSD1306_BLACK

extern HinarUIDisplay display;

bool OLED_Setup(void);

#endif // MODULE_OLED_H

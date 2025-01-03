#ifndef __ASSET_H
#define __ASSET_H

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "esp_system.h"
#include "nvs_flash.h"
#include "nvs.h"

#define OLED_SDA            21  
#define OLED_SCL            22

#define KEY_ENTER           32
#define KEY_BACK            34 
#define KEY_CYCLE           33

#define SCREEN_WIDTH        128
#define SCREEN_HEIGHT       64
#define SELECTED_COLOR      SSD1306_WHITE
#define UNSELECTED_COLOR    SSD1306_BLACK

bool OLED_Setup();
bool RTC_Setup();
bool KEY_Setup();

uint32_t RTC_Time();

#endif
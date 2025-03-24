#ifndef __ASSET_H
#define __ASSET_H

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_SHT31.h>

#include <Fonts/FreeSans9pt7b.h>
 
#include "esp_system.h"
#include "nvs_flash.h"
#include "nvs.h"

#define OLED_SDA            21
#define OLED_SCL            22
#define OLED_ADDR           0x3C

#define SHT30_SDA           18
#define SHT30_SCL           19
#define SHT30_ADDR          0x44

#define KEY_ENTER           32
#define KEY_BACK            35 
#define KEY_CYCLE           33

#define KEY_BACKUP          15

#define SCREEN_WIDTH        128
#define SCREEN_HEIGHT       64
#define SELECTED_COLOR      SSD1306_WHITE
#define UNSELECTED_COLOR    SSD1306_BLACK

static String PAGE_NAME = "FORWARD";
static String UI_NAME   = "HinarUI";

bool OLED_Setup();
bool RTC_Setup();
bool KEY_Setup();
bool SHT30_Setup();

void drawScaledPixel(int x, int y, float scale);
void drawScaledChar(char c, int x, int y, float scale);
void drawScaledText(const char* text, int x, int y, float scale);
uint32_t RTC_Time();

#endif
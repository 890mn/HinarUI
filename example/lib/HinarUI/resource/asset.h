#ifndef __ASSET_H
#define __ASSET_H

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_SHT31.h>
#include <AutoConnect.h>
#include <WiFi.h>
#include <WebServer.h>

#define OLED_SDA            21
#define OLED_SCL            22
#define OLED_ADDR           0x3C

#define SHT30_SDA           18
#define SHT30_SCL           19
#define SHT30_ADDR          0x44

#define KEY_ENTER           32
#define KEY_BACK            35 
#define KEY_CYCLE           33
#define KEY_OFF             15

#define SCREEN_WIDTH        128
#define SCREEN_HEIGHT       64
#define SELECTED_COLOR      SSD1306_WHITE
#define UNSELECTED_COLOR    SSD1306_BLACK

#include "fonts/Arame_Regular5pt7b.h"
#include "fonts/Arame_Regular6pt7b.h"
#include "fonts/Arame_Regular8pt7b.h"
#include "fonts/Arame_Regular10pt7b.h"

#define SET_FONT_USMALL  display.setFont(&Arame_Regular5pt7b);
#define SET_FONT_SMALL   display.setFont(&Arame_Regular6pt7b);
#define SET_FONT_MEDIUM  display.setFont(&Arame_Regular8pt7b);
#define SET_FONT_LARGE   display.setFont(&Arame_Regular10pt7b);
#define SET_FONT_DEFAULT display.setFont();

static String PAGE_NAME = "FORWARD";
static String UI_NAME   = "HinarUI";

bool OLED_Setup();
bool KEY_Setup();
bool SHT30_Setup();
bool WIFI_Setup();

uint32_t RTC_Time();

#endif
#ifndef __MODULE_H
#define __MODULE_H

#include "esp_system.h"
#include "nvs_flash.h"
#include "nvs.h"

#define MODULE_MAX          6
#define MODULE_OFFSET       45
#define MODULE_DIRECTION    -1
#define MODULE_FORWARD      3
#define MODULE_BACKWARD     MODULE_MAX - MODULE_FORWARD

#define RADIUS_PALL         2
#define RADIUS_RECT         5

void module_LIGHT();
void module_TIME();
void module_DHT11();
void module_UICORE();
void module_github();
void module_ABOUT();

void RTC_Setup();
uint32_t RTC_Time();

#endif
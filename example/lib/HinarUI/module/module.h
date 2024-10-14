#ifndef __MODULE_H
#define __MODULE_H

#include "resource/oled.h"
#include "resource/icon.h"

#define MODULE_MAX          6
#define MODULE_OFFSET       45
#define MODULE_DIRECTION    -1
#define MODULE_FORWARD      3
#define MODULE_BACKWARD     MODULE_MAX - MODULE_FORWARD

#define RADIUS_PALL         2
#define RADIUS_RECT         5

typedef void (*entry)();

extern String labels[MODULE_MAX];
extern unsigned char* icons[MODULE_MAX];
extern entry modules[MODULE_MAX];

void module_LIGHT();
void module_TIME();
void module_DHT11();
void module_UICORE();
void module_github();
void module_ABOUT();

#endif
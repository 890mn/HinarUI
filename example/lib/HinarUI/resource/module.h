#ifndef __MODULE_H
#define __MODULE_H

#define MODULE_MAX          6
#define MODULE_OFFSET       45
#define MODULE_DIRECTION    -1
#define MODULE_FORWARD      3
#define MODULE_BACKWARD     MODULE_MAX - MODULE_FORWARD

#define RADIUS_PALL         2
#define RADIUS_RECT         5

void module_serial();
void module_battery();
void module_SHT30();
void module_UICORE();
void module_github();
void module_ABOUT();

extern int module_UICORE_page;
extern const int module_UICORE_totalPages;
#endif
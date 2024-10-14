#include "module.h"

String labels[MODULE_MAX] = {"LIGHT", "TIME", "DHT11",
                             "UICORE", "GITHUB", "ABOUT"};
    
unsigned char* icons[MODULE_MAX] = {bitmap_diode, bitmap_clock, bitmap_data, 
                                    bitmap_chip, bitmap_github, bitmap_cube};

entry modules[MODULE_MAX] = {module_LIGHT, module_TIME, module_DHT11,
                             module_UICORE, module_github, module_ABOUT};

void module_LIGHT() {

}

void module_TIME() {

}

void module_DHT11() {

}

void module_UICORE() {

}

void module_github() {
    display.print("GITHUB");
    display.display();
}

void module_ABOUT() {

}

#include "module.h"

String labels[MODULE_MAX] = {"LIGHT", "TIME", "DHT11",
                             "UICORE", "GITHUB", "ABOUT"};
    
unsigned char* icons[MODULE_MAX] = {bitmap_diode, bitmap_clock, bitmap_data, 
                                    bitmap_chip, bitmap_github, bitmap_cube};

void module_github() {
    display.print("GITHUB");
    display.display();
}

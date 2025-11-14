#include <Arduino.h>
#include "HinarUI_Core.h"

void setup() {
    menu.create();
    perf.begin();
}

void loop() {
    menu.loop();
    perf.loop();
}
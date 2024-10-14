#include <Arduino.h>
#include "UI.h"

Menu menu;

void setup() {
    menu.create();
}

void loop() {
    menu.loop();
}

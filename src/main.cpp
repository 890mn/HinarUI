#include <Arduino.h>
#include "UI.h"

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

Menu menu;

void setup() {
    menu.create();
}

void loop() {
    menu.loop();
}

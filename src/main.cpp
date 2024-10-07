#include <Arduino.h>
#include "UI.h"

Menu menu;

extern int KEY_UP;
extern int KEY_RIGHT;
extern Adafruit_SSD1306 display;

void setup() {
    Serial.begin(115200);

    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
        Serial.println(F("SSD1306 allocation failed"));
        while (1);
    }

    pinMode(KEY_UP, INPUT_PULLUP);
    pinMode(KEY_RIGHT, INPUT_PULLUP);

    menu.create();
}

void loop() {
    menu.loop();
}

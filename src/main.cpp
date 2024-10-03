#include <Arduino.h>
#include "UI.h"

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
Menu menu;

const int KEY_LEFT  = 18;
const int KEY_RIGHT = 19;

bool          isAnimating     = false;
unsigned long currentTime     = 0;
unsigned long lastButtonPress = 0;

void setup() {
    Serial.begin(115200);

    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
        Serial.println(F("SSD1306 allocation failed"));
        while (1);
    }

    pinMode(KEY_LEFT, INPUT_PULLUP);
    pinMode(KEY_RIGHT, INPUT_PULLUP);

    menu.init();
}

void loop() {
    currentTime = millis();

    if (!isAnimating && (currentTime - lastButtonPress) > DEBOU_DELAY) {
        int keyLeftState = digitalRead(KEY_LEFT);
        int keyRightState = digitalRead(KEY_RIGHT);
        int currentPointer = menu.getModuleNum();

        if (keyLeftState == LOW) {
            isAnimating = true;
            if (currentPointer < 0) currentPointer = MODULE_MAX - 1;
            menu.animateSelection(false);
            menu.updatePointer(--currentPointer);        
            lastButtonPress = currentTime;
        }
        else if (keyRightState == LOW) {
            isAnimating = true;
            if (currentPointer > MODULE_MAX - 1) currentPointer = 0;
            menu.animateSelection(true);
            menu.updatePointer(++currentPointer);
            lastButtonPress = currentTime;
        }
    }
    menu.draw(0);
}

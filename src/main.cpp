#include <Arduino.h>
#include "UI.h"

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
Menu menu;

const int KEY_PRESS  = 18;
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

    pinMode(KEY_PRESS, INPUT_PULLUP);
    pinMode(KEY_RIGHT, INPUT_PULLUP);

    menu.init();
}

void loop() {
    currentTime = millis();

    if (!isAnimating && (currentTime - lastButtonPress) > 200) {
        int keyPressState = digitalRead(KEY_PRESS);
        int keyRightState = digitalRead(KEY_RIGHT);

        if (keyPressState == LOW) {
            //isAnimating = true;
            //menu.animateSelection();    
            //lastButtonPress = currentTime;
        }
        else if (keyRightState == LOW) {
            isAnimating = true;
            menu.renderStart();
            lastButtonPress = currentTime;
        }
    }
}

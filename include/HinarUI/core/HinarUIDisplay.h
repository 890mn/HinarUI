#ifndef HINARUI_DISPLAY_H
#define HINARUI_DISPLAY_H

#include <Adafruit_SSD1306.h>

class HinarUIDisplay : public Adafruit_SSD1306 {
public:
    using Adafruit_SSD1306::Adafruit_SSD1306;

    void flushRegion(int16_t x, int16_t y, int16_t width, int16_t height,
                     const uint8_t* sourceBuffer);
};

#endif

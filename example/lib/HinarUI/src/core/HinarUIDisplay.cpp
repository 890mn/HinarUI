#include "HinarUI/core/HinarUIDisplay.h"

#include <Wire.h>

void HinarUIDisplay::flushRegion(int16_t x, int16_t y, int16_t width,
                                 int16_t height, const uint8_t* sourceBuffer) {
    if (width <= 0 || height <= 0) return;
    if (!wire) {
        Adafruit_SSD1306::display();
        return;
    }

    if (x < 0) {
        width += x;
        x = 0;
    }
    if (y < 0) {
        height += y;
        y = 0;
    }
    if (x + width > WIDTH) {
        width = WIDTH - x;
    }
    if (y + height > HEIGHT) {
        height = HEIGHT - y;
    }
    if (width <= 0 || height <= 0) return;

    int16_t pageStart = y / 8;
    int16_t pageEnd = (y + height - 1) / 8;
    int16_t columnEnd = x + width - 1;

    ssd1306_command(SSD1306_PAGEADDR);
    ssd1306_command(pageStart);
    ssd1306_command(pageEnd);

    ssd1306_command(SSD1306_COLUMNADDR);
    ssd1306_command(x);
    ssd1306_command(columnEnd);

    for (int16_t page = pageStart; page <= pageEnd; ++page) {
        wire->beginTransmission(i2caddr);
        wire->write((uint8_t)0x40);
        int16_t offset = page * WIDTH + x;
        for (int16_t col = 0; col < width; ++col) {
            wire->write(sourceBuffer[offset + col]);
        }
        wire->endTransmission();
    }
}

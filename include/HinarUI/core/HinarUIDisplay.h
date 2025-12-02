#ifndef HINARUI_DISPLAY_H
#define HINARUI_DISPLAY_H

#include <Adafruit_SSD1306.h>

class HinarUIDisplay : public Adafruit_SSD1306 {
public:
    using Adafruit_SSD1306::Adafruit_SSD1306;

    bool begin(uint8_t switchvcc = SSD1306_SWITCHCAPVCC, uint8_t i2caddr = 0,
               bool reset = true, bool periphBegin = true);

    void commitFrame();
    void flushRegion(int16_t x, int16_t y, int16_t width, int16_t height,
                     const uint8_t* sourceBuffer);

    // Frame pool helpers
    uint8_t* acquireFrameBuffer();
    void releaseFrameBuffer(uint8_t* buf);
    void useExternalBuffer(uint8_t* buf) { if (buf) buffer = buf; }

    void setDisplayPower(bool on);
    bool isDisplayOn() const { return displayOn_; }

private:
    void freeBuffers();

    uint8_t* buffers_[2] = {nullptr, nullptr};
    size_t bufferSize_ = 0;
    uint8_t activeIndex_ = 0;
    bool usingPSRAM_ = false;
    bool displayOn_ = true;
};

#endif

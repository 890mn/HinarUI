#include "HinarUI/core/HinarUIDisplay.h"

#include <Wire.h>
#include <cstring>

#if defined(ESP32)
#include <esp_heap_caps.h>
#endif

namespace {
void* allocatePsram(size_t size) {
#if defined(ESP32)
    return heap_caps_malloc(size, MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);
#else
    return malloc(size);
#endif
}

void freePsram(void* ptr) {
    if (!ptr) return;
#if defined(ESP32)
    heap_caps_free(ptr);
#else
    free(ptr);
#endif
}
}  // namespace

bool HinarUIDisplay::begin(uint8_t switchvcc, uint8_t i2caddr, bool reset,
                           bool periphBegin) {
    bool ok = Adafruit_SSD1306::begin(switchvcc, i2caddr, reset, periphBegin);
    if (!ok) return false;

    uint8_t* defaultBuffer = buffer;
    freeBuffers();
    bufferSize_ = WIDTH * ((HEIGHT + 7) / 8);
    buffers_[0] = static_cast<uint8_t*>(allocatePsram(bufferSize_));
    buffers_[1] = static_cast<uint8_t*>(allocatePsram(bufferSize_));

    if (buffers_[0] && buffers_[1]) {
        usingPSRAM_ = true;
        memset(buffers_[0], 0, bufferSize_);
        memset(buffers_[1], 0, bufferSize_);
        activeIndex_ = 0;
        if (defaultBuffer) {
            free(defaultBuffer);
        }
        buffer = buffers_[activeIndex_];
    } else {
        usingPSRAM_ = false;
        freeBuffers();
        buffer = defaultBuffer;
    }
    return true;
}

void HinarUIDisplay::commitFrame() {
    if (!usingPSRAM_) {
        Adafruit_SSD1306::display();
        return;
    }

    uint8_t displayIndex = activeIndex_;
    uint8_t* outgoingBuffer = buffers_[displayIndex];

    activeIndex_ ^= 1;
    uint8_t* nextBuffer = buffers_[activeIndex_];
    if (nextBuffer) {
        std::memcpy(nextBuffer, outgoingBuffer, bufferSize_);
        buffer = nextBuffer;
    }

    uint8_t* saved = buffer;
    buffer = outgoingBuffer;
    Adafruit_SSD1306::display();
    buffer = saved;
}

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

void HinarUIDisplay::freeBuffers() {
    for (int i = 0; i < 2; ++i) {
        if (buffers_[i]) {
            freePsram(buffers_[i]);
            buffers_[i] = nullptr;
        }
    }
    usingPSRAM_ = false;
}

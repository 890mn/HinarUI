#pragma once

#include <Arduino.h>

typedef void (*MenuEntry)();

enum class MenuState {
    Idle,
    Forward,
    Backward,
    BackwardSelected,
    Module
};

struct ModuleVisual {
    int x = 0;
    int y = 0;
    int width = 0;
    int height = 0;
    String label = "INIT";
    unsigned char* icon = nullptr;

    ModuleVisual() {}
    ModuleVisual(int xVal, int yVal, int w, int h, const String& text = "INIT", unsigned char* data = 0)
        : x(xVal), y(yVal), width(w), height(h), label(text), icon(data) {}
};

struct ModuleDescriptor {
    String label;
    unsigned char* icon = 0;
    MenuEntry handler = 0;
};

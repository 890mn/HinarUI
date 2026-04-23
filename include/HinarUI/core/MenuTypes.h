#pragma once

#include <Arduino.h>

typedef void (*MenuEntry)();
using MenuIcon = const unsigned char*;

enum class MenuState {
    Idle,
    Forward,
    Backward,
    BackwardSelected,
    Module,
    Sleep
};

struct ModuleVisual {
    int x = 0;
    int y = 0;
    int width = 0;
    int height = 0;
    String label = "INIT";
    MenuIcon icon = nullptr;

    ModuleVisual() {}
    ModuleVisual(int xVal, int yVal, int w, int h, const String& text = "INIT", MenuIcon data = nullptr)
        : x(xVal), y(yVal), width(w), height(h), label(text), icon(data) {}
};

struct ModuleDescriptor {
    String label;
    MenuIcon icon = nullptr;
    MenuEntry handler = nullptr;
};

# HinarUI

Original OLED UI Designed for 2:1

## Current Status

- 2025.01.05 | Board on
- 2024.11.17 | Module init
- 2024.10.14 | Status Machine Rebuild
- 2024.10.12 | Icon on
- 2024.10.09 | base on

## Project Structure

```plaintext
HinarUI
├─ resource
│   ├─ asset.h
│   ├─ icon.h
│   └─ module.h
├─ cpp
│   ├─ asset.cpp
│   └─ module.cpp
├─ example
│   ├─ include
│   ├─ lib
│   ├─ src
│   └─ platformio.ini
├─ UI.h
├─ UI.cpp
└─ README.md
```

## Usage

Try to include it by `#include "HinarUI/UI.h"` and modify the GPIO in `resource/asset.h` by your actual board.

like this:

In `main.cpp`
```cpp
#include"UI.h"

void setup() {
  menu.create();
}

void loop() {
  menu.loop();
}
```

In `resource/asset.h`
```cpp
#define OLED_SDA            21  
#define OLED_SCL            22

#define KEY_ENTER           32
#define KEY_BACK            34 
#define KEY_CYCLE           33

#define SCREEN_WIDTH        128
#define SCREEN_HEIGHT       64
```
Or current best way is to open the [EXAMPLE](/example/README.md) to recognize.

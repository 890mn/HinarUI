![head](pic/HinarUI.jpg)

# HinarUI v4
<img alt="Bilibili" src="https://img.shields.io/badge/Bilibili-EE979F?link=https%3A%2F%2Fspace.bilibili.com%2F45409103"> <img alt="oshwhub" src="https://img.shields.io/badge/%E7%AB%8B%E5%88%9B%E5%BC%80%E6%BA%90%E7%A4%BE%E5%8C%BA-4672F5?link=https%3A%2F%2Foshwhub.com%2Flink_joestar%2Fhinarui-open-source-embedded-sel"> <img alt="MakerWorld" src="https://img.shields.io/badge/MakerWorld-white?link=https%3A%2F%2Fmakerworld.com.cn%2Fzh%2Fmodels%2F1583584-hinarui-shell-v1">  ![GitHub Created At](https://img.shields.io/github/created-at/890mn/HinarUI?labelColor=%2339354A&color=%239BB9B4)
![GitHub last commit](https://img.shields.io/github/last-commit/890mn/HinarUI?labelColor=%2339354A&color=%239BB9B4)
![Static Badge](https://img.shields.io/github/v/release/890mn/HinarUI?color=%23263C53)

Original OLED UI Design based on Embedded Platform, with complete software, hardware and shell design.

基于嵌入式平台的原创 OLED UI 设计，包含软硬件与外壳方案

## Current Status

- **Next Version will come to 1.0 Stable!**
- 2025.12.02 | Full advance of modules and performance (V4)
- 2025.11.14 | Refactor Core of HinarUI
- 2025.09.19 | Add Li Ver of PCB/Shell (V3)
- 2025.05.06 | Create Shell of the board (V2)
- 2025.03.25 | Complete TESTVer.0.2 and PCB
- 2025.01.02 | Draw PCB (imperfect)
- 2024.11.17 | Module init
- 2024.10.14 | Status Machine Rebuild
- 2024.10.12 | Icon on
- 2024.10.09 | base on

## Project Structure

```plaintext
HinarUI
├─ example/              # PlatformIO demo (esp-wrover-kit by default)
│  ├─ include/
│  ├─ lib/HinarUI/       # Linked library for the demo
│  └─ src/
├─ include/              
│  ├─ boards/            # Board-level pin/I2C config
│  │  └─ esp_wrover_kit.h
│  ├─ fonts/             # Built-in fonts + optional.md for custom fonts
│  ├─ HinarUI/           # Core headers
│  └─ modules/
└─ src/                  # Library sources (core + modules)
   ├─ boards/
   ├─ core/
   └─ modules/
```

## Usage

Main Doc on my website: [Link2hinar](https://link2hinar.fun/p/hinarui/) |
Supporting design PCB at [oshwhub](https://oshwhub.com/link_joestar/hinarui-open-source-embedded-sel) |
Shell at [Makerworld](https://makerworld.com.cn/zh/models/1583584-hinarui-shell-v1)

![cover](pic/HinarUI_Cover.jpg)

*One way is to open the [EXAMPLE](/example/README.md) to understand.*

About how to adapt this project on your board, here gives some tips:

- Quick start as a library:
    ```c++
    #include <Arduino.h>
    #include "HinarUI_Core.h"

    void setup() {
        menu.create();
    }

    void loop() {
        menu.loop();
    }
    ```

- Board / pin adaptation:
    - Edit `include/boards/esp_wrover_kit.h` for pins and I2C addresses:
    ```cpp
    static constexpr int I2C_SDA = 21;
    static constexpr int I2C_SCL = 22;
    static constexpr uint8_t OLED_ADDR = 0x3C;
    static constexpr int KEY_ENTER = 32;
    static constexpr int KEY_BACK  = 35;
    // ... KEY_OFF/KEY_CYCLE/VBAT/CHARGE_SENSE etc.
    // SHT30_ADDR, OLED_ADDR 可按需要改为 0x3D/0x45
    ```
    - 其他传感器/模块可参考现有模块的实现
    - 若作为库引入其他工程，只需 `#include "HinarUI_Core.h"` 并确保上述 pins 地址匹配

## Postscript

Thanks for u here whatever this project can or not helps you, any problem please use `issue` or contact me by [Gmail](mailto:linkjoestar402212@gmail.com)

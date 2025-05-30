![head](pic/HinarUI_Cover.jpg)

# HinarUI 
![Bilibili](https://img.shields.io/badge/Bilibili-EE979F?link=https%3A%2F%2Fspace.bilibili.com%2F45409103)
![oshwhub](https://img.shields.io/badge/%E7%AB%8B%E5%88%9B%E5%BC%80%E6%BA%90%E7%A4%BE%E5%8C%BA-4672F5?link=https%3A%2F%2Foshwhub.com%2Flink_joestar%2Fhinarui-open-source-embedded-sel)
![GitHub Created At](https://img.shields.io/github/created-at/890mn/HinarUI?labelColor=%2339354A&color=%239BB9B4)
![GitHub last commit](https://img.shields.io/github/last-commit/890mn/HinarUI?labelColor=%2339354A&color=%239BB9B4)
![Static Badge](https://img.shields.io/github/v/release/890mn/HinarUI?color=%23263C53)

Original OLED UI Design based on Embedded Platform, current exam on PlatformIO + ESP32-WROVER-E, aim to fit on various proportion of Embedded screen

基于嵌入式平台的原创 OLED UI 设计，目前使用 PlatformIO + ESP32-WROVER-E 进行测试，长远计划是适应各种比例的嵌入式屏幕(maybe?)

By the way, all of it just try something i wanna to do...why not LVGL or other solution? Only because im excited about creating something by my own :)

## Current Status

It seems to be a little slow because everyday get so busy :(

- 2025.05.06 | Create Shell of the board (V2)
- 2025.03.25 | Complete TESTVer.0.2 and PCB (Scale with 1:1)
- 2025.01.02 | Draw PCB (imperfect so not here)
- 2024.11.17 | Module init
- 2024.10.14 | Status Machine Rebuild
- 2024.10.12 | Icon on
- 2024.10.09 | base on

## Project Structure

Clone this project down will include these part, the Library "HinarUI" and an Example  

Also include the PCB and Shell
```plaintext
HinarUI
├─ PCB
│   ├─ BOM
│   └─ Gerber
├─ Shell
│   ├─ Box.3mf
│   └─ Top.3mf
├─ example
│   ├─ .pio
│   ├─ include
│   ├─ lib ― HinarUI
│   ├─ src
│   └─ test
├─ resource
│   ├─ asset.h
│   ├─ icon.h
│   └─ module.h
├─ fonts
│   ├─ Arame_Regular[5:10pt]7b.h
│   ├─ Cubano7pt7b.h
│   └─ optional.md
├─ cpp
│   ├─ asset.cpp
│   └─ module.cpp
├─ UI.h
└─ UI.cpp
```

## Usage

Optional: Supporting design board at [oshwhub](https://oshwhub.com/link_joestar/hinarui-open-source-embedded-sel)  

![cover](pic/Board.jpg)

*One way is to open the [EXAMPLE](/example/README.md) to understand.*

About how to adapt this project on your board, here gives some tips:

- open `resource/asset.h` and modify these GPIO by your own

    ```cpp
    #define OLED_SDA            21
    #define OLED_SCL            22

    #define SHT30_SDA           18
    #define SHT30_SCL           19

    #define KEY_ENTER           32
    #define KEY_BACK            35 
    #define KEY_CYCLE           33

    #define KEY_BACKUP          15
    ```

- open `resource/asset.h` and modify the OLED / SHT30 I2C address if not Default

    ```cpp
    // Default
    #define OLED_ADDR           0x3C
    #define SHT30_ADDR          0x44
    
    // Another Possible
    #define OLED_ADDR           0x3D
    #define SHT30_ADDR          0x45
    ```

- It can normally run at this time of everything alright

## Postscript

Thanks for u here whatever this project can or not helps you, any problem please use `issue` or contact me by [Gmail](mailto:linkjoestar402212@gmail.com)

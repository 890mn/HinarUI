# Board Adaptation Guide

本文档用于给 HinarUI 新增板级适配，将不同硬件的引脚、I2C 地址、外设初始化和可选模块开关集中放在 `include/boards/` 与 `src/boards/` 中。

## 1. 新增文件

以 `esp_wrover_kit` 为模板，新增两类文件：

```text
include/boards/my_board.h
src/boards/my_board.cpp
```

文件名建议使用小写加下划线，避免空格和中文路径。

## 2. 头文件需要定义什么

在 `include/boards/my_board.h` 中定义板卡支持的模块和引脚。

至少需要：

```cpp
#ifndef MY_BOARD_H
#define MY_BOARD_H

#include <stdint.h>

#define MODULE_OLED
static constexpr int     I2C_SDA      = 21;
static constexpr int     I2C_SCL      = 22;
static constexpr uint8_t OLED_ADDR    = 0x3C;
static constexpr int     OLED_RST_PIN = -1;
bool MY_BOARD_OLED_Setup(void);

static constexpr int     KEY_OFF      = 15;
static constexpr int     KEY_ENTER    = 32;
static constexpr int     KEY_CYCLE    = 33;
static constexpr int     KEY_BACK     = 35;
bool MY_BOARD_KEY_Setup(void);

bool MY_BOARD_Board_Init(void);

#endif
```

如果硬件支持对应模块，再加入：

```cpp
#define MODULE_SHT30
static constexpr int     SHT30_SDA    = 18;
static constexpr int     SHT30_SCL    = 19;
static constexpr uint8_t SHT30_ADDR   = 0x44;
bool MY_BOARD_SHT30_Setup(void);

#define MODULE_VBAT
static constexpr int     VBAT_PIN = 36;
static constexpr int     CHARGE_SENSE_PIN = 13;
bool MY_BOARD_VBAT_Setup(void);

#define MODULE_COM
static constexpr int     COM_TX_PIN = 27;
static constexpr int     COM_RX_PIN = 14;
bool MY_BOARD_SERIAL_COMMUNICATION_Setup(void);

#define MODULE_DAC
static constexpr int     HINARUI_DAC_PIN = 25;
bool MY_BOARD_DAC_Setup(void);
```

## 3. 源文件需要实现什么

在 `src/boards/my_board.cpp` 中实现头文件声明的初始化函数。

示例结构：

```cpp
#include "boards/my_board.h"
#include "Profile.h"

bool MY_BOARD_Board_Init(void) {
#if defined(MODULE_OLED)
    if (!MY_BOARD_OLED_Setup()) return false;
#endif
#if defined(MODULE_SHT30)
    if (!MY_BOARD_SHT30_Setup()) return false;
#endif
#if defined(MODULE_VBAT)
    if (!MY_BOARD_VBAT_Setup()) return false;
#endif
#if defined(MODULE_COM)
    if (!MY_BOARD_SERIAL_COMMUNICATION_Setup()) return false;
#endif
#if defined(MODULE_DAC)
    if (!MY_BOARD_DAC_Setup()) return false;
#endif
    if (!MY_BOARD_KEY_Setup()) return false;
    return true;
}
```

初始化建议：

- OLED：调用 `Wire.begin(I2C_SDA, I2C_SCL)`，再调用 `display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)`。
- SHT30：如果使用第二路 I2C，调用 `Wire1.begin(SHT30_SDA, SHT30_SCL)`，再调用 `SHT.begin(SHT30_ADDR)`。
- VBAT：将 `VBAT_PIN` 设置为输入；如果有充电/USB 检测脚，也显式设置为输入。
- COM：调用 `Serial1.begin(COM_BAUD_RATE, SERIAL_8N1, COM_RX_PIN, COM_TX_PIN)`。
- DAC：ESP32 内置 DAC 只支持 GPIO25 / GPIO26，初始化时建议先 `dacWrite(HINARUI_DAC_PIN, 0)`。
- KEY：确认按键电路是上拉还是下拉，再选择 `INPUT_PULLUP`、`INPUT_PULLDOWN` 或 `INPUT`。

## 4. 接入 Profile.h

在 `include/Profile.h` 中增加板卡选择：

```cpp
#if defined(BOARD_MY_BOARD)
    #include "boards/my_board.h"
#endif
```

并在 `board_init()` 中加入：

```cpp
#if defined(BOARD_MY_BOARD)
    Serial.println(F("[INFO] Using MY_BOARD Board"));
    return MY_BOARD_Board_Init();
#endif
```

## 5. 接入 PlatformIO

在示例工程或测试环境中添加板卡宏：

```ini
build_flags =
  -D BOARD_MY_BOARD
```

如果需要单独的示例环境，可以复制 `example/platformio.ini` 中的 `esp-wrover-kit` 环境，改成新板卡对应的 `board` 和 `build_flags`。

## 6. 适配时优先检查的点

新增板卡后先不要改 `Menu::loop()`、`MenuAnimator`、`FrameBufferManager` 或 `MenuRenderer`。先确认板级适配是否成立：

1. OLED 是否能初始化并显示文字。
2. 按键空闲电平是否正确，按下后是否会变为期望电平。
3. SHT30 地址和 I2C 引脚是否正确。
4. VBAT 分压比例是否与 `BatteryMath.h` 的换算参数一致。
5. 无电池状态下 VBAT ADC 是否会悬空；如果会悬空，建议硬件增加下拉或独立 battery-present 信号。
6. CHARGE/USB 检测脚的高低电平含义是否与代码一致。
7. Serial1 的 TX/RX 是否接反。
8. DAC 是否接在 GPIO25 或 GPIO26。

## 7. 适配后运行测试

先跑不需要硬件的 native 测试：

```powershell
pio test -e native
```

再跑板级 smoke 测试。把 `COMx` 换成实际串口：

```powershell
pio test -e esp-wrover-kit-smoke --upload-port COMx --test-port COMx
```

如果是新板卡，建议复制一个新的 smoke 测试环境，例如：

```ini
[env:my-board-smoke]
platform = espressif32
board = esp32dev
framework = arduino
test_framework = unity
test_filter = test_board_smoke
test_build_src = yes
upload_protocol = esptool
build_flags =
  -D BOARD_MY_BOARD
lib_deps =
  adafruit/Adafruit BusIO @ ^1.17.0
  adafruit/Adafruit SSD1306 @ ^2.5.1
  adafruit/Adafruit GFX Library @ ^1.10.10
  adafruit/Adafruit SHT31 Library @ ^2.2.2
```

## 8. 通过标准

板级 smoke 测试通过只能说明硬件基础适配基本成立，还不能替代完整 UI 测试。测试通过后，还需要手动确认：

- 菜单前进、后退、进入模块是否正常。
- OFF 键息屏和唤醒是否正常。
- 模块页返回菜单后状态是否正确。
- OLED 是否有花屏、残影或刷新不完整。
- SHT30、VBAT、COM、DAC 模块在菜单中显示和交互是否符合预期。

如果 smoke 测试失败，优先修板级引脚和初始化。

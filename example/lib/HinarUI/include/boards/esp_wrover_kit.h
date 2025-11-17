#ifndef ESP_WROVER_KIT_H
#define ESP_WROVER_KIT_H

#include <stdint.h>

// OLED_ESP_WROVER_KIT
#define MODULE_OLED
static constexpr int     I2C_SDA      = 21;
static constexpr int     I2C_SCL      = 22;
static constexpr uint8_t OLED_ADDR    = 0x3C;
static constexpr int     OLED_RST_PIN = -1;
bool ESP_WROVER_KIT_OLED_Setup(void);

// SHT30_ESP_WROVER_KIT
#define MODULE_SHT30
static constexpr int     SHT30_SDA    = 18;
static constexpr int     SHT30_SCL    = 19;
static constexpr uint8_t SHT30_ADDR   = 0x44;
bool ESP_WROVER_KIT_SHT30_Setup(void);

// VBAT_ESP_WROVER_KIT
#define MODULE_VBAT
static constexpr int     VBAT_PIN     = 36;
bool ESP_WROVER_KIT_VBAT_Setup(void);

// Serial_Communication_ESP_WROVER_KIT
#define MODULE_COM
static constexpr int     COM_TX_PIN   = 26;
static constexpr int     COM_RX_PIN   = 25;
bool ESP_WROVER_KIT_SERIAL_COMMUNICATION_Setup(void);

// Keys_ESP_WROVER_KIT
static constexpr int     KEY_OFF      = 15;
static constexpr int     KEY_ENTER    = 32;
static constexpr int     KEY_CYCLE    = 33;
static constexpr int     KEY_BACK     = 35;
bool ESP_WROVER_KIT_KEY_Setup(void);

// Board Initialization
bool ESP_WROVER_KIT_Board_Init(void);

#endif // ESP_WROVER_KIT_H
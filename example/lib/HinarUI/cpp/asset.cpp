#include "resource/asset.h"

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

Adafruit_SHT31 SHT = Adafruit_SHT31(&Wire1);

WebServer Server;
AutoConnect Portal(Server);
AutoConnectConfig config;

bool OLED_Setup() {
    Wire.begin(OLED_SDA, OLED_SCL);

    if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
        Serial.println(F("-! Inital Failed == [ SSD1306 OLED ]"));
        return false;
    }
    display.setTextSize(1);
    display.setTextColor(SELECTED_COLOR);
    display.setCursor(0, 0);
    SET_FONT_DEFAULT;

    Serial.println(F("-- Inital Success == [ OLED ]"));
    return true;
}

bool SHT30_Setup() {
    Wire1.begin(SHT30_SDA, SHT30_SCL);

    if (!SHT.begin(SHT30_ADDR)) {
        Serial.println(F("-! Inital Failed == [ SHT30 ]"));
        return false;
    }

    Serial.print("Heater Enabled State: ");
    if (SHT.isHeaterEnabled())
        Serial.println("ENABLED");
    else
        Serial.println("DISABLED");

    Serial.println(F("-- Inital Success == [ SHT30 ]"));
    return true;
}

bool KEY_Setup() {
    pinMode(KEY_ENTER, INPUT_PULLUP);
    pinMode(KEY_CYCLE, INPUT_PULLUP);
    pinMode(KEY_BACK , INPUT_PULLUP);
    pinMode(KEY_OFF  , INPUT_PULLUP);

    Serial.println(F("-- Inital Success == [ KEY ]"));
    return true;
}

bool WIFI_Setup() {
    config.apid = "ESP-HinarUI";
    config.psk = "12345678";      // SoftAP 密码（用户连接 ESP32 用的）
    config.autoReconnect = true; // 断网后自动重连
    config.hostName = "ESP-WROVER-E-HinarUI";
    Portal.config(config);

    if (Portal.begin()) {
        Serial.println("WiFi connected: " + WiFi.localIP().toString());
        configTime(8 * 3600, 0, "ntp.aliyun.com", "pool.ntp.org");
    } else {
        Serial.println("WiFi setup failed!");
        return false;
    }
    return true;
}

#include <Arduino.h>
#include <math.h>
#include <unity.h>

#include "Profile.h"
#include "HinarUI_Core.h"
#include "HinarUI/core/BatteryMath.h"

namespace {
struct SmokeResult {
    const char* name;
    bool pass;
    char detail[40];
};

constexpr int kMaxResults = 12;
SmokeResult results[kMaxResults];
int resultCount = 0;
bool oledReady = false;
bool smokeDone = false;
unsigned long lastPageMs = 0;
int pageIndex = 0;

void drawResultsPage() {
    if (!oledReady) {
        return;
    }

    constexpr int rowsPerPage = 6;
    int pageCount = (resultCount + rowsPerPage - 1) / rowsPerPage;
    if (pageCount < 1) {
        pageCount = 1;
    }
    if (pageIndex >= pageCount) {
        pageIndex = 0;
    }

    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.print("Smoke ");
    display.print(pageIndex + 1);
    display.print("/");
    display.print(pageCount);

    int start = pageIndex * rowsPerPage;
    int end = start + rowsPerPage;
    if (end > resultCount) {
        end = resultCount;
    }
    for (int i = start; i < end; ++i) {
        display.setCursor(0, 8 + (i - start) * 9);
        display.print(results[i].pass ? "OK " : "NG ");
        display.print(results[i].name);
    }
    display.display();
}

void logCheck(const char* name, bool pass, const char* detail = nullptr) {
    Serial.print(pass ? "[PASS] " : "[FAIL] ");
    Serial.print(name);
    if (detail && detail[0] != '\0') {
        Serial.print(" ");
        Serial.print(detail);
    }
    Serial.println();

    if (resultCount < kMaxResults) {
        results[resultCount].name = name;
        results[resultCount].pass = pass;
        if (detail) {
            snprintf(results[resultCount].detail, sizeof(results[resultCount].detail), "%s", detail);
        } else {
            results[resultCount].detail[0] = '\0';
        }
        ++resultCount;
    }
    drawResultsPage();
}

void assertCheck(const char* name, bool pass) {
    logCheck(name, pass);
    TEST_ASSERT_TRUE_MESSAGE(pass, name);
}

void assertCheckDetail(const char* name, bool pass, const char* detail) {
    logCheck(name, pass, detail);
    TEST_ASSERT_TRUE_MESSAGE(pass, detail);
}
}  // namespace

void setUp(void) {}
void tearDown(void) {}

void test_board_profile_constants_are_valid(void) {
    bool pass = OLED_ADDR == 0x3C &&
                SHT30_ADDR == 0x44 &&
                I2C_SDA != I2C_SCL &&
                SHT30_SDA != SHT30_SCL &&
                COM_TX_PIN != COM_RX_PIN &&
                KEY_ENTER != KEY_CYCLE &&
                KEY_ENTER != KEY_BACK &&
                KEY_ENTER != KEY_OFF &&
                KEY_CYCLE != KEY_BACK &&
                KEY_CYCLE != KEY_OFF &&
                KEY_BACK != KEY_OFF &&
                HINARUI_DAC_PIN != VBAT_PIN &&
                VBAT_PIN >= 32 &&
                VBAT_PIN <= 39 &&
                KEY_BACK >= 34 &&
                KEY_BACK <= 39;
    assertCheck("profile/pins", pass);
}

void test_key_gpio_setup_succeeds(void) {
    assertCheck("keys", ESP_WROVER_KIT_KEY_Setup());
}

void test_key_idle_levels_are_not_stuck_active(void) {
    ESP_WROVER_KIT_KEY_Setup();
    delay(20);
    int enter = digitalRead(KEY_ENTER);
    int cycle = digitalRead(KEY_CYCLE);
    int off = digitalRead(KEY_OFF);
    bool pass = enter == HIGH && cycle == HIGH && off == HIGH;

    char detail[40];
    snprintf(detail, sizeof(detail), "E%d C%d O%d", enter, cycle, off);
    assertCheckDetail("key-idle", pass, detail);
}

void test_oled_setup_and_basic_draw_succeeds(void) {
    bool pass = ESP_WROVER_KIT_OLED_Setup();
    oledReady = pass;
    display.clearDisplay();
    display.setCursor(0, 0);
    display.print("HinarUI Smoke");
    display.display();
    pass = pass && display.width() == 128 && display.height() == 64;
    assertCheck("oled", pass);
}

void test_sht30_setup_and_read_returns_sensor_values(void) {
    bool pass = ESP_WROVER_KIT_SHT30_Setup();
    float minTemp = 1000.0f;
    float maxTemp = -1000.0f;
    float minHum = 1000.0f;
    float maxHum = -1000.0f;

    for (int i = 0; i < 3; ++i) {
        float temp = SHT.readTemperature();
        float hum = SHT.readHumidity();
        pass = pass && !isnan(temp) && !isnan(hum);
        if (!isnan(temp)) {
            minTemp = min(minTemp, temp);
            maxTemp = max(maxTemp, temp);
        }
        if (!isnan(hum)) {
            minHum = min(minHum, hum);
            maxHum = max(maxHum, hum);
        }
        delay(80);
    }

    pass = pass &&
           minTemp >= -20.0f &&
           maxTemp <= 80.0f &&
           minHum >= 0.0f &&
           maxHum <= 100.0f &&
           (maxTemp - minTemp) <= 5.0f &&
           (maxHum - minHum) <= 20.0f;

    char detail[40];
    snprintf(detail, sizeof(detail), "T%.1f-%.1f H%.0f-%.0f", minTemp, maxTemp, minHum, maxHum);
    assertCheckDetail("sht30", pass, detail);
}

void test_vbat_setup_and_adc_range_is_valid(void) {
    bool pass = ESP_WROVER_KIT_VBAT_Setup();
    int raw = analogRead(VBAT_PIN);
    pass = pass &&
           raw >= 0 &&
           raw <= 4095;
    assertCheck("vbat-adc", pass);
}

void test_battery_presence_voltage_is_plausible(void) {
    constexpr int sampleCount = 8;
    constexpr int maxStableSpread = 120;
    int minRaw = 4095;
    int maxRaw = 0;
    long totalRaw = 0;

    for (int i = 0; i < sampleCount; ++i) {
        int raw = analogRead(VBAT_PIN);
        minRaw = min(minRaw, raw);
        maxRaw = max(maxRaw, raw);
        totalRaw += raw;
        delay(10);
    }

    float avgRaw = static_cast<float>(totalRaw) / sampleCount;
    float voltage = HinarUI::batteryVoltageFromRaw(static_cast<int>(avgRaw + 0.5f));
    bool stable = (maxRaw - minRaw) <= maxStableSpread;
    bool plausibleSingleCell = voltage >= 3.0f && voltage <= 4.35f;
    bool notAtRail = minRaw > 50 && maxRaw < 4000;
    bool pass = stable && plausibleSingleCell && notAtRail;

    char detail[48];
    snprintf(detail, sizeof(detail), "bat %.2fV r%d-%d", voltage, minRaw, maxRaw);
    assertCheckDetail("battery", pass, detail);
}

void test_charge_sense_is_active_on_usb_power(void) {
    ESP_WROVER_KIT_VBAT_Setup();
    delay(10);
    int chargeSense = digitalRead(CHARGE_SENSE_PIN);
    bool pass = chargeSense == HIGH;

    char detail[32];
    snprintf(detail, sizeof(detail), "chg%d", chargeSense);
    assertCheckDetail("charge", pass, detail);
}

void test_serial_communication_setup_succeeds(void) {
    bool pass = ESP_WROVER_KIT_SERIAL_COMMUNICATION_Setup();
    Serial1.println("COM OK");
    bool pinsValid = COM_TX_PIN >= 0 &&
                     COM_TX_PIN <= 39 &&
                     COM_RX_PIN >= 0 &&
                     COM_RX_PIN <= 39 &&
                     COM_TX_PIN != COM_RX_PIN;
    assertCheck("serial1", pass && pinsValid);
}

void test_dac_setup_and_force_off_succeeds(void) {
    bool pass = ESP_WROVER_KIT_DAC_Setup();
    module_DAC_ForceOff();
    bool pinValid = HINARUI_DAC_PIN == 25 || HINARUI_DAC_PIN == 26;
    assertCheck("dac", pass && pinValid);
}

void setup() {
    Serial.begin(115200);
    delay(2000);
    UNITY_BEGIN();
    RUN_TEST(test_oled_setup_and_basic_draw_succeeds);
    RUN_TEST(test_board_profile_constants_are_valid);
    RUN_TEST(test_key_gpio_setup_succeeds);
    RUN_TEST(test_key_idle_levels_are_not_stuck_active);
    RUN_TEST(test_sht30_setup_and_read_returns_sensor_values);
    RUN_TEST(test_vbat_setup_and_adc_range_is_valid);
    RUN_TEST(test_battery_presence_voltage_is_plausible);
    RUN_TEST(test_charge_sense_is_active_on_usb_power);
    RUN_TEST(test_serial_communication_setup_succeeds);
    RUN_TEST(test_dac_setup_and_force_off_succeeds);
    UNITY_END();
    smokeDone = true;
    drawResultsPage();
}

void loop() {
    if (!smokeDone || !oledReady || resultCount <= 6) {
        return;
    }
    if (millis() - lastPageMs >= 2500) {
        lastPageMs = millis();
        ++pageIndex;
        drawResultsPage();
    }
}

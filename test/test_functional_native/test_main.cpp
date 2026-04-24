#include <math.h>
#include <unity.h>

#include "HinarUI/core/BatteryMath.h"
#include "HinarUI/core/DacMath.h"
#include "HinarUI/core/FilteredValue.h"
#include "HinarUI/core/MenuConfig.h"
#include "boards/esp_wrover_kit.h"

void setUp(void) {}
void tearDown(void) {}

void test_battery_voltage_conversion_matches_divider_model(void) {
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.27f, HinarUI::batteryVoltageFromRaw(0));
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 3.57f, HinarUI::batteryVoltageFromRaw(2048));
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 6.87f, HinarUI::batteryVoltageFromRaw(4095));
}

void test_battery_percent_is_clamped_and_monotonic(void) {
    TEST_ASSERT_EQUAL_INT(0, HinarUI::batteryPercentFromVoltage(2.8f));
    TEST_ASSERT_TRUE(HinarUI::batteryPercentFromVoltage(3.6f) <
                     HinarUI::batteryPercentFromVoltage(3.8f));
    TEST_ASSERT_EQUAL_INT(100, HinarUI::batteryPercentFromVoltage(4.3f));
}

void test_filtered_value_suppresses_small_jitter(void) {
    FilteredValue filter(0.2f, 0.05f);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 25.0f, filter.update(25.0f));
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 25.0f, filter.update(25.10f));
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 25.30f, filter.update(25.30f));
}

void test_dac_voltage_clamps_to_supported_range(void) {
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, HinarUI::clampDacVoltage(-0.1f));
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 1.25f, HinarUI::clampDacVoltage(1.25f));
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 2.0f, HinarUI::clampDacVoltage(3.0f));
}

void test_dac_code_matches_esp32_dac_range(void) {
    TEST_ASSERT_EQUAL_UINT8(0, HinarUI::dacValueFromVoltage(0.0f));
    TEST_ASSERT_EQUAL_UINT8(77, HinarUI::dacValueFromVoltage(1.0f));
    TEST_ASSERT_EQUAL_UINT8(155, HinarUI::dacValueFromVoltage(2.0f));
    TEST_ASSERT_EQUAL_UINT8(155, HinarUI::dacValueFromVoltage(3.0f));
}

void test_menu_config_keeps_forward_backward_window_consistent(void) {
    MenuConfig config = DefaultMenuConfig();
    TEST_ASSERT_EQUAL_INT(7, config.moduleMax);
    TEST_ASSERT_EQUAL_INT(4, config.moduleForward);
    TEST_ASSERT_EQUAL_INT(3, config.moduleBackward());
    TEST_ASSERT_TRUE(config.moduleForward < config.moduleMax);
    TEST_ASSERT_TRUE(config.stepCount > 0);
}

void test_esp_wrover_kit_pin_map_has_no_key_conflicts(void) {
    TEST_ASSERT_NOT_EQUAL(KEY_OFF, KEY_ENTER);
    TEST_ASSERT_NOT_EQUAL(KEY_OFF, KEY_CYCLE);
    TEST_ASSERT_NOT_EQUAL(KEY_OFF, KEY_BACK);
    TEST_ASSERT_NOT_EQUAL(KEY_ENTER, KEY_CYCLE);
    TEST_ASSERT_NOT_EQUAL(KEY_ENTER, KEY_BACK);
    TEST_ASSERT_NOT_EQUAL(KEY_CYCLE, KEY_BACK);
}

void test_esp_wrover_kit_peripheral_pins_are_in_expected_ranges(void) {
    TEST_ASSERT_EQUAL_UINT8(0x3C, OLED_ADDR);
    TEST_ASSERT_EQUAL_UINT8(0x44, SHT30_ADDR);
    TEST_ASSERT_EQUAL_INT(25, HINARUI_DAC_PIN);
    TEST_ASSERT_TRUE(VBAT_PIN >= 32 && VBAT_PIN <= 39);
    TEST_ASSERT_TRUE(CHARGE_SENSE_PIN >= 0 && CHARGE_SENSE_PIN <= 39);
}

int main(int argc, char** argv) {
    (void)argc;
    (void)argv;

    UNITY_BEGIN();
    RUN_TEST(test_battery_voltage_conversion_matches_divider_model);
    RUN_TEST(test_battery_percent_is_clamped_and_monotonic);
    RUN_TEST(test_filtered_value_suppresses_small_jitter);
    RUN_TEST(test_dac_voltage_clamps_to_supported_range);
    RUN_TEST(test_dac_code_matches_esp32_dac_range);
    RUN_TEST(test_menu_config_keeps_forward_backward_window_consistent);
    RUN_TEST(test_esp_wrover_kit_pin_map_has_no_key_conflicts);
    RUN_TEST(test_esp_wrover_kit_peripheral_pins_are_in_expected_ranges);
    return UNITY_END();
}

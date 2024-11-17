#include "UI.h"

RTC_DATA_ATTR uint64_t rtc_time_start = 0;

void RTC_Setup() {
    nvs_flash_init();
    rtc_time_start = esp_timer_get_time();
}

uint32_t RTC_Time() {
    return (esp_timer_get_time() - rtc_time_start) / 1000000;
}
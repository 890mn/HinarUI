#ifndef MODULE_VBAT_H
#define MODULE_VBAT_H

struct BatteryStatus {
    float voltage;
    int percent;
};

float readBatteryVoltage();
int calcBatteryPercent(float voltage);
BatteryStatus batteryReadStatus(unsigned long now, unsigned long intervalMs = 5000, bool force = false);

#endif

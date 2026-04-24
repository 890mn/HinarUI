# HinarUI Functional Tests

These tests are split into two levels so board adaptation can be checked before
manual UI validation.

## Native Functional Tests

Run on the development machine. They do not need ESP32 hardware.

```powershell
pio test -e native
```

Covered items:

- battery ADC to voltage conversion
- battery percentage clamp and monotonic behavior
- SHT30 value jitter filtering
- DAC voltage clamp and DAC code conversion
- menu window configuration
- ESP-WROVER-KIT pin-map sanity checks

## Board Smoke Tests

Run on a connected ESP-WROVER-KIT compatible board. Replace `COMx` with the
actual serial port.

```powershell
pio test -e esp-wrover-kit-smoke --upload-port COMx
```

Covered items are printed through the Unity serial log. After OLED setup
succeeds, the OLED shows a compact final `OK` / `NG` summary. If the result list
does not fit on one 128x64 screen, it rotates through summary pages every 2.5
seconds.

Covered items and pass rules:

- `oled`: SSD1306 setup succeeds and the display reports 128x64.
- `profile/pins`: OLED/SHT30 I2C addresses match the board profile, paired
  pins are not equal, key pins are not duplicated, VBAT uses an ADC-capable
  input-only pin, and `KEY_BACK` is on an input-only GPIO as expected.
- `keys`: key GPIO setup returns success.
- `key-idle`: ENTER, CYCLE, and OFF read HIGH after setup, meaning they are not
  stuck active. Release all keys before running this test.
- `sht30`: setup succeeds, three temperature/humidity reads are not NaN,
  temperature is between -20 C and 80 C, humidity is between 0% and 100%, and
  the short-read spread is limited to 5 C / 20% RH.
- `vbat-adc`: VBAT setup succeeds and the ADC raw value is inside 0..4095.
- `battery`: 8 VBAT ADC samples are stable, not near ADC rails, and convert to
  a plausible single-cell Li-ion voltage of 3.00 V to 4.35 V.
- `charge`: `CHARGE_SENSE_PIN` reads HIGH while the board is powered through
  USB, matching the board-profile comment that HIGH means charging or USB
  present.
- `serial1`: Serial1 setup succeeds and TX/RX pins are valid, distinct GPIOs.
- `dac`: DAC setup and force-off run, and the DAC pin is one of ESP32's DAC
  output pins, GPIO25 or GPIO26.

If a board adaptation fails, the first failing Unity test item usually points
to the missing or mismatched peripheral.

The battery-present item is an electrical smoke test, not a battery gauge
calibration. A board with a floating VBAT divider can still produce misleading
ADC values; for reliable detection, the hardware should make the no-battery
state pull near ground or expose a dedicated battery-present signal.

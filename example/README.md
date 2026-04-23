# HinarUI PlatformIO Example

This example targets `esp-wrover-kit` and uses the root HinarUI library through
`symlink://..` in `platformio.ini`.

## Build

From the repository root:

```powershell
pio run -d example -e esp-wrover-kit
```

From this `example/` directory:

```powershell
pio run -e esp-wrover-kit
```

## Upload

From the repository root:

```powershell
pio run -d example -e esp-wrover-kit --target upload
```

The default pin and I2C mapping is defined in
`../include/boards/esp_wrover_kit.h`.

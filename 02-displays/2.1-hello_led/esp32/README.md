# 2.1 — Hello, LED! - ESP32

Blink an LED on GPIO26 using ESP-IDF v5.x.

## Build & Flash

```bash
idf.py build
idf.py -p <PORT> flash monitor
```

**Port by OS:** `/dev/ttyUSB0` (Linux) · `/dev/cu.usbserial-*` (macOS) · `COMx` (Windows)

## Key Concepts

### `gpio_config_t`
ESP-IDF's preferred way to configure a GPIO pin. Configures direction, pull-up/pull-down, and interrupt mode in a single struct, then applies it with `gpio_config()`. More explicit and scalable than calling `gpio_set_direction()` alone.

### `vTaskDelay()`
FreeRTOS delay. Takes ticks, not milliseconds — use `pdMS_TO_TICKS(ms)` to convert. Yields the task to the scheduler during the delay rather than busy-waiting like Arduino's `delay()`.
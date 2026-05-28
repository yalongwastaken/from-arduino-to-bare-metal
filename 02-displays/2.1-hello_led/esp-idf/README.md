# 2.1 — Hello, LED! — ESP-IDF

Blink an LED on GPIO26 using ESP-IDF v5.x.

## Build & Flash

```bash
cd esp-idf
idf.py build
idf.py -p /dev/ttyUSB0 flash monitor
```

Replace `/dev/ttyUSB0` with your port (`/dev/tty.usbserial-*` on macOS, `COMx` on Windows).

## Key Concepts

### `gpio_config_t`
ESP-IDF's preferred way to configure a GPIO pin. Configures direction, pull-up/pull-down, and interrupt mode in a single struct, then applies it with `gpio_config()`. More explicit and scalable than calling `gpio_set_direction()` alone.

### `vTaskDelay()`
FreeRTOS delay. Takes ticks, not milliseconds — use `pdMS_TO_TICKS(ms)` to convert. Yields the task to the scheduler during the delay rather than busy-waiting like Arduino's `delay()`.
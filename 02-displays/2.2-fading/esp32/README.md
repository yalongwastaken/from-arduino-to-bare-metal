# 2.2 — Fading — ESP32

Fade an LED in and out on GPIO26 using the LEDC peripheral.

## Build & Flash

```bash
idf.py build
idf.py -p <PORT> flash monitor
```

**Port by OS:** `/dev/ttyUSB0` (Linux) · `/dev/cu.usbserial-*` (macOS) · `COMx` (Windows)

## Key Concepts

### LEDC Peripheral
ESP32's dedicated PWM peripheral, primarily designed for LED brightness control. In ESP-IDF, configuring LEDC requires two steps — a timer config and a channel config — compared to Arduino's single `ledcAttach()` call. The timer sets the frequency and resolution; the channel binds a GPIO pin to that timer.

### PWM Resolution and Frequency
This lesson uses 5000Hz and 8-bit resolution (0–255 duty cycle range), matching the Arduino tutorial exactly. Higher resolution means finer brightness steps but limits the maximum frequency.

### `ledc_set_duty()` + `ledc_update_duty()`
In ESP-IDF, updating the duty cycle is a two-step operation — set the new value, then commit it. Arduino's `ledcWrite()` does both in one call.
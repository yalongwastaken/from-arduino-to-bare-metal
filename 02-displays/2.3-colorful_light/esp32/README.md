# 2.3 — Colorful Light — ESP-IDF

Control an RGB LED using three independent LEDC channels on GPIO27, GPIO26, and GPIO25.

## Build & Flash

```bash
idf.py build
idf.py -p /dev/tty.usbserial-* flash monitor
```

## Key Concepts

### Three LEDC Channels
Each color pin requires its own LEDC channel. All three channels share the same timer — same frequency and resolution — but each has an independently controllable duty cycle. This is what allows arbitrary color mixing.

### `set_color()`
A helper function that wraps the three `ledc_set_duty()` + `ledc_update_duty()` calls into a single call, matching the structure of Arduino's `setColor()`.

### Additive Color Mixing
Duty cycle maps directly to brightness (0–255). Combining channels at full duty produces white; partial values produce mixed colors. Red + Green = Yellow, Blue + Green = Cyan, Red + Blue = Purple.
# 2.4 — Microchip - 74HC595 — ESP32

Control 8 LEDs using a 74HC595 shift register, bit-banged over 3 GPIO pins.

## Build & Flash

```bash
idf.py build
idf.py -p <PORT> flash monitor
```

**Port by OS:** `/dev/ttyUSB0` (Linux) · `/dev/cu.usbserial-*` (macOS) · `COMx` (Windows)

## Key Concepts

### Bit-banging `shiftOut()`
Arduino's `shiftOut()` is a convenience wrapper — it loops 8 times, setting DS high or low for each bit, toggling SHcp each iteration. In ESP-IDF you write this loop explicitly with `gpio_set_level()` calls. No SPI peripheral is involved — this is pure GPIO timing.

### Latch sequence
The STcp pin controls when the shift register's internal state is transferred to the output pins. Pull it low before sending, pulse it high after — this prevents the outputs from flickering mid-transmission as bits are clocked in.

### Data array
The tutorial uses an array of 8-bit patterns to sequence the LEDs. Each value is sent as a full byte to the shift register, lighting the corresponding combination of Q0–Q7 outputs.
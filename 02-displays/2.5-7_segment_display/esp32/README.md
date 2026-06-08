# 2.5 — 7 Segment Display — ESP32

Drive a 7-segment display over a 74HC595 shift register, bit-banged over 3 GPIO pins.

## Build & Flash

    idf.py build
    idf.py -p <PORT> flash monitor

**Port by OS:** `/dev/ttyUSB0` (Linux) · `/dev/cu.usbserial-*` (macOS) · `COMx` (Windows)

## Key Concepts

### 7-segment encoding
Each digit is pre-computed as a byte where each bit maps to one segment
(0bPGFEDCBA, common cathode, active high). For example, `0x3f` (0b00111111)
lights segments A–F, forming the digit 0. The decimal point bit (P) is left
clear for all digits.

### `shiftOut()` → `shift_out()`
Arduino's `shiftOut()` is a convenience wrapper — it loops 8 times, setting DS
high or low for each bit and toggling SHcp each iteration. `shift_out()` here
is the direct equivalent: the same loop written explicitly with
`gpio_set_level()` calls. No SPI peripheral is involved — this is pure GPIO
bit-banging.

### Latch sequence
STcp is held low for the entire duration of `shift_out()`, preventing the
display from showing partial data as bits are clocked in. A single STcp high
pulse after all 8 bits are shifted transfers the completed byte to the output
register, updating all 8 segment lines simultaneously.
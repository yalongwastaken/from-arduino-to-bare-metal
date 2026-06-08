# 2.5 — 7 Segment Display

Drive a common-cathode 7-segment display using a 74HC595 shift register over 3 GPIO pins. Counts 0–9, advancing once per second.

SunFounder tutorial: [2.5 7 Segment Display](https://docs.sunfounder.com/projects/esp32-starter-kit/en/latest/arduino/2_display.html#segment-display)

## Circuit

- DS (pin 14)  → GPIO25 — serial data
- SHcp (pin 11) → GPIO26 — shift clock
- STcp (pin 12) → GPIO27 — storage/latch clock
- MR (pin 10)  → 3.3V  — active low reset; hold high to keep the register active
- OE (pin 13)  → GND   — active low output enable; hold low to enable outputs
- Q0–Q7 (pins 15, 1–7) → 220Ω resistors → 7-segment display segments → GND
- VCC (pin 16) → 3.3V
- GND (pin 8)  → GND

## How It Works

The 74HC595 drives the 7-segment display's 8 segment lines (A–G + decimal point)
from a single serial byte. Each bit in the byte maps to one segment
(0bPGFEDCBA, active high). A lookup table of 10 pre-computed bytes encodes
the digits 0–9. The ESP32 shifts each byte out MSB-first, then pulses the latch
to update all segments simultaneously.

## Contents

    2.5-7_segment_display/
    ├── esp32/
    │   ├── main/
    │   │   ├── main.c
    │   │   └── CMakeLists.txt
    │   ├── CMakeLists.txt
    │   └── README.md
    └── README.md
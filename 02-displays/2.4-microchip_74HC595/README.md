# 2.4 — Microchip - 74HC595

Control 8 LEDs using only 3 GPIO pins via a 74HC595 serial-in, parallel-out shift register.

SunFounder tutorial: [2.4 Microchip - 74HC595](https://docs.sunfounder.com/projects/esp32-starter-kit/en/latest/arduino/2_display.html#microchip-74hc595)

## Circuit

- DS (pin 14) → GPIO25 — serial data
- SHcp (pin 11) → GPIO26 — shift clock
- STcp (pin 12) → GPIO27 — storage/latch clock
- MR (pin 10) → 3.3V — active low reset; hold high to keep the register active
- OE (pin 13) → GND — active low output enable; hold low to enable outputs
- Q0–Q7 (pins 15, 1–7) → 220Ω resistors → LEDs → GND
- VCC (pin 16) → 3.3V
- GND (pin 8) → GND

## How It Works

The 74HC595 is a serial-in, parallel-out shift register. It receives data one bit at a time over DS, clocked in by SHcp, then latches all 8 bits to its output pins simultaneously when STcp is pulsed. This lets you control 8 outputs using only 3 GPIO pins.

## Contents

```
2.4-microchip_74hc595/
├── esp32/
│   ├── main/
│   │   ├── main.c
│   │   └── CMakeLists.txt
│   ├── CMakeLists.txt
│   └── README.md
└── README.md
```
# 2.2 — Fading

Fade an LED in and out on GPIO26 using PWM, creating a breathing effect.

SunFounder tutorial: [2.2 Fading](https://docs.sunfounder.com/projects/esp32-starter-kit/en/latest/arduino/2_display.html#fading)

## Circuit

Same circuit as 2.1 — no changes needed.

- LED anode → 220Ω resistor → GPIO26
- LED cathode → GND

## Contents

```
2.2-fading/
├── esp-idf/
│   ├── main/
│   │   ├── main.c
│   │   └── CMakeLists.txt
│   ├── CMakeLists.txt
│   └── README.md
└── README.md
```
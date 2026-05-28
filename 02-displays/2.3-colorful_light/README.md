# 2.3 — Colorful Light

Control an RGB LED on GPIO27 (red), GPIO26 (green), and GPIO25 (blue) using PWM on three independent LEDC channels.

SunFounder tutorial: [2.3 Colorful Light](https://docs.sunfounder.com/projects/esp32-starter-kit/en/latest/arduino/2_display.html#colorful-light)

## Circuit

- RGB LED common cathode → GND
- Red anode → 220Ω resistor → GPIO27
- Green anode → 220Ω resistor → GPIO26
- Blue anode → 220Ω resistor → GPIO25

## Contents

```
2.3-colorful_light/
├── esp-idf/
│   ├── main/
│   │   └── main.c
│   ├── CMakeLists.txt
│   └── README.md
└── README.md
```
# From Arduino to Bare Metal

Direct ports of the [SunFounder ESP32 Starter Kit](https://docs.sunfounder.com/projects/esp32-starter-kit/en/latest/) Arduino tutorials to ESP32, STM32, and Raspberry Pi 5.

No abstractions, no libraries — just the same circuits, rewritten against the hardware.

> **Status: in progress.**

## Why

The SunFounder tutorials are a great introduction to ESP32 with Arduino. This repo translates each lesson into:

- **ESP32 (via ESP-IDF v5.x+)** — Espressif's native framework. FreeRTOS, proper drivers, no Arduino.h.
- **STM32 (Nucleo-F446RE)** — ARM Cortex-M4, HAL/LL drivers, STM32CubeIDE.
- **Raspberry Pi 5 (Embedded Linux)** — libgpiod, userspace drivers, C on Linux.

If you're comfortable with the Arduino version of a lesson and want to see how it maps to the real framework, this is for you.

## Repository Structure

Lessons mirror the SunFounder tutorial structure exactly:

```
from-arduino-to-bare-metal/
├── 01-get-started/            — environment setup, one file per host OS
│   ├── esp32/                 — ESP-IDF v5.x
│   ├── stm32/                 — STM32CubeIDE
│   └── rpi5/                  — Raspberry Pi OS + libgpiod
└── NN-section-name/           — one directory per SunFounder section
    ├── N.M-lesson-name/       — one directory per lesson
    │   ├── esp32/             — ESP-IDF v5.x implementation
    │   ├── stm32/             — STM32CubeIDE implementation
    │   ├── rpi5/              — libgpiod implementation
    │   └── README.md          — lesson overview, SunFounder link
    └── README.md              — section overview
```

Each platform is a separate pass — ESP32 first, then STM32, then RPi5 — and a lesson directory gains a platform folder only once that pass reaches it. The full lesson map lives in this repo's [milestones](../../milestones).

## Requirements

Environment setup and installation for all three platforms — covering **macOS** (Apple Silicon), **Windows 11**, and **Ubuntu 26.04 Linux** hosts — lives in [`01-get-started/`](01-get-started/):

- [**ESP32** — ESP-IDF v5.x](01-get-started/esp32/)
- [**STM32** — STM32CubeIDE (Nucleo-F446RE)](01-get-started/stm32/)
- [**RPi5** — Raspberry Pi OS + libgpiod](01-get-started/rpi5/)

You'll also need the matching hardware: an ESP32 board (SunFounder ESP32 Starter Kit or equivalent), a Nucleo-F446RE, and/or a Raspberry Pi 5.

## Author

**Anthony Yalong**
- Email: yalong.anthony123@gmail.com
- GitHub: [@yalongwastaken](https://github.com/yalongwastaken)

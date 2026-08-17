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
├── 01-get_started/
│   ├── esp32/             — ESP-IDF v5.x setup (macOS / Windows / Linux)
│   ├── stm32/             — STM32CubeIDE setup (macOS / Windows / Linux)
│   └── rpi5/              — Raspberry Pi OS + libgpiod setup (+ SSH from any host)
├── 02-displays/
│   ├── 2.1-hello_led/
│   │   ├── esp32/         — ESP-IDF v5.x implementation
│   │   ├── stm32/         — STM32CubeIDE implementation
│   │   ├── rpi5/          — libgpiod implementation
│   │   └── README.md      — lesson overview, SunFounder link
│   └── README.md          — section overview
├── 03-sounds/
├── 04-actuators/
├── 05-sensors/
├── 06-funny-projects/
├── 07-bluetooth-sd-camera/
└── 08-iot/
```

ESP32 translations come first. STM32 and RPi5 folders are added to each lesson after all ESP32 lessons are complete.

## Requirements

Environment setup and installation for all three platforms — covering **macOS** (Apple Silicon), **Windows 11**, and **Ubuntu 26.04 Linux** hosts — lives in [`01-get_started/`](01-get_started/):

- [**ESP32** — ESP-IDF v5.x](01-get_started/esp32/)
- [**STM32** — STM32CubeIDE (Nucleo-F446RE)](01-get_started/stm32/)
- [**RPi5** — Raspberry Pi OS + libgpiod](01-get_started/rpi5/)

You'll also need the matching hardware: an ESP32 board (SunFounder ESP32 Starter Kit or equivalent), a Nucleo-F446RE, and/or a Raspberry Pi 5.

## Author

**Anthony Yalong**
- Email: yalong.anthony123@gmail.com
- GitHub: [@yalongwastaken](https://github.com/yalongwastaken)

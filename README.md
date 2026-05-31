# from-arduino-to-bare-metal

Direct ports of the [SunFounder ESP32 Starter Kit](https://docs.sunfounder.com/projects/esp32-starter-kit/en/latest/) Arduino tutorials to ESP32, STM32, and Raspberry Pi 5.

No abstractions, no libraries — just the same circuits, rewritten against the hardware.

## Why

The SunFounder tutorials are a great introduction to ESP32 with Arduino. This repo translates each lesson into:

- **ESP32 (via ESP-IDF v5.x+)** — Espressif's native framework. FreeRTOS, proper drivers, no Arduino.h.
- **STM32 (Nucleo-F446RE)** — ARM Cortex-M4, HAL/LL drivers, STM32CubeIDE.
- **Raspberry Pi 5 (Embedded Linux)** — libgpiod, userspace drivers, C on Linux.

If you're comfortable with the Arduino version of a lesson and want to see how it maps to the real framework, this is for you.

## Structure

Lessons mirror the SunFounder tutorial structure exactly:

```
from-arduino-to-bare-metal/
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

## Prerequisites

### ESP32
- ESP-IDF v5.x — [installation guide](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/get-started/)
- ESP32 board (SunFounder ESP32 Starter Kit or equivalent)

### STM32 *(coming later)*
- STM32CubeIDE — [download](https://www.st.com/en/development-tools/stm32cubeide.html)
- Nucleo-F446RE

### RPi5 *(coming later)*
- Raspberry Pi 5 running Raspberry Pi OS
- libgpiod — [docs](https://libgpiod.readthedocs.io/en/master/)

## Status

Active. ESP32 translations in progress, working through the SunFounder lesson list in order.
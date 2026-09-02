# 01 — Get Started

Environment setup for every target platform in this repo, across all three host operating systems. Pick the **platform you're programming** (the row) and the **OS of your development machine** (the column), then follow that one guide.

| Platform | macOS (Apple Silicon) | Linux (Ubuntu 26.04) | Windows 11 |
|---|---|---|---|
| **ESP32** — ESP-IDF v5.5+ | [Setup](esp32/macos.md) | [Setup](esp32/linux.md) | [Setup](esp32/windows.md) |
| **STM32** — STM32CubeIDE | [Setup](stm32/macos.md) | [Setup](stm32/linux.md) | [Setup](stm32/windows.md) |
| **RPi5** — libgpiod | [Setup](rpi5/macos.md) | [Setup](rpi5/linux.md) | [Setup](rpi5/windows.md) |

## Host vs. target

The ESP32 and STM32 guides install a toolchain on your development machine and flash the board over USB — the OS column is your machine's OS.

The **RPi5 is itself a Linux computer**. Its guide sets up Raspberry Pi OS + libgpiod *on the board*; the OS column only determines how you reach it from your machine (SSH + VS Code Remote SSH). The board steps are identical regardless of host.

## Hardware

- **ESP32** — any ESP32 board (SunFounder ESP32 Starter Kit or equivalent)
- **STM32** — Nucleo-F446RE
- **RPi5** — Raspberry Pi 5 + microSD card (16 GB+)

## Once you're set up

Head to [`02-displays/`](../02-displays/) and start with lesson **2.1 — Hello LED**.

---

Back to the [repo README](../README.md).
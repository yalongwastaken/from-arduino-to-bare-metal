# ESP32 Setup — Windows 11 (ESP-IDF)

This guide gets you from zero to a working ESP-IDF environment on Windows 11. Complete this before attempting any ESP32 lesson in this repo.

---

## 1. Install ESP-IDF v5.5+

ESP-IDF is Espressif's native framework for ESP32. It includes the toolchain, build system, FreeRTOS, and all peripheral drivers. It replaces the Arduino core entirely.

On Windows the official GUI installer is the recommended path — it bundles Python, Git, CMake, Ninja, and the toolchain, and wires up the environment for you.

1. Download the **ESP-IDF Windows Installer** from Espressif: https://dl.espressif.com/dl/esp-idf/ (online installer recommended)
2. Run it and:
   - Select **ESP32** as the target chip
   - Select the latest **v5.5+** release
   - Keep the default install path (`C:\Espressif\`)
3. Let it finish downloading the toolchain.

> **Keep the install path short.** Windows has a legacy 260-character path limit that ESP-IDF builds can trip over. Installing under `C:\Espressif\` (the default) keeps build paths well clear of it — don't relocate it deep inside your user profile.

### Using the IDF shell

The installer creates two Start Menu shortcuts that open a terminal with the ESP-IDF environment already loaded:

- **ESP-IDF Command Prompt** (CMD)
- **ESP-IDF PowerShell**

Use one of these for all build/flash work. `idf.py` is available immediately — there's no environment to source manually (the Windows equivalent of `get_idf`).

### Verify

Open the ESP-IDF Command Prompt and run:

```cmd
idf.py --version
:: ESP-IDF v5.5.x
```

---

## 2. VS Code Setup

Espressif provides an official ESP-IDF extension for VS Code that wraps `idf.py` into a GUI and manages the environment automatically.

### Install

1. Open VS Code
2. Go to Extensions (`Ctrl+Shift+X`)
3. Search **ESP-IDF** and install the extension by Espressif Systems
4. Open the command palette (`Ctrl+Shift+P`) → **ESP-IDF: Configure ESP-IDF Extension**
5. Choose **Use existing installation** and point it at your `C:\Espressif\frameworks\esp-idf-v5.5` directory

### What it gives you

- Build, flash, and monitor buttons in the status bar
- IntelliSense configured automatically for ESP-IDF headers
- Integrated serial monitor
- Component manager access

---

## 3. Project Structure

Each lesson in this repo follows the standard ESP-IDF project layout:

```
2.1-hello_led/
└── esp32/
    ├── CMakeLists.txt          — top-level build file, registers the project
    ├── main/
    │   ├── CMakeLists.txt      — registers main as a component, lists source files
    │   └── main.c              — application entry point (app_main)
    └── components/             — custom or third-party components (when needed)
        └── my_component/
            ├── CMakeLists.txt
            ├── include/
            │   └── my_component.h
            └── my_component.c
```

### How the files relate

**Top-level `CMakeLists.txt`**
Declares the project name and pulls in the ESP-IDF build system. Every project has exactly one.

```cmake
cmake_minimum_required(VERSION 3.16)
include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(my_project)
```

**`main/CMakeLists.txt`**
Registers `main` as an ESP-IDF component and lists its source files. Add new `.c` files here as you add them to `main/`.

```cmake
idf_component_register(SRCS "main.c"
                        INCLUDE_DIRS ".")
```

**`main/main.c`**
Your application code. The entry point is `app_main()` — called by ESP-IDF after system initialization. Think of everything before the main loop as Arduino's `setup()`, and the `while(1)` loop as Arduino's `loop()`.

**`components/`**
Optional. Each subdirectory is an independent component with its own `CMakeLists.txt`. ESP-IDF discovers them automatically. Used for reusable code like sensor drivers or protocol handlers — not needed for simple lessons.

---

## 4. Build, Flash, and Monitor

From inside any lesson's `esp32/` directory, in the **ESP-IDF Command Prompt**:

```cmd
:: build
idf.py build

:: flash
idf.py -p COM3 flash

:: monitor serial output
idf.py -p COM3 monitor

:: all at once
idf.py -p COM3 flash monitor
```

**Windows serial port:** `COM3`, `COM4`, etc. Check **Device Manager → Ports (COM & LPT)** to find which COM number your board enumerated as.

Exit the monitor with `Ctrl+]`.

---

## 5. Common Pitfalls

**`idf.py` not found**
You're in a regular terminal. Use the **ESP-IDF Command Prompt** or **ESP-IDF PowerShell** shortcut, which pre-loads the environment.

**Board not detected / no COM port appears**
Install the USB-to-UART driver for your board's bridge chip, then re-check Device Manager:
- **CP2102 / CP2104** (common on SunFounder boards): [CP210x drivers](https://www.silabs.com/developers/usb-to-uart-bridge-vcp-drivers)
- **CH340 / CH341**: [CH340 drivers](https://www.wch-ic.com/downloads/CH341SER_ZIP.html)

**Build fails with path-length / file-not-found errors**
A 260-char path limit issue. Keep the project and IDF install paths short (see the install note).

**`sdkconfig` conflicts after switching branches**
Delete `sdkconfig` and `build\` and rebuild from scratch.

**IntelliSense not resolving ESP-IDF headers in VS Code**
Run **ESP-IDF: Configure ESP-IDF Extension** again and confirm the IDF path is correct. The extension regenerates `c_cpp_properties.json` automatically.

**Flash fails with "A fatal error occurred: Failed to connect"**
Hold the `BOOT` button on the ESP32 while flashing, release after the upload starts.
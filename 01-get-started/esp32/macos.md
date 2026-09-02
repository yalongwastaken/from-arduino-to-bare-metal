# ESP32 Setup — macOS (ESP-IDF)

This guide gets you from zero to a working ESP-IDF environment on Apple Silicon. Complete this before attempting any ESP32 lesson in this repo.

---

## 1. Install ESP-IDF v5.5+

ESP-IDF is Espressif's native framework for ESP32. It includes the toolchain, build system, FreeRTOS, and all peripheral drivers. It replaces the Arduino core entirely, and runs **natively on Apple Silicon** — no Rosetta needed.

```bash
# install prerequisites
brew install cmake ninja dfu-util python

# clone ESP-IDF
mkdir -p ~/esp
cd ~/esp
git clone --recursive https://github.com/espressif/esp-idf.git
cd esp-idf
git checkout v5.5   # v5.5+ required for the Python 3.13/3.14 that Homebrew installs

# run the install script
./install.sh esp32
```

If Gatekeeper blocks a downloaded toolchain binary on first run ("cannot verify developer"), clear the quarantine flag on the tools directory:

```bash
xattr -dr com.apple.quarantine ~/.espressif
```

### Make the environment persistent

`export.sh` only configures the current shell session. Add an alias to `~/.zshrc` so you can load it on demand instead of retyping the path:

```bash
echo 'alias esp-idf=". $HOME/esp/esp-idf/export.sh"' >> ~/.zshrc
source ~/.zshrc
esp-idf
```

### Verify

```bash
idf.py --version
# ESP-IDF v5.5.x
```

---

## 2. VS Code Setup

Espressif provides an official ESP-IDF extension for VS Code that wraps `idf.py` into a GUI and manages the environment automatically.

### Install

1. Open VS Code
2. Go to Extensions (`Cmd+Shift+X`)
3. Search **ESP-IDF** and install the extension by Espressif Systems
4. Open the command palette (`Cmd+Shift+P`) → **ESP-IDF: Configure ESP-IDF Extension**
5. Choose **Use existing installation** and point it at your `~/esp/esp-idf` directory

### What it gives you

- Build, flash, and monitor buttons in the status bar
- IntelliSense configured automatically for ESP-IDF headers
- Integrated serial monitor
- Component manager access

---

## 3. Project Structure

Each lesson in this repo follows the standard ESP-IDF project layout:

```
2.1-hello-led/
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

From inside any lesson's `esp32/` directory (run `get_idf` first if the environment isn't loaded in this shell):

```bash
# build
idf.py build

# flash
idf.py -p /dev/cu.usbserial-* flash

# monitor serial output
idf.py -p /dev/cu.usbserial-* monitor

# all at once
idf.py -p /dev/cu.usbserial-* flash monitor
```

**macOS serial port:** `/dev/cu.usbserial-*`, or `/dev/cu.SLAB_USBtoUART` (CP210x) / `/dev/cu.wchusbserial*` (CH340). List what's connected with `ls /dev/cu.*`.

> Use `cu.`, **not** `tty.`. The `tty.*` device blocks on open waiting for carrier-detect and can hang the flasher; `cu.*` (call-up) is the correct outbound serial device.

Recent macOS (12+) ships CP210x and CH34x drivers built in. If the port never appears on older macOS, install the [CP210x](https://www.silabs.com/developers/usb-to-uart-bridge-vcp-drivers) or [CH34x](https://www.wch-ic.com/downloads/CH34XSER_MAC_ZIP.html) driver.

Exit the monitor with `Ctrl+]`.

---

## 5. Common Pitfalls

**`idf.py` not found**
The ESP-IDF environment wasn't sourced. Run `get_idf` (or `. ~/esp/esp-idf/export.sh`) in this shell.

**Toolchain binary blocked by Gatekeeper**
Clear the quarantine attribute: `xattr -dr com.apple.quarantine ~/.espressif`

**Serial port hangs / flasher won't connect**
Make sure you're using the `/dev/cu.*` device, not `/dev/tty.*` (see above).

**`sdkconfig` conflicts after switching branches**
Delete `sdkconfig` and `build/` and rebuild from scratch.

**IntelliSense not resolving ESP-IDF headers in VS Code**
Run **ESP-IDF: Configure ESP-IDF Extension** again and confirm the IDF path is correct. The extension regenerates `c_cpp_properties.json` automatically.

**Flash fails with "A fatal error occurred: Failed to connect"**
Hold the `BOOT` button on the ESP32 while flashing, release after the upload starts.
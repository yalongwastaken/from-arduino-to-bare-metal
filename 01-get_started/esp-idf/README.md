# ESP-IDF Setup

This guide gets you from zero to a working ESP-IDF environment on your machine. Complete this before attempting any lesson in this repo.

---

## 1. Install ESP-IDF v5.x

ESP-IDF is Espressif's native framework for ESP32. It includes the toolchain, build system, FreeRTOS, and all peripheral drivers. It replaces the Arduino core entirely.

### macOS / Linux

```bash
# install prerequisites (macOS)
brew install cmake ninja dfu-util python3

# clone ESP-IDF
mkdir -p ~/esp
cd ~/esp
git clone --recursive https://github.com/espressif/esp-idf.git
cd esp-idf
git checkout v5.3  # or latest stable v5.x tag

# run the install script
./install.sh esp32

# add to your shell config (~/.zshrc or ~/.bashrc)
. ~/esp/esp-idf/export.sh
```

### Windows

Use the [ESP-IDF Windows Installer](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/get-started/windows-setup.html) — it handles Python, Git, CMake, and the toolchain automatically.

### Verify

```bash
idf.py --version
# ESP-IDF v5.x.x
```

---

## 2. VS Code Setup

Espressif provides an official ESP-IDF extension for VS Code that wraps `idf.py` into a GUI and manages the environment automatically.

### Install

1. Open VS Code
2. Go to Extensions (`Cmd+Shift+X` / `Ctrl+Shift+X`)
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
2.1-hello_led/
└── esp-idf/
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

From inside any lesson's `esp-idf/` directory:

```bash
# build
idf.py build

# flash
idf.py -p /dev/tty.usbserial-* flash

# monitor serial output
idf.py -p /dev/tty.usbserial-* monitor

# all at once
idf.py -p /dev/tty.usbserial-* flash monitor
```

**Port names by OS:**
- macOS: `/dev/tty.usbserial-*` or `/dev/tty.SLAB_USBtoUART`
- Linux: `/dev/ttyUSB0`
- Windows: `COM3`, `COM4`, etc.

Exit the monitor with `Ctrl+]`.

---

## 5. Common Pitfalls

**`idf.py` not found**
The ESP-IDF environment wasn't sourced. Run `. ~/esp/esp-idf/export.sh` or add it to your shell config.

**Permission denied on port (Linux)**
```bash
sudo usermod -a -G dialout $USER
# log out and back in
```

**`sdkconfig` conflicts after switching branches**
Delete `sdkconfig` and `build/` and rebuild from scratch.

**IntelliSense not resolving ESP-IDF headers in VS Code**
Run **ESP-IDF: Configure ESP-IDF Extension** again and make sure the IDF path is correct. The extension generates `c_cpp_properties.json` automatically.

**Flash fails with "A fatal error occurred: Failed to connect"**
Hold the `BOOT` button on the ESP32 while flashing, release after the upload starts.
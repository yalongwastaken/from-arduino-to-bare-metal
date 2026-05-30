# RPi5 Setup (libgpiod)

This guide gets you from zero to a working embedded Linux development environment on Raspberry Pi 5. Complete this before attempting any RPi5 lesson in this repo.

---

## 1. Install Raspberry Pi OS

Flash Raspberry Pi OS (64-bit, Lite or Desktop) to a microSD card using [Raspberry Pi Imager](https://www.raspberrypi.com/software/). Enable SSH and set your hostname/credentials in the imager before flashing.

Verify once booted:

```bash
uname -a
# Linux raspberrypi 6.x.x ...
```

---

## 2. Install libgpiod

libgpiod is the modern, kernel-supported GPIO library for Linux. It replaces the deprecated sysfs interface and is what production embedded Linux projects use.

```bash
sudo apt update
sudo apt install -y libgpiod-dev gpiod
```

Verify:

```bash
gpiodetect
# lists all GPIO chips on the system
```

---

## 3. VS Code Setup (Remote SSH)

The recommended workflow is to write code on your development machine and build/run on the Pi over SSH.

### Install

1. Open VS Code
2. Go to Extensions (`Cmd+Shift+X` / `Ctrl+Shift+X`)
3. Search **Remote - SSH** and install the extension by Microsoft
4. Open the command palette (`Cmd+Shift+P`) → **Remote-SSH: Connect to Host**
5. Enter `user@raspberrypi.local`

You now have a full VS Code environment running on the Pi.

### What it gives you

- Edit files directly on the Pi from your development machine
- Integrated terminal running on the Pi
- IntelliSense for C with libgpiod headers

---

## 4. Project Structure

Each RPi5 lesson in this repo follows a simple flat structure:

```
2.1-hello_led/
└── rpi5/
    ├── Makefile
    ├── main.c
    └── README.md
```

No build system complexity — just a `Makefile` that compiles with `gcc` and links against `libgpiod`.

### Makefile template

```makefile
CC     = gcc
CFLAGS = -Wall -Wextra
LIBS   = -lgpiod

TARGET = main
SRC    = main.c

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC) $(LIBS)

clean:
	rm -f $(TARGET)
```

---

## 5. Build and Run

From inside any lesson's `rpi5/` directory on the Pi:

```bash
# build
make

# run (GPIO access requires root or gpio group membership)
sudo ./main

# clean
make clean
```

### GPIO group (avoid sudo)

```bash
sudo usermod -aG gpio $USER
# log out and back in
```

---

## 6. Common Pitfalls

**`libgpiod/gpiod.h` not found**
Run `sudo apt install libgpiod-dev` — the `-dev` package provides the headers.

**Permission denied on `/dev/gpiochip*`**
Either run with `sudo` or add your user to the `gpio` group (see above).

**`gpiodetect` shows no chips**
The Pi isn't running a recent enough kernel. Update with `sudo apt full-upgrade` and reboot.

**Finding the right GPIO chip and line**
Use `gpioinfo` to list all chips and their lines. On RPi5, GPIO pins are typically on `gpiochip4`.

```bash
gpioinfo
```
# RPi5 Setup — Windows 11 host (libgpiod, cross-compile via WSL2)

This guide gets you from zero to a working cross-compilation workflow for Raspberry Pi 5, building inside WSL2 on Windows 11 and deploying to the Pi over SSH. Complete this before attempting any RPi5 lesson in this repo.

> **Workflow:** edit on Windows → build inside WSL2 → `scp` binary to Pi → run on Pi. The Pi never needs a compiler installed.

> **Why WSL2?** Native Windows cross-compilation toolchains for aarch64 Linux exist but are fragile and non-standard. WSL2 gives you a full Ubuntu environment on Windows — you get the same clean `apt install` toolchain path used in the Linux guide, and it's how Windows-based embedded developers actually work in industry.

---

## 1. Flash Raspberry Pi OS (on the Pi)

Flash Raspberry Pi OS (64-bit, Lite or Desktop) to a microSD card using [Raspberry Pi Imager](https://www.raspberrypi.com/software/). Enable SSH and set your hostname/credentials in the imager's **OS Customisation** (gear icon) before flashing.

Verify once booted:

```bash
uname -a
# Linux raspberrypi 6.x.x ... aarch64
```

---

## 2. Install libgpiod on the Pi

SSH into the Pi (from Windows Terminal: `ssh pi@raspberrypi.local`) and install libgpiod:

```bash
sudo apt update
sudo apt install -y libgpiod-dev gpiod
```

> Bookworm ships **libgpiod v2**, whose API differs substantially from the older v1 (builder-pattern line requests instead of `gpiod_line_request_*`). Lessons in this repo target the **v2 API** — if you pattern-match against an old v1 tutorial, the calls won't line up.

Verify:

```bash
gpiodetect --version   # gpiodetect v2.x
gpiodetect             # gpiochip4 is the RP1 chip driving the 40-pin header
```

---

## 3. Set up WSL2

If you don't already have WSL2:

```powershell
# in Windows Terminal (PowerShell or CMD), run as administrator
wsl --install
```

This installs WSL2 with Ubuntu as the default distro. Reboot when prompted. After rebooting, Ubuntu launches and prompts you to set a username and password.

Verify:

```powershell
wsl --list --verbose
# Ubuntu   Running   2
```

All remaining steps in this guide run **inside the WSL2 Ubuntu terminal**, not in PowerShell or CMD.

---

## 4. Install the cross-compiler (inside WSL2)

```bash
sudo apt update
sudo apt install gcc-aarch64-linux-gnu
```

Verify:

```bash
aarch64-linux-gnu-gcc --version
# aarch64-linux-gnu-gcc x.x.x ...
```

---

## 5. Set up the sysroot (one-time, inside WSL2)

Copy the Pi's `gpiod.h` header and `libgpiod.a` static library into a sysroot on your WSL2 filesystem. SSH and scp work natively from inside WSL2:

```bash
mkdir -p ~/rpi5-sysroot/include ~/rpi5-sysroot/lib

scp pi@raspberrypi.local:/usr/include/gpiod.h ~/rpi5-sysroot/include/
scp pi@raspberrypi.local:/usr/lib/aarch64-linux-gnu/libgpiod.a ~/rpi5-sysroot/lib/
```

This is a one-time setup — all lessons share the same sysroot.

---

## 6. VS Code Setup

VS Code on Windows has native WSL2 integration — you edit files inside WSL2 and build in the WSL2 terminal, all without leaving VS Code.

1. Install VS Code on Windows: https://code.visualstudio.com/
2. Install the **WSL** extension by Microsoft
3. Open the command palette (`Ctrl+Shift+P`) → **WSL: Open Folder in WSL** → navigate to the repo inside your WSL2 filesystem (e.g. `~/from-arduino-to-bare-metal`)
4. Install the **C/C++** extension by Microsoft for IntelliSense
5. Point IntelliSense at the sysroot headers: in `.vscode/c_cpp_properties.json`, add `~/rpi5-sysroot/include` to `includePath`

The integrated terminal in VS Code now runs inside WSL2 — `make`, `scp`, and `ssh` all work directly.

---

## 7. Project Structure

Each RPi5 lesson in this repo follows a simple flat structure:

```
2.1-hello_led/
└── rpi5/
    ├── Makefile
    ├── main.c
    └── README.md
```

### Makefile template

```makefile
CC      = aarch64-linux-gnu-gcc
SYSROOT = $(HOME)/rpi5-sysroot
CFLAGS  = -Wall -Wextra -I$(SYSROOT)/include
LDFLAGS = -L$(SYSROOT)/lib
LIBS    = -lgpiod

TARGET  = main
SRC     = main.c

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $(TARGET) $(SRC) $(LIBS)

deploy: $(TARGET)
	scp $(TARGET) pi@raspberrypi.local:~/

clean:
	rm -f $(TARGET)
```

---

## 8. Build, Deploy, and Run

From inside any lesson's `rpi5/` directory in your **WSL2 terminal**:

```bash
# build for aarch64
make

# copy to Pi
make deploy

# run on Pi
ssh pi@raspberrypi.local './main'
```

Or all at once:

```bash
make && make deploy && ssh pi@raspberrypi.local './main'
```

### GPIO permissions (on the Pi)

```bash
sudo usermod -aG gpio $USER
# log out and back in — then ./main runs without sudo
```

---

## 8. Common Pitfalls

**`gpiod.h` not found during build**
The sysroot isn't set up or the path is wrong. Confirm `~/rpi5-sysroot/include/gpiod.h` exists inside WSL2.

**`cannot find -lgpiod`**
`libgpiod.a` is missing from the sysroot. Re-run the `scp` from section 5.

**`Exec format error` when running on Pi**
The binary was compiled for the wrong architecture. Confirm `aarch64-linux-gnu-gcc --version` shows `aarch64` and that the Makefile CC is set correctly.

**Permission denied on `/dev/gpiochip*`**
Add your user to the `gpio` group on the Pi (see section 8).

**`raspberrypi.local` doesn't resolve from WSL2**
Try the Pi's IP address instead, or run `ssh pi@raspberrypi.local` from Windows Terminal first to confirm connectivity outside WSL2.

**Finding the right GPIO chip and line**
On RPi5 the 40-pin header is on `gpiochip4` (the RP1 chip):

```bash
gpioinfo gpiochip4
```
# RPi5 Setup — Windows 11 host (libgpiod)

This guide gets you from zero to a working embedded Linux development environment on Raspberry Pi 5, working from a Windows 11 host. Complete this before attempting any RPi5 lesson in this repo.

> The **RPi5 board** runs **Raspberry Pi OS** (a Linux shell), so the `apt`/`gcc`/`gpiod` commands below all run *on the Pi*, not in Windows. Your **Windows 11 machine** only needs SSH access — built into Windows 11.

---

## 1. Install Raspberry Pi OS

Flash Raspberry Pi OS (64-bit, Lite or Desktop) to a microSD card using [Raspberry Pi Imager](https://www.raspberrypi.com/software/). Enable SSH and set your hostname/credentials in the imager's **OS Customisation** (gear icon) before flashing.

Verify once booted (over SSH or on the Pi directly):

```bash
uname -a
# Linux raspberrypi 6.x.x ... aarch64
```

---

## 2. Install libgpiod

libgpiod is the modern, kernel-supported GPIO library for Linux. It replaces the deprecated sysfs interface and is what production embedded Linux projects use. Run these **on the Pi** (via SSH — see section 3):

```bash
sudo apt update
sudo apt install -y libgpiod-dev gpiod
```

> Bookworm ships **libgpiod v2**, whose API differs substantially from the older v1 (builder-pattern line requests instead of `gpiod_line_request_*`). Lessons in this repo target the **v2 API** — if you pattern-match against an old v1 tutorial, the calls won't line up.

Verify:

```bash
gpiodetect
# lists all GPIO chips; gpiochip4 is the RP1 chip driving the 40-pin header
gpiodetect --version
# gpiodetect v2.x
```

---

## 3. SSH and VS Code Setup (Remote SSH)

The recommended workflow is to edit on Windows and build/run on the Pi over SSH.

### Connect over SSH

Windows 11 ships with OpenSSH. Open **Windows Terminal** (PowerShell or CMD):

```powershell
ssh user@raspberrypi.local
```

To avoid retyping the address, add an entry to `C:\Users\<you>\.ssh\config`:

```
Host rpi5
    HostName raspberrypi.local
    User <username>
    IdentityFile C:\Users\<you>\.ssh\id_ed25519   # if using key auth
```

Then just `ssh rpi5`. (PuTTY also works if you prefer a GUI client: https://www.putty.org/)

### VS Code Remote SSH

1. Open VS Code
2. Go to Extensions (`Ctrl+Shift+X`)
3. Search **Remote - SSH** and install the extension by Microsoft
4. Open the command palette (`Ctrl+Shift+P`) → **Remote-SSH: Connect to Host**
5. Enter `user@raspberrypi.local` (or the `rpi5` alias from your SSH config)

You now have a full VS Code environment running on the Pi.

### What it gives you

- Edit files directly on the Pi from Windows
- Integrated terminal running on the Pi
- IntelliSense for C with libgpiod headers

---

## 4. Project Structure

Each RPi5 lesson in this repo follows a simple flat structure:

```
2.1-hello-led/
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

From inside any lesson's `rpi5/` directory **on the Pi** (via SSH or the VS Code remote terminal — not a Windows shell):

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

**`raspberrypi.local` won't resolve**
mDNS resolution works on Windows 11, but if it fails, connect by the Pi's IP address instead (find it in your router's client list or run `ip addr show` on the Pi).

**`gpiod.h` not found**
Run `sudo apt install libgpiod-dev` on the Pi — the `-dev` package provides the header (`#include <gpiod.h>`, installed to `/usr/include/gpiod.h`).

**Permission denied on `/dev/gpiochip*`**
Either run with `sudo` or add your user to the `gpio` group (see above).

**`gpiodetect` shows no chips**
The Pi isn't running a recent enough kernel. Update with `sudo apt full-upgrade` and reboot.

**Finding the right GPIO chip and line**
Use `gpioinfo` to list all chips and their lines. On RPi5, the 40-pin header is on `gpiochip4` (the RP1 chip).

```bash
gpioinfo gpiochip4
```
# RPi5 Setup — Linux host (libgpiod, cross-compile)

This guide gets you from zero to a working cross-compilation workflow for Raspberry Pi 5, building on Ubuntu 26.04 and deploying to the Pi over SSH. Complete this before attempting any RPi5 lesson in this repo.

> **Workflow:** edit and build on your Linux host → `scp` binary to Pi → run on Pi. The Pi never needs a compiler installed.

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

SSH into the Pi and install libgpiod. This gives you the runtime library, development headers, and CLI tools:

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

## 3. Install the cross-compiler on your Linux host

On Ubuntu 26.04:

```bash
sudo apt install gcc-aarch64-linux-gnu
```

Verify:

```bash
aarch64-linux-gnu-gcc --version
# aarch64-linux-gnu-gcc 15.x.x ...
```

---

## 4. Set up the sysroot (one-time)

The cross-compiler needs the Pi's `gpiod.h` header and `libgpiod.a` static library to resolve symbols at build time. Copy them from the Pi to a sysroot directory on your host:

```bash
mkdir -p ~/rpi5-sysroot/include ~/rpi5-sysroot/lib

scp pi@raspberrypi.local:/usr/include/gpiod.h ~/rpi5-sysroot/include/
scp pi@raspberrypi.local:/usr/lib/aarch64-linux-gnu/libgpiod.a ~/rpi5-sysroot/lib/
```

This is a one-time setup — all lessons share the same sysroot.

---

## 5. VS Code Setup

Edit files locally in VS Code as you would any C project. Use the integrated terminal for building and deploying.

1. Open VS Code
2. Open the repo folder locally (**File → Open Folder**)
3. Install the **C/C++** extension by Microsoft for IntelliSense
4. Point IntelliSense at the sysroot headers: in `.vscode/c_cpp_properties.json`, add `~/rpi5-sysroot/include` to `includePath`

Use a second terminal pane (or a VS Code task) for `ssh pi@raspberrypi.local` when you need to run or monitor output on the Pi.

---

## 6. Project Structure

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

TARGET  = work/main
SRC     = main.c

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $(TARGET) $(SRC) $(LIBS)

deploy: $(TARGET)
	scp $(TARGET) pi@raspberrypi.local:~/

clean:
	rm -f $(TARGET)
```

---

## 7. Build, Deploy, and Run

From inside any lesson's `rpi5/` directory on your **Linux host**:

```bash
# build for aarch64
make

# copy to Pi
make deploy

# run on Pi (in a separate ssh session or one-liner)
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
The sysroot isn't set up or the path is wrong. Confirm `~/rpi5-sysroot/include/gpiod.h` exists.

**`cannot find -lgpiod`**
`libgpiod.a` is missing from the sysroot. Re-run the `scp` from section 4.

**`Exec format error` when running on Pi**
The binary was compiled for the wrong architecture. Confirm `aarch64-linux-gnu-gcc --version` shows `aarch64` and that the Makefile uses `CC = aarch64-linux-gnu-gcc`.

**Permission denied on `/dev/gpiochip*`**
Add your user to the `gpio` group on the Pi (see section 7).

**`raspberrypi.local` doesn't resolve**
Install `avahi-daemon` on the host (`sudo apt install avahi-daemon`) or connect by IP address.

**Finding the right GPIO chip and line**
On RPi5 the 40-pin header is on `gpiochip4` (the RP1 chip):

```bash
gpioinfo gpiochip4
```
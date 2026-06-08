# 2.1 — Hello, LED! - RPi5

Blink an LED on GPIO26 using libgpiod v2.

## Build & Deploy

```bash
make
make deploy
ssh pi@raspberrypi.local './main'
```

## Key Concepts

### `gpiod_line_settings`
libgpiod v2's way to configure a GPIO line. Sets direction, initial output value, bias, and edge detection in a single settings object before requesting the line from the kernel. More explicit than v1's `gpiod_line_request_output()` — you build the config, then commit it in one ioctl.

### `gpiod_chip_request_lines()`
The v2 equivalent of `pinMode()`. Issues a `GPIO_V2_GET_LINE_IOCTL` syscall that hands ownership of the pin to your process. The kernel enforces exclusive access — no other process can claim the line until you call `gpiod_line_request_release()` or your process exits.

### `sleep()`
POSIX delay in whole seconds. For sub-second timing use `usleep(microseconds)` or `nanosleep()`. Unlike Arduino's `delay()`, these yield to the Linux scheduler — your process isn't busy-waiting, it's just not scheduled until the sleep expires.
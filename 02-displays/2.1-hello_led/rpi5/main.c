/**
 * @file main.c
 * @brief Bare-metal port of the Arduino hello_led sketch using libgpiod v2.
 */

#include <gpiod.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#define CHIP_PATH    "/dev/gpiochip0"
#define GPIO_OFFSET  26               // the GPIO pin for the LED
#define BLINK_COUNT  10

int main(void) {
    /* ── 1. Open the GPIO chip ─────────────────────────────────────────────
     * gpiod_chip_open() opens /dev/gpiochip0 — the RP1 controller that
     * drives the 40-pin header. Equivalent to the board setup that happens
     * invisibly before Arduino's setup() runs.
     */
    struct gpiod_chip *chip = gpiod_chip_open(CHIP_PATH);
    if (!chip) {
        perror("gpiod_chip_open");
        return EXIT_FAILURE;
    }

    /* ── 2. Configure the line ─────────────────────────────────────────────
     * This is pinMode(ledPin, OUTPUT).
     * GPIOD_LINE_DIRECTION_OUTPUT — we are driving the pin, not reading it.
     * GPIOD_LINE_VALUE_INACTIVE   — start LOW (LED off) on request.
     */
    struct gpiod_line_settings *settings = gpiod_line_settings_new();
    if (!settings) {
        perror("gpiod_line_settings_new");
        gpiod_chip_close(chip);
        return EXIT_FAILURE;
    }
    gpiod_line_settings_set_direction(settings, GPIOD_LINE_DIRECTION_OUTPUT);
    gpiod_line_settings_set_output_value(settings, GPIOD_LINE_VALUE_INACTIVE);

    /* ── 3. Build a line config ────────────────────────────────────────────
     * Maps GPIO offset 18 to the settings above.
     * In Arduino this is implicit — pinMode() just takes a pin number.
     * Here we're explicit: offset → settings, and we could add more offsets
     * to this config for atomic multi-pin control.
     */
    struct gpiod_line_config *line_cfg = gpiod_line_config_new();
    if (!line_cfg) {
        perror("gpiod_line_config_new");
        gpiod_line_settings_free(settings);
        gpiod_chip_close(chip);
        return EXIT_FAILURE;
    }
    unsigned int offset = GPIO_OFFSET;
    if (gpiod_line_config_add_line_settings(line_cfg, &offset, 1, settings) < 0) {
        perror("gpiod_line_config_add_line_settings");
        goto cleanup;
    }

    /* ── 4. Request config: consumer name ──────────────────────────────────
     * Labels this process as the owner of the pin — visible in gpioinfo.
     * Arduino has no equivalent; the framework owns every pin and nothing
     * else can claim them. Here the kernel enforces ownership explicitly:
     * only one consumer can hold a line at a time.
     */
    struct gpiod_request_config *req_cfg = gpiod_request_config_new();
    if (!req_cfg) {
        perror("gpiod_request_config_new");
        goto cleanup;
    }
    gpiod_request_config_set_consumer(req_cfg, "hello_led");

    /* ── 5. Request the line ───────────────────────────────────────────────
     * This is the last part of setup() — claim the pin so we can drive it.
     * Under the hood: ioctl(chip_fd, GPIO_V2_GET_LINE_IOCTL, ...) tells the
     * kernel to assign GPIO 18 to us and return a line file descriptor.
     * From this point, no other process can claim the pin until we release.
     */
    struct gpiod_line_request *request = gpiod_chip_request_lines(chip, req_cfg, line_cfg);
    if (!request) {
        perror("gpiod_chip_request_lines");
        gpiod_request_config_free(req_cfg);
        goto cleanup;
    }

    /* ── 6. Blink ──────────────────────────────────────────────────────────
     * This is loop() — runs BLINK_COUNT times instead of forever.
     *
     * digitalWrite(ledPin, HIGH)  →  GPIOD_LINE_VALUE_ACTIVE   (turn LED on)
     * delay(1000)                 →  sleep(1)
     * digitalWrite(ledPin, LOW)   →  GPIOD_LINE_VALUE_INACTIVE (turn LED off)
     * delay(1000)                 →  sleep(1)
     *
     * set_value() calls ioctl(line_fd, GPIO_V2_LINE_SET_VALUES_IOCTL, ...)
     * which propagates: syscall → kernel GPIO driver → RP1 over PCIe →
     * physical pin voltage change. The whole chain takes microseconds.
     */
    printf("Blinking GPIO %d, %d times...\n", GPIO_OFFSET, BLINK_COUNT);
    for (int i = 0; i < BLINK_COUNT; i++) {
        gpiod_line_request_set_value(request, GPIO_OFFSET, GPIOD_LINE_VALUE_ACTIVE);   // HIGH — turn LED on
        sleep(1);
        gpiod_line_request_set_value(request, GPIO_OFFSET, GPIOD_LINE_VALUE_INACTIVE); // LOW  — turn LED off
        sleep(1);
    }

    /* ── 7. Cleanup ────────────────────────────────────────────────────────
     * Arduino has no equivalent — the board just loses power.
     * Here we explicitly release the line: close(line_fd) inside libgpiod
     * tells the kernel to relinquish GPIO 18. No stuck pins, no manual
     * cleanup step needed next time you run gpioinfo or claim this pin.
     */
    gpiod_line_request_release(request);
    gpiod_request_config_free(req_cfg);

cleanup:
    gpiod_line_config_free(line_cfg);
    gpiod_line_settings_free(settings);
    gpiod_chip_close(chip);

    return EXIT_SUCCESS;
}
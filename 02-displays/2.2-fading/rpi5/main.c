/**
 * @file main.c
 * @brief Fading — creates a breathing effect on an LED using PWM on GPIO26.
 *        RPi5 sysfs translation of SunFounder ESP32 Starter Kit lesson 2.2.
 */

#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>

#define PWM_CHIP    "pwmchip0c"            // PWM chip exposed by the RPi5 kernel driver
#define PWM_CHANNEL 0                     // PWM channel 0 maps to GPIO26 (pin 37)
#define PERIOD_NS   20000000UL            // 20ms period = 50 Hz PWM frequency

static int sysfs_write(const char *path, const char *value);
static int sysfs_write_ul(const char *path, unsigned long value);
static int pwm_export(int channel);
static int pwm_unexport(int channel);
static int pwm_set_period(int channel, unsigned long period_ns);
static int pwm_set_duty(int channel, unsigned long duty_ns);
static int pwm_set_duty_pct(int channel, int duty_pct, unsigned long period_ns);
static int pwm_set_enabled(int channel, int enable);

int main(void) {
    pwm_export(PWM_CHANNEL);               // expose pwm0 under /sys/class/pwm/pwmchip2/
    pwm_set_period(PWM_CHANNEL, PERIOD_NS); // set PWM period before duty or enable
    pwm_set_duty(PWM_CHANNEL, 0);          // initialize duty cycle to 0 (LED off)
    pwm_set_enabled(PWM_CHANNEL, 1);       // start PWM signal on the pin

    // fade up: 0% → 100% brightness
    for (int i = 0; i <= 100; i++) {
        pwm_set_duty_pct(PWM_CHANNEL, i, PERIOD_NS);
        usleep(20000); // wait 20ms per step
    }

    // fade down: 100% → 0% brightness
    for (int i = 100; i >= 0; i--) {
        pwm_set_duty_pct(PWM_CHANNEL, i, PERIOD_NS);
        usleep(20000); // wait 20ms per step
    }

    pwm_set_enabled(PWM_CHANNEL, 0);       // stop PWM signal
    pwm_unexport(PWM_CHANNEL);             // release the channel back to the kernel
    return 0;
}

// writes a string value to a sysfs file at the given path
static int sysfs_write(const char *path, const char *value) {
    int fd = open(path, O_WRONLY);
    if (fd < 0) {
        fprintf(stderr, "open(%s): %s\n", path, strerror(errno));
        return -1;
    }
    ssize_t len = (ssize_t)strlen(value);
    if (write(fd, value, (size_t)len) != len) {
        if (errno != EBUSY) {  // EBUSY is expected on already-exported channels
            fprintf(stderr, "write(%s, \"%s\"): %s\n", path, value, strerror(errno));
            close(fd);
            return -1;
        }
    }
    close(fd);
    return 0;
}

// converts an unsigned long to its ASCII decimal string and writes it to a sysfs file
static int sysfs_write_ul(const char *path, unsigned long value) {
    char buff[32];
    snprintf(buff, sizeof(buff), "%lu", value);
    return sysfs_write(path, buff);
}

// exports the channel, creating /sys/class/pwm/pwmchipN/pwmX/ and its control files
static int pwm_export(int channel) {
    char path[64];
    snprintf(path, sizeof(path), "/sys/class/pwm/%s/export", PWM_CHIP);
    return sysfs_write_ul(path, channel);
}

// unexports the channel, removing /sys/class/pwm/pwmchipN/pwmX/ from the filesystem
static int pwm_unexport(int channel) {
    char path[64];
    snprintf(path, sizeof(path), "/sys/class/pwm/%s/unexport", PWM_CHIP);
    return sysfs_write_ul(path, channel);
}

// sets the total PWM period in nanoseconds; must be set before duty_cycle or enable
static int pwm_set_period(int channel, unsigned long period_ns) {
    char path[64];
    snprintf(path, sizeof(path), "/sys/class/pwm/%s/pwm%d/period", PWM_CHIP, channel);
    return sysfs_write_ul(path, period_ns);
}

// sets the PWM duty cycle in nanoseconds; must be <= period_ns
static int pwm_set_duty(int channel, unsigned long duty_ns) {
    char path[64];
    snprintf(path, sizeof(path), "/sys/class/pwm/%s/pwm%d/duty_cycle", PWM_CHIP, channel);
    return sysfs_write_ul(path, duty_ns);
}

// sets duty cycle as a percentage of the period (0–100); integer division, sub-1% lost
static int pwm_set_duty_pct(int channel, int duty_pct, unsigned long period_ns) {
    unsigned long duty_ns = (period_ns / 100) * duty_pct;
    return pwm_set_duty(channel, duty_ns);
}

// enables (1) or disables (0) the PWM signal on the pin
static int pwm_set_enabled(int channel, int enable) {
    char path[64];
    snprintf(path, sizeof(path), "/sys/class/pwm/%s/pwm%d/enable", PWM_CHIP, channel);
    return sysfs_write(path, enable ? "1" : "0");
}
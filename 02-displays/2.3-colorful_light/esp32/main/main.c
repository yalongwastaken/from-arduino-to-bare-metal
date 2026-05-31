/**
 * @file main.c
 * @brief Colorful Light — cycles an RGB LED through red, green, blue, yellow,
 *        purple, and cyan using three independent LEDC channels on GPIO27/26/25.
 *        ESP-IDF translation of SunFounder ESP32 Starter Kit lesson 2.3.
 */

#include "esp_err.h"
#include "driver/ledc.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

// GPIO pins connected to each LED anode via 220ohm current limiting resistors
#define RED_PIN   GPIO_NUM_27
#define GREEN_PIN GPIO_NUM_26
#define BLUE_PIN  GPIO_NUM_25

// LEDC configuration — all three channels share a single timer
#define LEDC_FREQ_HZ    5000              // PWM frequency in Hz
#define LEDC_RESOLUTION LEDC_TIMER_8_BIT // 8-bit resolution: duty cycle range 0-255
#define LEDC_CLK_CFG    LEDC_AUTO_CLK
#define LEDC_TIMER      LEDC_TIMER_0
#define LEDC_SPEED_MODE LEDC_LOW_SPEED_MODE

// one LEDC channel per color pin
#define LEDC_CHANNEL_RED   2
#define LEDC_CHANNEL_GREEN 1
#define LEDC_CHANNEL_BLUE  0

// function prototypes
void setup_led(gpio_num_t pin, ledc_channel_t channel);
void set_color(uint32_t red, uint32_t green, uint32_t blue);

void app_main(void) {

    // configure LEDC timer: sets PWM frequency and resolution for all three channels
    ledc_timer_config_t timer_config = {
        .freq_hz         = LEDC_FREQ_HZ,
        .duty_resolution = LEDC_RESOLUTION,
        .clk_cfg         = LEDC_CLK_CFG,
        .timer_num       = LEDC_TIMER,
        .speed_mode      = LEDC_SPEED_MODE,
    };
    ESP_ERROR_CHECK(ledc_timer_config(&timer_config));

    // bind each color pin to its LEDC channel
    setup_led(RED_PIN,   LEDC_CHANNEL_RED);
    setup_led(GREEN_PIN, LEDC_CHANNEL_GREEN);
    setup_led(BLUE_PIN,  LEDC_CHANNEL_BLUE);

    // cycle through colors with one-second intervals between each change
    while (true) {
        set_color(255, 0,   0);   // red
        vTaskDelay(pdMS_TO_TICKS(1000));
        set_color(0,   255, 0);   // green
        vTaskDelay(pdMS_TO_TICKS(1000));
        set_color(0,   0,   255); // blue
        vTaskDelay(pdMS_TO_TICKS(1000));
        set_color(255, 255, 0);   // yellow
        vTaskDelay(pdMS_TO_TICKS(1000));
        set_color(80,  0,   80);  // purple
        vTaskDelay(pdMS_TO_TICKS(1000));
        set_color(0,   255, 255); // cyan
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

// configures a single LEDC channel and binds it to the given GPIO pin
void setup_led(gpio_num_t pin, ledc_channel_t channel) {
    ledc_channel_config_t channel_config = {
        .channel    = channel,
        .gpio_num   = pin,
        .duty       = 0,           // start with LED off
        .hpoint     = 0,
        .speed_mode = LEDC_SPEED_MODE,
        .timer_sel  = LEDC_TIMER,
    };
    ESP_ERROR_CHECK(ledc_channel_config(&channel_config));
}

// sets the RGB LED color by writing duty cycle values to each channel
// red, green, blue: brightness values in the range 0-255
void set_color(uint32_t red, uint32_t green, uint32_t blue) {
    ESP_ERROR_CHECK(ledc_set_duty(LEDC_SPEED_MODE, LEDC_CHANNEL_RED,   red));
    ESP_ERROR_CHECK(ledc_set_duty(LEDC_SPEED_MODE, LEDC_CHANNEL_GREEN, green));
    ESP_ERROR_CHECK(ledc_set_duty(LEDC_SPEED_MODE, LEDC_CHANNEL_BLUE,  blue));

    ESP_ERROR_CHECK(ledc_update_duty(LEDC_SPEED_MODE, LEDC_CHANNEL_RED));
    ESP_ERROR_CHECK(ledc_update_duty(LEDC_SPEED_MODE, LEDC_CHANNEL_GREEN));
    ESP_ERROR_CHECK(ledc_update_duty(LEDC_SPEED_MODE, LEDC_CHANNEL_BLUE));
}
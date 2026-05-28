/**
 * @file main.c
 * @brief Fading — creates a breathing effect on an LED using PWM on GPIO26.
 *        ESP-IDF translation of SunFounder ESP32 Starter Kit lesson 2.2.
 */

#include "esp_err.h"
#include "driver/ledc.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

// GPIO pin connected to the LED via a 220ohm current limiting resistor
#define LED_PIN         GPIO_NUM_26

// LEDC configuration
#define LEDC_FREQ_HZ    5000              // PWM frequency in Hz
#define LEDC_DUTY_RES   LEDC_TIMER_8_BIT  // 8-bit resolution: duty cycle range 0-255
#define LEDC_TIMER      LEDC_TIMER_0
#define LEDC_CHANNEL    LEDC_CHANNEL_0
#define LEDC_SPEED_MODE LEDC_LOW_SPEED_MODE
#define LEDC_CLK_CFG    LEDC_AUTO_CLK

void app_main(void) {

    int16_t brightness  = 0; // current brightness level of the LED
    int16_t fade_amount = 5; // amount to change brightness by each step

    // configure LEDC timer: sets PWM frequency and duty cycle resolution
    ledc_timer_config_t timer_config = {
        .timer_num       = LEDC_TIMER,
        .duty_resolution = LEDC_DUTY_RES,
        .freq_hz         = LEDC_FREQ_HZ,
        .clk_cfg         = LEDC_CLK_CFG,
        .speed_mode      = LEDC_SPEED_MODE,
    };
    ESP_ERROR_CHECK(ledc_timer_config(&timer_config));

    // configure LEDC channel: binds GPIO26 to the timer and sets initial duty
    ledc_channel_config_t channel_config = {
        .gpio_num   = LED_PIN,
        .timer_sel  = LEDC_TIMER,
        .channel    = LEDC_CHANNEL,
        .speed_mode = LEDC_SPEED_MODE,
        .duty       = brightness,
        .hpoint     = 0,
    };
    ESP_ERROR_CHECK(ledc_channel_config(&channel_config));

    while (true) {
        // write the new brightness value to the PWM channel
        ESP_ERROR_CHECK(ledc_set_duty(LEDC_SPEED_MODE, LEDC_CHANNEL, brightness));
        ESP_ERROR_CHECK(ledc_update_duty(LEDC_SPEED_MODE, LEDC_CHANNEL));

        // increment brightness by fade_amount each step
        brightness += fade_amount;

        // reverse fade direction at min and max brightness
        if (brightness <= 0 || brightness >= 255) {
            fade_amount = -fade_amount;
        }

        vTaskDelay(pdMS_TO_TICKS(50)); // wait 50 milliseconds
    }
}
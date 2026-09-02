/**
 * @file main.c
 * @brief Hello, LED! — blinks an LED on GPIO26 once per second.
 *        ESP-IDF translation of SunFounder ESP32 Starter Kit lesson 2.1.
 */

#include "esp_err.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

// GPIO pin connected to the LED via a 220ohm current limiting resistor
#define LED_PIN GPIO_NUM_26

void app_main(void) {

    // configure GPIO26 as a push-pull output with no pull resistors and no interrupt
    gpio_config_t led_config = {
        .mode         = GPIO_MODE_OUTPUT,
        .pin_bit_mask = (1ULL << LED_PIN),
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .pull_up_en   = GPIO_PULLUP_DISABLE,
        .intr_type    = GPIO_INTR_DISABLE,
    };
    ESP_ERROR_CHECK(gpio_config(&led_config));

    while (true) {
        gpio_set_level(LED_PIN, 1);             // turn the LED on (HIGH)
        vTaskDelay(pdMS_TO_TICKS(1000));        // wait for a second

        gpio_set_level(LED_PIN, 0);             // turn the LED off (LOW)
        vTaskDelay(pdMS_TO_TICKS(1000));        // wait for a second
    }
}
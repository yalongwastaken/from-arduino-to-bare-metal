/**
 * @file main.c
 * @brief Lesson 2.4 — 74HC595 shift register driving a flowing 8-LED display.
 *
 * Bit-bangs the 74HC595 over three GPIOs (serial data + shift clock + latch
 * clock). Shifts one byte MSB-first into the shift register, then pulses the
 * latch to present it on Q0..Q7. No inter-bit delays are needed: the chip's
 * DS->SHCP setup time is ~10-20 ns, far below the spacing between consecutive
 * gpio_set_level() calls.
 */

#include <stdint.h>
#include "esp_err.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

// GPIO pins
#define STcp_PIN GPIO_NUM_27   // storage/latch clock (ST_CP / RCLK)
#define SHcp_PIN GPIO_NUM_26   // shift clock          (SH_CP / SRCLK)
#define DS_PIN   GPIO_NUM_25   // serial data          (DS / SER)

// data patterns for 8 LEDs — progressively fill Q0..Q7
static const uint8_t dat_array[] = {
    0x00, 0x01, 0x03,
    0x07, 0x0F, 0x1F,
    0x3F, 0x7F, 0xFF
};

#define DAT_ARRAY_LEN (sizeof(dat_array) / sizeof(dat_array[0]))

// function prototypes
static esp_err_t shift_out(uint8_t data);

void app_main(void) {
    // setup — configure DS, SHcp, STcp as outputs
    gpio_config_t config = {
        .mode = GPIO_MODE_OUTPUT,
        .pin_bit_mask = ((1ULL << DS_PIN) | (1ULL << SHcp_PIN) | (1ULL << STcp_PIN)),
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .intr_type = GPIO_INTR_DISABLE
    };
    ESP_ERROR_CHECK(gpio_config(&config));

    // initialize to a known state
    ESP_ERROR_CHECK(gpio_set_level(STcp_PIN, 1));
    ESP_ERROR_CHECK(gpio_set_level(SHcp_PIN, 0));
    ESP_ERROR_CHECK(gpio_set_level(DS_PIN, 0));

    // loop
    while (true) {
        for (size_t i = 0; i < DAT_ARRAY_LEN; i++) {
            // hold latch low while shifting
            ESP_ERROR_CHECK(gpio_set_level(STcp_PIN, 0));

            // shift the byte into the shift register
            ESP_ERROR_CHECK(shift_out(dat_array[i]));

            // pulse latch high to transfer to the output register (Q0..Q7)
            ESP_ERROR_CHECK(gpio_set_level(STcp_PIN, 1));

            vTaskDelay(pdMS_TO_TICKS(1000));
        }
    }
}

/**
 * @brief Shift one byte out to the 74HC595, MSB first.
 *
 * For each bit (7 down to 0): drive DS, then pulse SHCP high->low. The chip
 * latches DS on SHCP's rising edge. No delays needed (see file header).
 */
static esp_err_t shift_out(uint8_t data) {
    for (int i = 0; i < 8; i++) {
        // MSB first: bit 7 is sent on the first iteration
        uint8_t bit = (data >> (7 - i)) & 0x01;
        ESP_ERROR_CHECK(gpio_set_level(DS_PIN, bit));

        // pulse the shift clock — data captured on the rising edge
        ESP_ERROR_CHECK(gpio_set_level(SHcp_PIN, 1));
        ESP_ERROR_CHECK(gpio_set_level(SHcp_PIN, 0));
    }
    return ESP_OK;
}
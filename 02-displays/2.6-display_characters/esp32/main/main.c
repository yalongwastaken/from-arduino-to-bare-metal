/**
 * @file main.c
 * @author Anthony Yalong
 * @brief Lesson 2.6 — I2C LCD1602 display driver demo.
 *
 * Initializes the I2C master bus and LCD1602 display via a PCF8574 I/O
 * expander. Prints a static greeting, then counts upward once per second.
 * LCD API operates in 4-bit mode; all I2C lifecycle is managed by the i2c
 * driver component.
 */

#include "lcd.h"
#include "i2c.h"
#include "esp_err.h"
#include "esp_log.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

// GPIO pins
#define SDA_PIN GPIO_NUM_21    // I2C data line
#define SCL_PIN GPIO_NUM_22    // I2C clock line

// display geometry
#define LCD_COLS 16
#define LCD_ROWS 2

static const char *TAG = "main";

void app_main(void) {
    esp_err_t ret;

    // setup — initialize I2C bus and retrieve handle
    ret = i2c_bus_init(SDA_PIN, SCL_PIN);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "i2c init failed");
        return;
    }

    i2c_master_bus_handle_t i2c_handle = NULL;
    ret = i2c_bus_get_handle(&i2c_handle);
    if (ret != ESP_OK) {
        ESP_ERROR_CHECK(i2c_bus_deinit());
        return;
    }

    // initialize LCD
    lcd_handle_t lcd = {0};
    ret = lcd_init(i2c_handle, LCD_COLS, LCD_ROWS, &lcd);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "lcd init failed");
        ESP_ERROR_CHECK(i2c_bus_deinit());
        return;
    }

    // greeting
    ESP_ERROR_CHECK(lcd_backlight(&lcd, true));
    ESP_ERROR_CHECK(lcd_set_cursor(&lcd, 0, 0));
    ESP_ERROR_CHECK(lcd_print(&lcd, "hello world!"));
    vTaskDelay(pdMS_TO_TICKS(3000));

    // loop — count upward once per second
    int count = 0;
    while (true) {
        ESP_ERROR_CHECK(lcd_clear(&lcd));
        ESP_ERROR_CHECK(lcd_set_cursor(&lcd, 0, 0));
        ESP_ERROR_CHECK(lcd_print(&lcd, "COUNT: "));
        ESP_ERROR_CHECK(lcd_print_int(&lcd, count));
        vTaskDelay(pdMS_TO_TICKS(1000));
        count++;
    }
}
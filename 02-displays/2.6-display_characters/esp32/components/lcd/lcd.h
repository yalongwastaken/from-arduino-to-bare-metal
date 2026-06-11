/**
 * @file lcd.h
 * @author Anthony Yalong
 * @brief I2C LCD1602 display driver.
 */
#pragma once

#include <stdbool.h>
#include <stdint.h>
#include "driver/i2c_master.h"
#include "esp_err.h"

// ── Commands ──────────────────────────────────────────────────────────────────

#define LCD_CMD_CLEAR        0x01
#define LCD_CMD_HOME         0x02
#define LCD_CMD_ENTRY_MODE   0x04
#define LCD_CMD_DISPLAY_CTRL 0x08
#define LCD_CMD_FUNCTION_SET 0x20
#define LCD_CMD_DDRAM_ADDR   0x80

// ── Flags ─────────────────────────────────────────────────────────────────────

#define LCD_ENTRY_LEFT   0x02
#define LCD_DISPLAY_ON   0x04
#define LCD_4BIT_MODE    0x00  // 4-bit mode: DL bit clear
#define LCD_2_LINE       0x08
#define LCD_5x8_DOTS     0x00  // 5x8 font: F bit clear
#define LCD_BACKLIGHT    0x08
#define LCD_NO_BACKLIGHT 0x00
#define LCD_EN           0x04
#define LCD_RW           0x02
#define LCD_RS           0x01

// ── Config ────────────────────────────────────────────────────────────────────

#define I2C_TIMEOUT_MS 1000

// ── Types ─────────────────────────────────────────────────────────────────────

/**
 * @brief LCD device handle. Holds I2C device handle and display configuration.
 */
typedef struct {
    i2c_master_dev_handle_t dev;
    uint8_t cols;
    uint8_t rows;
    uint8_t backlight_state;
} lcd_handle_t;

// ── Public API ────────────────────────────────────────────────────────────────

/**
 * @brief Initialize the LCD display.
 * @param[in]  dev   Pre-configured I2C device handle (address 0x27 set by caller)
 * @param[in]  cols  Number of columns (typically 16)
 * @param[in]  rows  Number of rows (typically 2)
 * @param[out] lcd   Pointer to LCD handle to populate
 * @return ESP_OK on success, or an esp_err_t error code on failure
 */
esp_err_t lcd_init(i2c_master_dev_handle_t dev, uint8_t cols, uint8_t rows, lcd_handle_t *lcd);

/**
 * @brief Clear the display and return cursor to home position.
 * @param[in] lcd   Pointer to initialized LCD handle
 * @return ESP_OK on success, or an esp_err_t error code on failure
 */
esp_err_t lcd_clear(lcd_handle_t *lcd);

/**
 * @brief Set the cursor position.
 * @param[in] lcd   Pointer to initialized LCD handle
 * @param[in] col   Column (0 to cols-1)
 * @param[in] row   Row (0 to rows-1)
 * @return ESP_OK on success, or an esp_err_t error code on failure
 */
esp_err_t lcd_set_cursor(lcd_handle_t *lcd, uint8_t col, uint8_t row);

/**
 * @brief Control the backlight.
 * @param[in] lcd    Pointer to initialized LCD handle
 * @param[in] state  true = on, false = off
 * @return ESP_OK on success, or an esp_err_t error code on failure
 */
esp_err_t lcd_backlight(lcd_handle_t *lcd, bool state);

/**
 * @brief Print a null-terminated string at the current cursor position.
 * @param[in] lcd   Pointer to initialized LCD handle
 * @param[in] str   Null-terminated string to print
 * @return ESP_OK on success, or an esp_err_t error code on failure
 */
esp_err_t lcd_print(lcd_handle_t *lcd, const char *str);

/**
 * @brief Print an integer at the current cursor position.
 * @param[in] lcd    Pointer to initialized LCD handle
 * @param[in] value  Integer to print
 * @return ESP_OK on success, or an esp_err_t error code on failure
 */
esp_err_t lcd_print_int(lcd_handle_t *lcd, int value);
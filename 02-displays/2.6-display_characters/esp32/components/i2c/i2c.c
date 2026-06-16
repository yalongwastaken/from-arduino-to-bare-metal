/**
 * @file i2c.c
 * @author Anthony Yalong
 * @brief I2C bus initialization and handle management implementation.
 */

#include "i2c.h"
#include "esp_err.h"
#include "driver/gpio.h"
#include "driver/i2c_master.h"

#define I2C_PORT            I2C_NUM_0
#define I2C_CLK_SRC         I2C_CLK_SRC_DEFAULT
#define I2C_GLITCH_IGNR_CNT 7
#define I2C_EN_PULLUP       true

static i2c_master_bus_handle_t s_handle = NULL;

esp_err_t i2c_bus_init(gpio_num_t sda_pin, gpio_num_t scl_pin) {
    // guard against double-init (would leak the existing bus)
    if (s_handle != NULL) return ESP_ERR_INVALID_STATE;

    i2c_master_bus_config_t cfg = {
        .i2c_port                     = I2C_PORT,
        .scl_io_num                   = scl_pin,
        .sda_io_num                   = sda_pin,
        .clk_source                   = I2C_CLK_SRC,
        .glitch_ignore_cnt            = I2C_GLITCH_IGNR_CNT,
        .flags.enable_internal_pullup = I2C_EN_PULLUP,
    };
    return i2c_new_master_bus(&cfg, &s_handle);
}

esp_err_t i2c_bus_get_handle(i2c_master_bus_handle_t *handle) {
    if (handle == NULL) return ESP_ERR_INVALID_ARG;
    if (s_handle == NULL) return ESP_ERR_INVALID_STATE;

    *handle = s_handle;
    return ESP_OK;
}

esp_err_t i2c_bus_deinit(void) {
    if (s_handle == NULL) return ESP_ERR_INVALID_STATE;

    esp_err_t ret = i2c_del_master_bus(s_handle);
    if (ret != ESP_OK) return ret;

    s_handle = NULL;
    return ESP_OK;
}
/**
 * @file i2c.h
 * @brief I2C bus initialization and handle management.
 */

#pragma once
#include "esp_err.h"
#include "driver/gpio.h"
#include "driver/i2c_master.h"

/**
 * @brief Initializes the I2C master bus.
 * @param[in] sda_pin   GPIO number for hte SDA line
 * @param[in] scl_pin   GPIO number for the SCL line
 * @return ESP_OK on success, or an esp_err_t error code on failure
 */
esp_err_t i2c_init(gpio_num_t sda_pin, gpio_num_t scl_pin);

/**
 * @brief Returns the internal I2C master bus handle.
 * @param[in] handle    Pointer to store the bus handle
 * @return ESP_OK on success, or an esp_err_t error code on failure
 */
esp_err_t i2c_get_handle(i2c_master_bus_handle_t *handle);

/**
 * @brief Deinitializes the I2C master bus.
 * @return ESP_OK on success, or an esp_err_t error code on failure
 */
esp_err_t i2c_deinit(void);
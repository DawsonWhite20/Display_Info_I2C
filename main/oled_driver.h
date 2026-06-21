#ifndef OLED_DRIVER_H
#define OLED_DRIVER_H

#include "driver/i2c_master.h"
#include "esp_err.h"

/**
 *  @brief Initializes the OLED display
 *  @param bus_handle The handle of the I2C master bus
 *  @param out_device_handle The handle of the initialized OLED display device
 *  @return ESP_OK if successful, otherwise an error code
 */
esp_err_t oled_init(i2c_master_bus_handle_t bus_handle, i2c_master_dev_handle_t *out_device_handle);

#endif

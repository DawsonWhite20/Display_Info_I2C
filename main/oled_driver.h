#ifndef OLED_DRIVER_H
#define OLED_DRIVER_H

#include "driver/i2c_master.h"
#include "esp_err.h"

/**
 * @brief Registers the io and panel handle into the master bus as well as sets the screen up for use
 * @param bus_handle The handle of the I2C master bus
 * @param out_io_handle The handle of the LCD IO handle
 * @param out_panel_handle The handle of the LCD panel handle
 * @return ESP_OK if successful, otherwise an error code
 */
esp_err_t oled_sensor_init(i2c_master_bus_handle_t bus_handle, 
    esp_lcd_panel_io_handle_t *out_io_handle, esp_lcd_panel_handle_t *out_panel_handle);

#endif

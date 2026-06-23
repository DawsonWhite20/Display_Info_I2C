#ifndef OLED_DRIVER_H
#define OLED_DRIVER_H
    
#include <stdbool.h>
#include "driver/i2c_master.h"
#include "esp_lcd_panel_io.h"
#include "esp_lcd_panel_vendor.h"

/**
 * @brief Registers the io and panel handle into the master bus as well as sets the screen up for use
 * @param bus_handle The handle of the I2C master bus
 * @param out_io_handle The handle of the LCD IO
 * @param out_panel_handle The handle of the LCD panel
 * @return ESP_OK if successful, otherwise an error code
 */
esp_err_t oled_sensor_init(i2c_master_bus_handle_t bus_handle, 
    esp_lcd_panel_io_handle_t *out_io_handle, esp_lcd_panel_handle_t *out_panel_handle);

/**
 * @brief Draws a pixel at a set coordinate on the OLED
 * @param x X coordinate
 * @param y Y coordinate
 * @param color Draws a white pixel if 1 and stays black if 0
 */
void oled_draw_pixel(int x, int y, bool color);

/**
 * @brief Displays the OLED screen with the bitmap held in RAM by the framebuffer 
 * and clears the memory from RAM for the next call
 * @param out_panel_handle The handle of the LCD panel
 */
void oled_refresh_screen(esp_lcd_panel_handle_t out_panel_handle);

/**
 * @brief Renders characters from the string pixel-by-pixel and passes each one into oled_draw_pixel
 * @param x Column of the font matrix
 * @param y Row of the font matrix
 * @param str Text to be displayed to the OLED
 */
void oled_draw_string(int x, int y, const char *str);

#endif

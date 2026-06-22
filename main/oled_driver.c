#include <stdio.h>
#include "driver/i2c_master.h"
#include "oled_driver.h"
#include "esp_lcd_panel_io.h"
#include "esp_lcd_io_i2c.h"
#include "esp_lcd_panel_vendor.h"
#include "esp_lcd_panel_ops.h"

#define OLED_DEVICE_ADDRESS 0x3C
#define OLED_SCL_SPEED_HZ 400000 // OLED is practically sent an image, so processing needs to move faster

static uint8_t framebuffer[1024];

esp_err_t oled_sensor_init(i2c_master_bus_handle_t bus_handle, 
    esp_lcd_panel_io_handle_t *out_io_handle, esp_lcd_panel_handle_t *out_panel_handle) {
        // OLED io settings
        esp_lcd_panel_io_i2c_config_t io_config = {
            .dev_addr = OLED_DEVICE_ADDRESS,
            .scl_speed_hz = OLED_SCL_SPEED_HZ,
            .control_phase_bytes = 1,
            .dc_bit_offset = 6,
            .lcd_cmd_bits = 8, // Every command sent is 1 byte
            .lcd_param_bits = 8, // Every parameter sent is 1 byte
        };

        ESP_ERROR_CHECK(esp_lcd_new_panel_io_i2c(bus_handle, &io_config, out_io_handle));

        // OLED panel settings
        esp_lcd_panel_dev_config_t panel_config = {
            .bits_per_pixel = 1,
            .reset_gpio_num = -1, // No physical reset gpio on OLED board
        };

        ESP_ERROR_CHECK(esp_lcd_new_panel_ssd1306(*out_io_handle, &panel_config, out_panel_handle));

        // Routine to power on the hardware
        esp_lcd_panel_reset(*out_panel_handle);
        esp_lcd_panel_init(*out_panel_handle);
        esp_lcd_panel_disp_on_off(*out_panel_handle, true);

        memset(framebuffer, 0, sizeof(framebuffer));
        esp_lcd_panel_draw_bitmap(*out_panel_handle, 0, 0, 128, 64, framebuffer);
        
        return ESP_OK;
}

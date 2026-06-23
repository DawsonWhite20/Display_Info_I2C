#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "driver/i2c_master.h"
#include "esp_lcd_panel_io.h"
#include "esp_lcd_io_i2c.h"
#include "esp_lcd_panel_vendor.h"
#include "esp_lcd_panel_ops.h"

#include "oled_driver.h"

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

        memset(framebuffer, 0, sizeof(framebuffer)); // Clears the RAM of the OLED bitmap
        esp_lcd_panel_draw_bitmap(*out_panel_handle, 0, 0, 128, 64, framebuffer);
        
        return ESP_OK;
}

void oled_draw_pixel(int x, int y, bool color) {
    // Returns if the x or y coordinate are outside the 128 x 64 pixel size of the OLED screen
    if (x < 0 || x >= 128 || y < 0 || y >= 64) return;

    // Calculates both the byte and bit index of the pixel
    int byte_index = x + (y / 8) * 128;
    int bit_index = y % 8;

    if (color) {
        framebuffer[byte_index] |= (1 << bit_index); // Bitwise OR draws a white pixel at the bit 
    }
    else {
        framebuffer[byte_index] &= ~(1 << bit_index); // Bitwise AND-NOT clears the pixel at the bit
    }
}

void oled_refresh_screen(esp_lcd_panel_handle_t out_panel_handle) {
    // Draw the entire canvas on the OLED
    esp_lcd_panel_draw_bitmap(out_panel_handle, 0, 0, 128, 64, framebuffer);

    // Clears the RAM of the OLED bitmap
    memset(framebuffer, 0, sizeof(framebuffer));
}

static const uint8_t font8x8[][8] = {
    [' '] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    ['-'] = {0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x00},
    [':'] = {0x00, 0x00, 0x24, 0x00, 0x00, 0x00, 0x00, 0x00},
    ['A'] = {0x7C, 0x12, 0x11, 0x12, 0x7C, 0x00, 0x00, 0x00},
    ['G'] = {0x3E, 0x41, 0x49, 0x4A, 0x3A, 0x00, 0x00, 0x00},
    ['X'] = {0x63, 0x14, 0x08, 0x14, 0x63, 0x00, 0x00, 0x00},
    ['Y'] = {0x07, 0x08, 0x70, 0x08, 0x07, 0x00, 0x00, 0x00},
    ['Z'] = {0x61, 0x51, 0x49, 0x45, 0x43, 0x00, 0x00, 0x00},
    ['0'] = {0x3E, 0x51, 0x49, 0x45, 0x3E, 0x00, 0x00, 0x00},
    ['1'] = {0x00, 0x42, 0x7F, 0x40, 0x00, 0x00, 0x00, 0x00},
    ['2'] = {0x42, 0x61, 0x51, 0x49, 0x46, 0x00, 0x00, 0x00},
    ['3'] = {0x21, 0x41, 0x45, 0x4B, 0x31, 0x00, 0x00, 0x00},
    ['4'] = {0x18, 0x14, 0x12, 0x7F, 0x10, 0x00, 0x00, 0x00},
    ['5'] = {0x27, 0x45, 0x45, 0x45, 0x39, 0x00, 0x00, 0x00},
    ['6'] = {0x3C, 0x4A, 0x49, 0x49, 0x30, 0x00, 0x00, 0x00},
    ['7'] = {0x01, 0x71, 0x09, 0x05, 0x03, 0x00, 0x00, 0x00},
    ['8'] = {0x36, 0x49, 0x49, 0x49, 0x36, 0x00, 0x00, 0x00},
    ['9'] = {0x06, 0x49, 0x49, 0x29, 0x1E, 0x00, 0x00, 0x00}
};

void oled_draw_string(int x, int y, const char *str) {
    while (*str) {
        char c = *str;
        if ((c < 128 && font8x8[(unsigned char)c][0] != 0x00) || c == ' ') {
            for (int col = 0; col < 8; col++) {
                uint8_t line = font8x8[(unsigned char)c][col];
                for (int row = 0; row < 8; row++) {
                    if (line & (1 << row)) {
                        oled_draw_pixel(x + col, y + row, true);
                    }
                }
            }
        }
        x += 8;
        str++;
    }
}

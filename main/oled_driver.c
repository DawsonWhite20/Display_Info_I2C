#include <stdio.h>
#include "driver/i2c_master.h"
#include "u8g2.h"

#include "oled_driver.h"

#define OLED_DEVICE_ADDRESS 0x3C
#define OLED_SCL_SPEED_HZ 400000 // OLED is practically sent an image, so processing needs to move faster

// Used so the U8g2 cb callback function can transmit it's data
static i2c_master_dev_handle_t u8g2_oled_handle;
static u8g2_t u8g2;

// Callback function that performs actions on the i2c transmission based on the stage it's in
static uint8_t u8g2_esp32_i2c_byte_cb(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr) {
    static uint8_t buffer[32];
    static uint8_t buf_index;

    switch (msg) {
        case U8X8_MSG_BYTE_INIT:
            break;
        case U8X8_MSG_BYTE_START_TRANSFER:
            buf_index = 0;
            break;
        case U8X8_MSG_BYTE_SEND: {
            uint8_t *data = (uint8_t *)arg_ptr;
            while (arg_int > 0) {
                buffer[buf_index++] = *data++;
                arg_int--;
            }
            break;
        }
        case U8X8_MSG_BYTE_END_TRANSFER:
        // Sends the graphics down the master bus
        if (u8g2_oled_handle != NULL) {
            i2c_master_transmit(u8g2_oled_handle, buffer, buf_index, -1);
        }
            break;
        default:
            return 0; 
    }
    return 1;
};

esp_err_t oled_sensor_init(i2c_master_bus_handle_t bus_handle, i2c_master_dev_handle_t *out_device_handle) {
    // OLED device settings
    i2c_device_config_t dev_cfg = {
        .dev_addr_length = I2C_ADDR_BIT_LEN_7,
        .device_address = OLED_DEVICE_ADDRESS,
        .scl_speed_hz = OLED_SCL_SPEED_HZ,
    };

    return i2c_master_bus_add_device(bus_handle, &dev_cfg, out_device_handle);
};

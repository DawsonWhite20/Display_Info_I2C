#include "driver/i2c_master.h"
#include "u8g2.h"

#include "oled_driver.h"

#define OLED_DEVICE_ADDRESS 0x3C
#define OLED_SCL_SPEED_HZ 400000 // OLED is practically sent an image, so processing needs to move faster

esp_err_t oled_sensor_init(i2c_master_bus_handle_t bus_handle, i2c_master_dev_handle_t *out_device_handle) {
    // OLED device settings
    i2c_device_config_t dev_cfg = {
        .dev_addr_length = I2C_ADDR_BIT_LEN_7,
        .device_address = OLED_DEVICE_ADDRESS,
        .scl_speed_hz = OLED_SCL_SPEED_HZ,
    };

    return i2c_master_bus_add_device(bus_handle, &dev_cfg, out_device_handle);
};

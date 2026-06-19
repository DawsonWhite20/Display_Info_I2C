#include <stdio.h>
#include "esp_log.h"
#include "driver/i2c_master.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

// Only two options for scl and sda for esp32 without additional routing
#define I2C_MASTER_SCL_IO  22
#define I2C_MASTER_SDA_IO  21
// Defines first port
#define I2C_MASTER_PORT_NUM  I2C_NUM_0

static const char *TAG = "MAIN";

i2c_master_bus_handle_t init_i2C_master_port(void) {
    // Configures the master bus
    i2c_master_bus_config_t i2c_mst_config = {
        .i2c_port = I2C_MASTER_PORT_NUM,
        .clk_source = I2C_CLK_SRC_DEFAULT,
        .scl_io_num = I2C_MASTER_SCL_IO,
        .sda_io_num = I2C_MASTER_SDA_IO,
        .glitch_ignore_cnt = 7,
        // Pins need internal pullup
        .flags.enable_internal_pullup = true,
    };

    // Alocates a handles to keep track of initialized port
    i2c_master_bus_handle_t bus_handle;

    // Initialize port hardware using driver
    ESP_ERROR_CHECK(i2c_new_master_bus(&i2c_mst_config, &bus_handle));

    return bus_handle;
}

i2c_master_dev_handle_t init_mpu_device(i2c_master_bus_handle_t bus_handle) {
        // Configures accelerometer/gyroscope device
        i2c_device_config_t dev_cfg = {
        .dev_addr_length = I2C_ADDR_BIT_LEN_7,
        .device_address = 0x68,
        .scl_speed_hz = 100000,
    };

    i2c_master_dev_handle_t mpu_handle;
    ESP_ERROR_CHECK(i2c_master_bus_add_device(bus_handle, &dev_cfg, &mpu_handle));

    return mpu_handle;
}

void app_main(void)
{
    // Initializes the master bus
    i2c_master_bus_handle_t main_bus_handle = init_i2C_master_port();

    // Initializes the mpu6500 device into the master bus
    i2c_master_dev_handle_t mpu_handle = init_mpu_device(main_bus_handle);

    while(1) {
        uint8_t buffer[6] = {0};
        uint8_t reg_addr = 0x3B;

        // Pass in mpu_handle as well as how many bytes for the esp32 to read and receive 
        // Catch the return status code into an esp_err_t variable
        esp_err_t ret = i2c_master_transmit_receive(mpu_handle, &reg_addr, 1, buffer, 6, -1);

        // Check if the transaction succeeded
        if (ret == ESP_OK) {
            // Print the 6 raw hex bytes trapped in your array elements
            ESP_LOGI(TAG, "Success! Raw Bytes: [0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X]", 
                     buffer[0], buffer[1], buffer[2], buffer[3], buffer[4], buffer[5]);
        } else {
            // This will print if lines are disconnected or target is wrong, without crashing
            ESP_LOGE(TAG, "I2C Transaction Failed! Error code: %s", esp_err_to_name(ret));
        }

        vTaskDelay(pdMS_TO_TICKS(500));
    }
}
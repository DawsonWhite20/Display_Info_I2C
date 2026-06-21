#include "driver/i2c_master.h"

#include "sensor_driver.h"

// Private constants
#define MPU_DEVICE_ADDRESS 0x68
#define MPU_SCL_SPEED_HZ 100000
#define MPU_REG_ACCEL_XOUT_H 0x3B // Starting register for accelerometer data
#define MPU_REG_GYRO_XOUT_H 0x43 // Starting register for gyroscope data

esp_err_t mpu_sensor_init(i2c_master_bus_handle_t bus_handle, i2c_master_dev_handle_t *out_device_handle) {
    // MPU device settings
    i2c_device_config_t dev_cfg = {
        .dev_addr_length = I2C_ADDR_BIT_LEN_7,
        .device_address = MPU_DEVICE_ADDRESS,
        .scl_speed_hz = MPU_SCL_SPEED_HZ,
    };

    // Add MPU device to master bus after configuration
    return i2c_master_bus_add_device(bus_handle, &dev_cfg, out_device_handle);
};

esp_err_t mpu_sensor_read_accel(i2c_master_dev_handle_t device_handle, mpu_raw_data_t *out_data) {
    if (out_data == NULL) {
        return ESP_ERR_INVALID_ARG;
    }

    // Array of six, 8 bit integers (1 byte each) 
    uint8_t buffer[6] = {0};
    uint8_t reg_addr = MPU_REG_ACCEL_XOUT_H;

    // Read and write between ESP32 and MPU device
    esp_err_t ret = i2c_master_transmit_receive(device_handle, &reg_addr, 1, buffer, 6, -1);

    if (ret == ESP_OK) {
        // Combining MSB's and LSB's to 16-bit form
        out_data -> x_accel = (int16_t) ((buffer[0] << 8) | buffer[1]);
        out_data -> y_accel = (int16_t) ((buffer[2] << 8) | buffer[3]);
        out_data -> z_accel = (int16_t) ((buffer[4] << 8) | buffer[5]);
    }

    return ret;
};

esp_err_t mpu_sensor_read_gyro(i2c_master_dev_handle_t device_handle, mpu_raw_data_t *out_data) {
    if (out_data == NULL) {
        return ESP_ERR_INVALID_ARG;
    }

    // Array of six, 8 bit integers (1 byte each)
    uint8_t buffer[6] = {0};
    uint8_t reg_addr = MPU_REG_GYRO_XOUT_H;

    // Read and write between ESP32 and MPU device
    esp_err_t ret = i2c_master_transmit_receive(device_handle, &reg_addr, 1, buffer, 6, -1);

    if (ret == ESP_OK) {
        // Combining MSB's and LSB's to 16-bit form
        out_data -> x_gyro = (int16_t) ((buffer[0] << 8) | buffer[1]);
        out_data -> y_gyro = (int16_t) ((buffer[2] << 8) | buffer[3]);
        out_data -> z_gyro = (int16_t) ((buffer[4] << 8) | buffer[5]);
    }

    return ret;
};

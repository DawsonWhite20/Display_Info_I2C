#ifndef SENSOR_DRIVER_H
#define SENSOR_DRIVER_H

#include "driver/i2c_master.h"
#include "esp_err.h"

// Holds 16 bit processed mpu accelerometer and gyroscope sensor data
typedef struct {
    int16_t x_accel;
    int16_t y_accel;
    int16_t z_accel;

    int16_t x_gyro;
    int16_t y_gyro;
    int16_t z_gyro;
} mpu_raw_data_t;

/**
 *  @brief Registers the MPU sensor device onto the master bus
 *  @param bus_handle The handle of the I2C master bus
 *  @param out_device_handle The handle of thex MPU sensor device
 *  @return ESP_OK if successful, otherwise an error code
 */

esp_err_t mpu_sensor_init(i2c_master_bus_handle_t bus_handle, i2c_master_dev_handle_t *out_device_handle);

/**
 *  @brief Reads the raw accelerometer data from the MPU sensor
 *  @param device_handle The handle of the MPU sensor device
 *  @param out_data The buffer to store the raw accelerometer data
 *  @return ESP_OK if successful, otherwise an error code
 */
esp_err_t mpu_sensor_read_accel(i2c_master_dev_handle_t device_handle, mpu_raw_data_t *out_data);

/**
 *  @brief Reads the raw gyroscope data from the MPU sensor
 *  @param device_handle The handle of the MPU sensor device
 *  @param out_data The buffer to store the raw gyroscope data
 *  @return ESP_OK if successful, otherwise an error code
 */
esp_err_t mpu_sensor_read_gyro(i2c_master_dev_handle_t device_handle, mpu_raw_data_t *out_data);

#endif

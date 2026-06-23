#include <stdio.h>
#include "esp_log.h"
#include "driver/i2c_master.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_lcd_io_i2c.h"

#include "sensor_driver.h"
#include "oled_driver.h"

// Only two options for scl and sda for esp32 without additional routing
#define I2C_MASTER_SCL  22
#define I2C_MASTER_SDA  21
#define I2C_PORT_NUM I2C_NUM_0

static const char *TAG = "MAIN";

void app_main(void) {
    char str_ax[16], str_ay[16], str_az[16];
    char str_gx[16], str_gy[16], str_gz[16];

    i2c_master_bus_handle_t master_bus_handle;

    // Configure master bus
    i2c_master_bus_config_t bus_cfg = {
        .i2c_port = I2C_PORT_NUM,
        .scl_io_num = I2C_MASTER_SCL,
        .sda_io_num = I2C_MASTER_SDA,
        .clk_source = I2C_CLK_SRC_DEFAULT,
        .glitch_ignore_cnt = 7,
        .flags.enable_internal_pullup = true,
    };

    // Init master bus and add MPU sensor to the bus

    ESP_ERROR_CHECK(i2c_new_master_bus(&bus_cfg, &master_bus_handle));

    // Address of mpu handle will later be passed into mpu_sensor_init() and added to master bus
    i2c_master_dev_handle_t mpu_handle = NULL;
    ESP_ERROR_CHECK(mpu_sensor_init(master_bus_handle, &mpu_handle));

    // OLED initialization of io and panel handles
    esp_lcd_panel_io_handle_t oled_io_handle = NULL;
    esp_lcd_panel_handle_t oled_panel_handle = NULL;
    ESP_ERROR_CHECK(oled_sensor_init(master_bus_handle, &oled_io_handle, &oled_panel_handle));

    while (1) {
        // Typedef struct from sensor_driver.h
        mpu_raw_data_t sensor_values; 
        esp_err_t ret;

        ret = mpu_sensor_read_accel(mpu_handle, &sensor_values);

        if (ret == ESP_OK) {
            // Print to the console bit-shifted x, y, and z data from acceleremoter
            ESP_LOGI(TAG, "Accelerometer -> x: %d, y: %d, z: %d", 
                sensor_values.x_accel, sensor_values.y_accel, sensor_values.z_accel);
        }
        else {
            // Used for critical failure
            ESP_LOGE(TAG, "Error: %s", esp_err_to_name(ret));
        }

        ret = mpu_sensor_read_gyro(mpu_handle, &sensor_values);

        if (ret == ESP_OK) {
            // Print to the console bit-shifted x, y, and z data from gyroscope
            ESP_LOGI(TAG, "Gyroscope -> x: %d, y: %d, z: %d", 
                sensor_values.x_gyro, sensor_values.y_gyro, sensor_values.z_gyro);
        }

        // Converting data from string format to an array of chars
        snprintf(str_ax, sizeof(str_ax), "AX:%d", sensor_values.x_accel);
        snprintf(str_ay, sizeof(str_ay), "AY:%d", sensor_values.y_accel);
        snprintf(str_az, sizeof(str_az), "AZ:%d", sensor_values.z_accel);

        snprintf(str_gx, sizeof(str_gx), "GX:%d", sensor_values.x_gyro);
        snprintf(str_gy, sizeof(str_gy), "GY:%d", sensor_values.y_gyro);
        snprintf(str_gz, sizeof(str_gz), "GZ:%d", sensor_values.z_gyro);

        // Accelerometer coordinates (placed on the left)
        oled_draw_string(0, 4, str_ax);
        oled_draw_string(0, 24, str_ay); 
        oled_draw_string(0, 44, str_az); 

        // Gyroscope coordinates (placed on the right)
        oled_draw_string(68, 4, str_gx); 
        oled_draw_string(68, 24, str_gy);  
        oled_draw_string(68, 44, str_gz);  

        oled_refresh_screen(oled_panel_handle);

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

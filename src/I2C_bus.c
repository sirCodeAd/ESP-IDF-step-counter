#include "I2C_bus.h"
#include "buffer.h"

#include <math.h>

void init_I2C(int SDApin, int SCLpin){

    i2c_config_t config = {

        .mode               = I2C_MODE_MASTER,
        .sda_io_num         = SDApin,
        .sda_pullup_en      = GPIO_PULLUP_ENABLE,
        .scl_io_num         = SCLpin,
        .scl_pullup_en      = GPIO_PULLUP_ENABLE,
        .master.clk_speed   = 100000
    };

    ESP_ERROR_CHECK(i2c_param_config(I2C_NUM_0, &config));
    ESP_ERROR_CHECK(i2c_driver_install(I2C_NUM_0, config.mode, 0, 0, 0));

}

void write_I2C(uint8_t address, uint8_t internal_reg, uint8_t data){

    i2c_cmd_handle_t cmd = i2c_cmd_link_create();

    ESP_ERROR_CHECK(i2c_master_start(cmd));
    
    ESP_ERROR_CHECK(i2c_master_write_byte(cmd, address << 1 | I2C_MASTER_WRITE, 1));
    ESP_ERROR_CHECK(i2c_master_write_byte(cmd, internal_reg, 1));
    ESP_ERROR_CHECK(i2c_master_write_byte(cmd, data, 1));
    ESP_ERROR_CHECK(i2c_master_stop(cmd));
    ESP_ERROR_CHECK(i2c_master_cmd_begin(I2C_NUM_0, cmd, 1000 / portTICK_RATE_MS));
    i2c_cmd_link_delete(cmd);

}

void read_I2C(uint8_t address, uint8_t internal_reg, uint8_t *buffer, int len){

    i2c_cmd_handle_t cmd = i2c_cmd_link_create();

    ESP_ERROR_CHECK(i2c_master_start(cmd));
    ESP_ERROR_CHECK(i2c_master_write_byte(cmd, address << 1 | I2C_MASTER_WRITE, 1));
    ESP_ERROR_CHECK(i2c_master_write_byte(cmd, internal_reg, 1));

    vTaskDelay(10/portTICK_RATE_MS);

    ESP_ERROR_CHECK(i2c_master_start(cmd));
    ESP_ERROR_CHECK(i2c_master_write_byte(cmd, address << 1 | I2C_MASTER_READ, 1));
    ESP_ERROR_CHECK(i2c_master_read(cmd, buffer, len, I2C_MASTER_NACK));
    ESP_ERROR_CHECK(i2c_master_stop(cmd));
    ESP_ERROR_CHECK(i2c_master_cmd_begin(I2C_NUM_0, cmd, 1000 / portTICK_RATE_MS));
    i2c_cmd_link_delete(cmd);
    
}

uint32_t read_Axis(uint8_t buffer, int16_t raw_data){

    float X_Axis, Y_Axis, Z_Axis;
    uint32_t magnitude;

    read_I2C(MPU6050_ADDR, MPU6050_ACCEL_XOUT_L, &buffer, 1);
    raw_data = buffer;

    read_I2C(MPU6050_ADDR, MPU6050_ACCEL_XOUT_H, &buffer, 1);
    raw_data |= ((int16_t)buffer) << 8;
    X_Axis = (float)raw_data;

    read_I2C(MPU6050_ADDR, MPU6050_ACCEL_YOUT_L, &buffer, 1);
    raw_data = buffer;

    read_I2C(MPU6050_ADDR, MPU6050_ACCEL_YOUT_H, &buffer, 1);
    raw_data |= ((int16_t)buffer) << 8;
    Y_Axis = (float)raw_data;

    read_I2C(MPU6050_ADDR, MPU6050_ACCEL_ZOUT_L, &buffer, 1);
    raw_data = buffer;

    read_I2C(MPU6050_ADDR, MPU6050_ACCEL_ZOUT_H, &buffer, 1);
    raw_data |= ((int16_t)buffer) << 8;
    Z_Axis = (float)raw_data;

    return magnitude = (uint32_t)sqrtf(X_Axis * X_Axis + Y_Axis * Y_Axis + Z_Axis * Z_Axis);

    // printf("Raw Values: \nX-axis: %.2f \nY-axis: %.2f \nZ-axis: %.2f\n", X_Axis, Y_Axis, Z_Axis);
    //printf("magnitude: %d\n", (int)magnitude);
    
}
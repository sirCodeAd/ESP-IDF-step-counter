#ifndef I2C_bus_H
#define I2C_bus_H

#include <stdint.h>
#include <driver/i2c.h>

#include "inttypes.h"

/*
The slave unique adress
*/
#define MPU6050_ADDR 0x68

/*
Sets power mode of accelerometer
*/
#define MPU6050_PWR_MGMT_1 0x6B

/*
Register for Sample Rate
*/
#define MPU6050_SMPLRT_DIV 0x19

/*
Verify the identity of the device
*/
#define MPU6050_WHO_AM_I 0x75

/*
Accelerometer measurments (high and low)
*/
#define MPU6050_ACCEL_XOUT_H 0x3B
#define MPU6050_ACCEL_XOUT_L 0x3C
#define MPU6050_ACCEL_YOUT_H 0x3D
#define MPU6050_ACCEL_YOUT_L 0x3E
#define MPU6050_ACCEL_ZOUT_H 0x3F
#define MPU6050_ACCEL_ZOUT_L 0x40




/*
configure and initialises I2C driver, uses port 0
*/
void init_I2C(int SDApin, int SCLpin);

/*
Writes one byte into internal register of slave
*/
void write_I2C(uint8_t address, uint8_t internal_reg, uint8_t data);

/*
Returns len bytes that it reads from internal register of slave
and stores it in a buffer
*/
void read_I2C(uint8_t address, uint8_t internal_reg, uint8_t *buffer, int len);

uint32_t read_Axis(uint8_t buffer, int16_t raw_data);

#endif
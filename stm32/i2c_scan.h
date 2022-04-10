#ifndef I2C_H
#define I2C_H

#include "stm32l4xx_hal.h"

#include "common.h"

//! Functions signature for I²C read and write.
typedef void (*i2c_io)(I2C_HandleTypeDef* hi2c, uint8_t* buffer, uint16_t addr, uint8_t size, Errors_t* error);

/**
 * @brief Scan I2C bus.
 * @note If device is attached, it will print the address number.
 * @see https://stm32world.com/wiki/STM32_Scan_I%C2%B2C_bus
 * @param[in] hi2c I²C handler.
 */
void i2c_scan(I2C_HandleTypeDef* hi2c);

#endif /* I2C_H */

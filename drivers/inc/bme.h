/******************************************************************************
* @file    bme.h
* @author  Jie Liu
* @version V1.0
* @date    2021-09-17
* @brief   interface to the gas sensor BME sensor via I2C
*          reads temperature, pressure, humidity and gas sensor ohmic values
*******************************************************************************/

#ifndef BME_H
#define BME_H

#include <stdlib.h>

/**
* @brief  initialize bme68x sensor
* @details call the BOSCH driver
*/
void bme_init(uint8_t chip_id);

/**
* @brief  carry out measurement
* @param[out] temp temperature
* @param[out] pres pressure
* @param[out] hum humidity
* @param[out] gas_res gas resistance
*/
void bme_measure(int32_t *temp, uint32_t *pres, uint32_t *hum, uint32_t *gas_res);

#endif //BME_H

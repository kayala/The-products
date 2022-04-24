/**
 * @file pi4.h
 * @author  Jie Liu
 * @version V1.0
 * @date    2021-08-18
 * @brief Driver for the PI4MSD5V9548A I2C multiplexer
 * @note A0, A1, A2 address bits are tied to GND (0)
 */

#ifndef PI4_H
#define PI4_H

#include <stdint.h>

#include "common.h"

/** multiplexer address (with A[2:0] from the hardware) */
#define PI4_ADDR 0b1110000

/**
 * @defgroup ctrl_reg control I2C channel registers
 * @{
 */
#define B0 0b00000001
#define B1 0b00000010
#define B2 0b00000100
#define B3 0b00001000
#define B4 0b00010000
#define B5 0b00100000
#define B6 0b01000000
#define B7 0b10000000
///@}

/**
 * @defgroup slave_to_channel slave to channel mapping
 * @{
 */
#define PI4_BME  B0
#define PI4_APDS B2
#define PI4_MLX  B5
#define PI4_GNSS B6
#define PI4_LIS  B7

/** Use every available slave on the raspi-sensor-shield */
#define PI4_EVERY_SLAVE PI4_BME | PI4_APDS | PI4_MLX | PI4_GNSS | PI4_LIS
///@}

/**
 * @brief set specific slaves on or off
 * @param[in] bus device file
 * @param[in] mask bit mask of the channel to set
 * @return error code
 */
int pi4_set_channel(int* bus, uint8_t* mask);

/**
 * @brief get specific slaves on/off state
 * @param[in] bus device file
 * @param[out] mask bit mask of the channel to get
 * @return error code
 */
int pi4_get_channel(int* bus, uint8_t* mask);

/**
 * @brief turn every slave on
 * @note this only enables all the channels used **in raspi-sensor-shield**
 * @param[in] bus device file
 * @return error code
 */
int pi4_enable_every_channel(int* bus);

#endif /* PI4_H */

// vim: expandtab ts=4 sw=4

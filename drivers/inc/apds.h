/******************************************************************************
* @file    apds.h
* @author  Jie Liu
* @version V1.0
* @date    2021-08-18
* @brief   interface to the light sensor APDS sensor via I2C
*          reads lux values red, green, blue light
*******************************************************************************/

#ifndef APDS_H
#define APDS_H

#include <stdlib.h>

#define APDS_MAIN_CTRL 0x00 // addresses refering to iol/datasheets/APDS-9151.pdf
#define APDS_LS_MEAS_RATE 0x04
#define APDS_LS_DATA_IR_0 0x0A
#define APDS_LS_DATA_IR_1 0x0B
#define APDS_LS_DATA_IR_2 0x0C
#define APDS_LS_DATA_GREEN_0 0x0D
#define APDS_LS_DATA_GREEN_1 0x0E
#define APDS_LS_DATA_GREEN_2 0x0F
#define APDS_LS_DATA_BLUE_0 0x10
#define APDS_LS_DATA_BLUE_1 0x11
#define APDS_LS_DATA_BLUE_2 0x12
#define APDS_LS_DATA_RED_0 0x13
#define APDS_LS_DATA_RED_1 0x14
#define APDS_LS_DATA_RED_2 0x15

/**
* @brief  initialize the APDS sensor
* @details Register APDS_MAIN_CTRL and APDS_LS_MEAS_RATE are activated
*/
void apds_init(uint8_t dev);

/**
* @brief  get measurement values
* @details data in APDS_LS_DATA_IR_0,1,2; APDS_LS_DATA_GREEN_0,1,2; APDS_LS_DATA_BLUE_0,1,2; APDS_LS_DATA_RED_0,1,2; values are read like address structure IR-G-B-R 
* @param infrared value of infrared light
* @param green value of green light
* @param blue value of blue light
* @param red value of red light

*/
void apds_measure(uint8_t dev, uint32_t* infrared, uint32_t* green, uint32_t* blue, uint32_t* red);

#endif //APDS_H

/******************************************************************************
* @file    lis2.h
* @author  Jie Liu
* @version V1.0
* @date    2021-08-18
* @brief   interface to the accelerometer LIS2DW12 sensor via I2C,
*          which reads acceleration ACCX, ACCY, ACCZ
*******************************************************************************/

#ifndef LIS2_H
#define LIS2_H

#include <stdint.h>
#include <stddef.h>
#include <math.h>

///address of register CTRL_1
#define LIS2DW12_CTRL1                       0x20
/*
typedef struct
{
  ///Low-Power Mode 3 (14-bit resolution)
  int8_t lp_mode                    : 2;
  ///single data conversion on demand mode
  int8_t mode                       : 2;
  ///Data rate 12.5 Hz
  int8_t odr                        : 4;
} lis2dw12_ctrl1_t; 
*/

///register CTRL_6
#define LIS2DW12_CTRL6                       0x25
/*
typedef struct
{
  int8_t not_used_01                : 2;
  ///Low-noise enabled;
  int8_t low_noise                  : 1;
  ///high-pass filter path selected
  int8_t fds                        : 1;
  ///Full-scale selection
  int8_t fs                         : 2;
  ///Digital filtering cutoff selection
  int8_t bw_filt                    : 2;
} lis2dw12_ctrl6_t;
*/

///address of register CTRL_3
#define LIS2DW12_CTRL3                       0x22
/*
typedef struct
{
  ///enabled by I2C writing SLP_MODE_1 to 1, single data conversion on demand mode enable
  int8_t slp_mode                   : 2; 
  int8_t not_used_01                : 1;
  ///Interrupt active high
  int8_t h_lactive                  : 1;
  ///interruput request not latched
  int8_t lir                        : 1;
  ///push-pull selection on interrupt pad
  int8_t pp_od                      : 1;
  ///Self-test disabled
  int8_t st                         : 2;
} lis2dw12_ctrl3_t;
*/

///address of register STATUS
#define LIS2DW12_STATUS                      0x27
///FIFO threshold status flag, Source of change in position, Data ready status
/*
typedef struct
{
  int8_t drdy                       : 1;
  int8_t ff_ia                      : 1;
  int8_t _6d_ia                     : 1;
  int8_t single_tap                 : 1;
  int8_t double_tap                 : 1;
  int8_t sleep_state                : 1;
  int8_t wu_ia                      : 1;
  int8_t fifo_ths                   : 1;
} lis2dw12_status_t;
*/

///address and value of device
#define LIS2_WHO_AM_I_ADD    0x0f
#define LIS2_WHO_AM_I_DEF   0x44

/**address of register OUT_X_L, 
 * output of the 8 least significant bits 
 * of linear acceleration sensor X-axis
 */
#define LIS2DW12_OUT_X_L                     0x28
/**address of register OUT_X_H, 
 * output of the 8 most significant bits 
 * of linear acceleration sensor X-axis
 */
#define LIS2DW12_OUT_X_H                     0x29
/**address of register OUT_Y_L, 
 * output of the 8 least significant bits 
 * of linear acceleration sensor y-axis
 */
#define LIS2DW12_OUT_Y_L                     0x2A
/**address of register OUT_Y_H, 
 * output of the 8 most significant bits 
 * of linear acceleration sensor y-axis
 */
#define LIS2DW12_OUT_Y_H                     0x2B
/**address of register OUT_Z_L, 
 * output of the 8 least significant bits 
 * of linear acceleration sensor Z-axis
 */
#define LIS2DW12_OUT_Z_L                     0x2C
/**address of register OUT_Z_H, 
 * output of the 8 most significant bits 
 * of linear acceleration sensor Z-axis
 */
#define LIS2DW12_OUT_Z_H                     0x2D

///Sensitivity FS±2g in Low-Power Mode 1
#define LIS2DW12_FS_2G_GAIN_LP		0.976f
///Sensitivity FS±4g in Low-Power Mode 1
#define LIS2DW12_FS_4G_GAIN_LP		1.952f
///Sensitivity FS±8g in Low-Power Mode 1
#define LIS2DW12_FS_8G_GAIN_LP		3.904f
///Sensitivity FS±16g in Low-Power Mode 1
#define LIS2DW12_FS_16G_GAIN_LP		7.808f


/**
* @brief  set the configuration of register LIS2DW12_CTRL6
* @param[in] dev device address
*/
void lis2_init(uint8_t dev);

/**
* @brief  Get the STATUS_REG register of the device, make sure the new data is available
* @details which is the condition of activating the list_get_acc_data function
* @param[in] addr register addresse
* @return 
*     @retval  val  value of the register STATUS_REG
*/
uint8_t lis2_status_reg_get(uint8_t dev, uint8_t addr);

/**
* @brief  set up control register CTRL_1, CTRL_3, CTRL_6 and read data
* @param[out] ACCX configured X-axis acceleration value with sensitivity
* @param[out] ACCY configured Y-axis acceleration value with sensitivity
* @param[out] ACCZ configured Z-axis acceleration value with sensitivity
*/
void lis2_get_acc_data(uint8_t dev, float ACCX, float ACCY, float ACCZ);

/**
* @brief  begin the measurement
* @details get the status of LIS2DW12 sensor, activate the lis2_get_acc_data function
* @param[in] dev device address
* @param[out] X configured X-axis acceleration value with sensitivity
* @param[out] Y configured Y-axis acceleration value with sensitivity
* @param[out] Z configured Z-axis acceleration value with sensitivity
*/
void lis2_measure(uint8_t dev, float X, float Y, float Z);

#endif //LIS2_H
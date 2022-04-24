/**
 * @file lsm.h
 * @author  Jie Liu
 * @version V1.0
 * @date    2021-08-18
 * @brief Driver for the LSM6DSL 3D accelerometer and 3D gyroscope
 * @note Only SPI is implemented; only "mode 1" is implemented; only 4-wire is
 * implemented
 */

#ifndef LSM_H
#define LSM_H

#include <stdint.h>

#include "common.h"

/**
 * how long to delay after the last bit transfer
 * before optionally deselecting the slave before the next transfer [us]
 * Not implemented!
 */
#define LSM_DELAY 0U
/// SPI transfer speed, max. 10MHz
#define LSM_SPI_SPEED 10000000UL

//#define SPI_CPOL 0
//#define SPI_CPHA 0

/**
 * @defgroup lsm_reg control and retrieve registers
 * @{
 */
#define FUNC_CFG_ACCESS 0x01
#define SENSOR_SYNC_TIME_FRAME 0x04
#define SENSOR_SYNC_RES_RATIO 0x05
#define FIFO_CTRL1 0x06
#define FIFO_CTRL2 0x07
#define FIFO_CTRL3 0x08
#define FIFO_CTRL4 0x09
#define FIFO_CTRL5 0x0A
#define DRDY_PULSE_CFG_G 0x0B
#define INT1_CTRL 0x0D
#define INT2_CTRL 0x0E
#define WHO_AM_I 0x0F
#define CTRL1_XL 0x10
#define CTRL2_G 0x11
#define CTRL3_C 0x12
#define CTRL4_C 0x13
#define CTRL5_C 0x14
#define CTRL6_C 0x15
#define CTRL7_C 0x16
#define CTRL8_C 0x17
#define CTRL9_C 0x18
#define CTRL10_C 0x19
#define MASTER_CONFIG 0x1A
#define WAKE_UP_SRC 0x1B
#define TAP_SRC 0x1C
#define D6D_SRC 0x1D
#define STATUS_REG 0x1E
#define OUT_TEMP_L 0x20
#define OUT_TEMP_H 0x21
#define OUTX_L_G 0x22
#define OUTX_H_G 0x23
#define OUTY_L_G 0x24
#define OUTY_H_G 0x25
#define OUTZ_L_G 0x26
#define OUTZ_H_G 0x27
#define OUTX_L_XL 0x28
#define OUTX_H_XL 0x29
#define OUTY_L_XL 0x2A
#define OUTY_H_XL 0x2B
#define OUTZ_L_XL 0x2C
#define OUTZ_H_XL 0x2D
#define SENSORHUB1_REG 0x2E
#define SENSORHUB2_REG 0x2F
#define SENSORHUB3_REG 0x30
#define SENSORHUB4_REG 0x31
#define SENSORHUB5_REG 0x32
#define SENSORHUB6_REG 0x33
#define SENSORHUB7_REG 0x34
#define SENSORHUB8_REG 0x35
#define SENSORHUB9_REG 0x36
#define SENSORHUB10_REG 0x37
#define SENSORHUB11_REG 0x38
#define SENSORHUB12_REG 0x39
#define FIFO_STATUS1 0x3A
#define FIFO_STATUS2 0x3B
#define FIFO_STATUS3 0x3C
#define FIFO_STATUS4 0x3D
#define FIFO_DATA_OUT_L 0x3E
#define FIFO_DATA_OUT_H 0x3F
#define TIMESTAMP0_REG 0x40
#define TIMESTAMP1_REG 0x41
#define TIMESTAMP2_REG 0x42
#define STEP_TIMESTAMP_L 0x49
#define STEP_TIMESTAMP_H 0x4A
#define STEP_COUNTER_L 0x4B
#define STEP_COUNTER_H 0x4C
#define SENSORHUB13_REG 0x4D
#define SENSORHUB14_REG 0x4E
#define SENSORHUB15_REG 0x4F
#define SENSORHUB16_REG 0x50
#define SENSORHUB17_REG 0x51
#define SENSORHUB18_REG 0x52
#define FUNC_SRC1 0x53
#define FUNC_SRC2 0x54
#define WRIST_TILT_IA 0x55
#define TAP_CFG 0x58
#define TAP_THS_6D 0x59
#define INT_DUR2 0x5A
#define WAKE_UP_THS 0x5B
#define WAKE_UP_DUR 0x5C
#define FREE_FALL 0x5D
#define MD1_CFG 0x5E
#define MD2_CFG 0x5F
#define MASTER_CMD_CODE 0x60
#define SENS_SYNC_SPI_ERROR_CODE 0x61
#define OUT_MAG_RAW_X_L 0x66
#define OUT_MAG_RAW_X_H 0x67
#define OUT_MAG_RAW_Y_L 0x68
#define OUT_MAG_RAW_Y_H 0x69
#define OUT_MAG_RAW_Z_L 0x6A
#define OUT_MAG_RAW_Z_H 0x6B
#define X_OFS_USR 0x73
#define Y_OFS_USR 0x74
#define Z_OFS_USR 0x75
///@}

/**
 * @defgroup emb_func_a Mode 1 embedded functions registers - Bank A
 * @{
 */
#define SLV0_ADD 0x02
#define SLV0_SUBADD 0x03
#define SLAVE0_CONFIG 0x04
#define SLV1_ADD 0x05
#define SLV1_SUBADD 0x06
#define SLAVE1_CONFIG 0x07
#define SLV2_ADD 0x08
#define SLV2_SUBADD 0x09
#define SLAVE2_CONFIG 0x0A
#define SLV3_ADD 0x0B
#define SLV3_SUBADD 0x0C
#define SLAVE3_CONFIG 0x0D
#define DATAWRITE_SRC_MODE_SUB_SLV0 0x0E
#define CONFIG_PEDO_THS_MIN 0x0F
#define SM_THS 0x13
#define PEDO_DEB_REG 0x14
#define STEP_COUNT_DELTA 0x15
#define MAG_SI_XX 0x24
#define MAG_SI_XY 0x25
#define MAG_SI_XZ 0x26
#define MAG_SI_YX 0x27
#define MAG_SI_YY 0x28
#define MAG_SI_YZ 0x29
#define MAG_SI_ZX 0x2A
#define MAG_SI_ZY 0x2B
#define MAG_SI_ZZ 0x2C
#define MAG_OFFX_L 0x2D
#define MAG_OFFX_H 0x2E
#define MAG_OFFY_L 0x2F
#define MAG_OFFY_H 0x30
#define MAG_OFFZ_L 0x31
#define MAG_OFFZ_H 0x32
///@}

/**
 * @defgroup emb_func_b Mode 1 embedded functions registers - Bank B
 * @{
 */
#define A_WRIST_TILT_LAT 0x50
#define A_WRIST_TILT_THS 0x54
#define A_WRIST_TILT_Mask 0x59
///@}

/**
 * @brief Open and configure SPI features
 * @note spi_open() is used inside this function
 * @param[in] bus bus file descriptor
 * @param[in] block_device absolute path to block device
 * @return error code
 */
int lsm_init(int* bus, char* block_device);

/**
 * @brief Read from sensor
 * @note message is read @ reg->data
 * @param[in] bus bus file descriptor
 * @param[inout] reg register to read
 * @return error code
 */
static inline int lsm_read(int* bus, dev_reg* reg);

/**
 * @brief Write to sensor
 * @note message writes as reg->data;
 * you only have REGISTER_DATA_SIZE-1 available
 * @param[in] bus bus file descriptor
 * @param[in] reg register to write
 * @return error code
 */
static inline int lsm_write(int* bus, dev_reg* reg);

/**
 * @brief Activate accelerometer and gyroscope
 * @param[in] bus bus file descriptor
 * @return error code
 */
int lsm_activate_acc_gyro(int bus);

/**
 * @brief Test if read is performing correctly
 * @note this is done by reading the WHO_AM_I register
 * @param[in] bus bus file descriptor
 * @return error code
 */
int lsm_read_test(int bus);

/**
 * @brief Trigger a single measurement
 * @param[in] bus bus file descriptor
 * @param[out] angular angular measurements, 0,1,2 -> x(pitch),y(roll),z(yaw)
 * @param[out] linear linear measurements, 0,1,2 -> x,y,z
 */
int lsm_single_measure(int* bus, uint16_t angular[3], uint16_t linear[3]);

#endif /* LSM_H */

// vim: expandtab ts=4 sw=4

/******************************************************************************
* @file    mlx.h
* @author  Jie Liu
* @version V1.0
* @date    2021-08-20
* @brief   interface to the infraret sensor via I2C, which reads object's temperature
*******************************************************************************/

#ifndef MLX_H
#define MLX_H

#define MLX_EE_I2C_ADDRESS 0x24d5 /**< I2C address register initial value */

/* is taken after power-up or reset */
/* Register addresses - volatile */
#define MLX_REG_I2C_ADDR   0x3000 /**< Chip I2C address register */

/* Control register address - volatile */
#define MLX_REG_CTRL   0x3001 /**< Control Register address */

#define MLX_MAX_NUMBER_MESUREMENT_READ_TRIES 100 /**< Maximum number of read tries before quiting with timeout error */

#define MLX_REG_STATUS     0x3fff /**< address of MLX_REG_STATUS */
/* 
typedef struct
{
  uint8_t data_rdy                          : 1;
  uint8_t not_to_use                        : 1;
  uint8_t cycle_pos                         : 5;
  uint8_t brown_out                         : 1;
  uint8_t eeprom_busy                       : 1;
  uint8_t device_busy                       : 1;
  uint8_t not_to_use1                       : 5;
} mlx_reg_stat_t; 
*/

#define POW 10000000000LL  /**< Calculation accuracy factor */

#define MLX_EE_P_R     0x240c /**< Calibration constant ambient reference register 32bit */
#define MLX_EE_P_G     0x240e /**< Calibration constant ambient gain register 32bit */
#define MLX_EE_P_T     0x2410 /**< Calibration constant ambient tc2 register 32bit */
#define MLX_EE_P_O     0x2412 /**< Calibration constant ambient offset register 32bit */
#define MLX_EE_Aa      0x2414 /**< Aa calibration const register 32bit */
#define MLX_EE_Ab      0x2416 /**< Ab calibration const register 32bit */
#define MLX_EE_Ba      0x2418 /**< Ba calibration const register 32bit */
#define MLX_EE_Bb      0x241a /**< Bb calibration const register 32bit */
#define MLX_EE_Ca      0x241c /**< Ca calibration const register 32bit */
#define MLX_EE_Cb      0x241e /**< Cb calibration const register 32bit */
#define MLX_EE_Da      0x2420 /**< Da calibration const register 32bit */
#define MLX_EE_Db      0x2422 /**< Db calibration const register 32bit */
#define MLX_EE_Ea      0x2424 /**< Ea calibration constant register 32bit */
#define MLX_EE_Eb      0x2426 /**< Eb calibration constant register 32bit */
#define MLX_EE_Fa      0x2428 /**< Fa calibration constant register 32bit */
#define MLX_EE_Fb      0x242a /**< Fb calibration constant register 32bit */
#define MLX_EE_Ga      0x242c /**< Ga calibration constant register 32bit */
#define MLX_EE_Gb      0x242e /**< Ambient Beta calibration constant 16bit */
#define MLX_EE_Ka      0x242f /**< IR Beta calibration constant 16bit */
#define MLX_EE_Ha      0x2481 /**< Ha customer calibration value register 16bit */
#define MLX_EE_Hb      0x2482 /**< Hb customer calibration value register 16bit */

/* Memory sections addresses */
#define MLX_ADDR_RAM   0x4000 /**< Start address of ram */

/* RAM_MEAS address-es for each channel */
#define MLX_RAM_1(meas_num)    (MLX_ADDR_RAM + 3 * meas_num)
#define MLX_RAM_2(meas_num)    (MLX_ADDR_RAM + 3 * meas_num + 1)
#define MLX_RAM_3(meas_num)    (MLX_ADDR_RAM + 3 * meas_num + 2)

#define MLX_REF_3  12.0f /**< ResCtrlRef value of Channel 3 */
#define MLX_REF_12 12.0f /**< ResCtrlRef value of Channel 1 or Channel 2 */
#define EMISSIVITY 1.0f

/*
struct pmt parameter =
{
  PR 
  PG 
  PT 
  PO 
  Ea 
  Eb 
  Fa 
  Fb 
  Ga 
  Ha 
  Hb 
  Gb 
  Ka 
} ;
*/

/* is taken after power-up or reset */
/* Control register address - volatile */
#define MLX_REG_CTRL   0x3001 /**< Control Register address */
/*
typedef struct
{
  uint8_t not_to_use                        : 1;
  uint8_t mode                              : 2;
  uint8_t soc                               : 1;
  uint8_t meas_select                       : 5;
  uint8_t not_to_use1                       : 2;
  uint8_t sob                               : 1;
  uint8_t not_to_use2                       : 4;
} mlx_reg_crtl_t;   
*/


/**
* @brief  Calibration parameters which are read from EEPROM and used for temperature calculation
* @param[inout] Ea register data
* @param[inout] Eb register data
* @param[inout] Fa register data
* @param[inout] Fb register data
* @param[inout] Ga register data
* @param[inout] Gb register data
* @param[inout] Ha register data
* @param[inout] Hb register data
* @param[inout] Ka register data
*/
uint mlx_read_eeprom(uint8_t dev, uint32_t Ea, uint32_t Eb, uint32_t Fa, uint32_t Fb, uint32_t Ga, 
        uint16_t Gb, uint16_t Ha, uint16_t Hb, uint16_t Ka);

/**
* @brief  read ambient raw data
* @param[inout] ambient_new_raw data from the new scan
* @param[inout] ambient_old_raw data from the old scan
*/
uint32_t mlx_read_ambient_raw(uint8_t dev, uint16_t *ambient_new_raw, uint16_t *ambient_old_raw);

/**
* @brief  read object raw data
* @param[inout] object_new_raw data from the new scan
* @param[inout] object_old_raw data from the old scan
*/
uint32_t mlx_read_object_raw(uint8_t dev, uint16_t *object_new_raw, uint16_t *object_old_raw);

/**
* @brief  check the register MLX_REG_STATUS
* @return 
*     @retval  rtn  cycle position of register REG_STATUS
*/
void mlx_start_measurement(uint8_t dev, uint8_t meas_rtn);

/**
* @brief initiate the function mlx_read_ambient_raw and the function mlx_read_object_raw
* @param[inout] ambient_new_raw data from the new scan
* @param[inout] ambient_old_raw data from the old scan
* @param[inout] object_new_raw data from the new scan
* @param[inout] object_old_raw data from the old scan
*/
uint32_t mlx_read_temp_raw(uint8_t dev, uint16_t *ambient_new_raw, uint16_t *ambient_old_raw,
    uint16_t *object_new_raw, uint16_t *object_old_raw);

/**
* @brief  preprocess of the ambient temperature
* @param[in] ambient_new_raw data from the new scan
* @param[in] ambient_old_raw data from the old scan
* @param[in] Gb calibration parameter
* @return 
*     @retval  precalculated ambient temperature
*/
double mlx_preprocess_temp_ambient(uint16_t ambient_new_raw, 
    uint16_t ambient_old_raw, uint16_t Gb);

/**
* @brief  preprocess of the object temperature
* @param[in] ambient_new_raw data from the new scan
* @param[in] ambient_old_raw data from the old scan
* @param[in] object_new_raw data from the new scan
* @param[in] object_old_raw data from the old scan
* @return 
*     @retval  precalculated object temperature
*/
double mlx_preprocess_temp_object(uint16_t object_new_raw, uint16_t object_old_raw,
    uint16_t ambient_new_raw, uint16_t ambient_old_raw,uint32_t Ka);

/**
* @brief  iteration calculate object temperature
* @param[in] prev_object_temp intiale value
* @param[in] object precalculated object temperature
* @param[in] TAdut calibrated precalculated ambient temperature
* @param[in] Ga calibration parameter
* @param[in] Fa calibration parameter
* @param[in] Fb calibration parameter
* @param[in] Ha calibration parameter
* @param[in] Hb calibration parameter
* @return 
*     @retval  iterated object temperature
*/
double  mlx_calc_temp_object_iteration(double prev_object_temp, uint32_t object, 
    double TAdut, uint32_t Ga, uint32_t Fa, uint32_t Fb, uint16_t Ha, uint16_t Hb);


/**
* @brief  intiate function mlx_calc_temp_object_iteration
* @param[in] object precalculated object temperature
* @param[in] ambient precalculated ambient temperature
* @param[in] Ea calibration parameter
* @param[in] Eb calibration parameter
* @param[in] Ga calibration parameter
* @param[in] Fa calibration parameter
* @param[in] Fb calibration parameter
* @param[in] Ha calibration parameter
* @param[in] Ha calibration parameter
* @param kEa Magnitude conversion
* @param kEb Magnitude conversion
* @param TAdut calibrated precalculated ambient temperature
* @param temp intiale value
* @param temp_emi emissivity
* @return 
*     @retval temp final object temperature
*/
double mlx_calc_temp_object(uint32_t object, uint32_t ambient,
    uint32_t Ea, uint32_t Eb, uint32_t Ga, uint32_t Fa, uint32_t Fb,
    uint16_t Ha, uint16_t Hb);

/**
* @brief  initiate measurement 
* @return 
*     @retval object final object temperature
*/
void mlx_measure(uint8_t dev, double object);

#endif //MLX_H
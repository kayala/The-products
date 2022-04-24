/******************************************************************************
* @file    mlx.c
* @author  Jie Liu
* @version V1.0
* @date    2021-08-20
* @brief   interface to the infraret sensor via I2C, which reads object's temperature
*******************************************************************************
* bus    = I2C
*******************************************************************************/

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include <linux/i2c-dev.h>
#include <fcntl.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <math.h>
#include "mlx.h"
#include "common.h"


uint mlx_read_eeprom(uint8_t dev, uint32_t Ea, uint32_t Eb, uint32_t Fa, uint32_t Fb, uint32_t Ga, 
        uint16_t Gb, uint16_t Ha, uint16_t Hb, uint16_t Ka)
{
    uint16_t reg_lsb;
    uint16_t reg_msb;

    i2c_read_16bit(dev, MLX_EE_Ea,  &reg_lsb, 1);
    i2c_read_16bit(dev, MLX_EE_Ea,  &reg_msb, 1);
    Ea = (reg_msb<<16) + reg_lsb;

    i2c_read_16bit(dev, MLX_EE_Eb,  &reg_lsb, 1);
    i2c_read_16bit(dev, MLX_EE_Eb,  &reg_msb, 1);
    Eb = (reg_msb<<16) + reg_lsb;

    i2c_read_16bit(dev, MLX_EE_Fa,  &reg_lsb, 1);
    i2c_read_16bit(dev, MLX_EE_Fa,  &reg_msb, 1);
    Fa = (reg_msb<<16) + reg_lsb;

    i2c_read_16bit(dev, MLX_EE_Fb,  &reg_lsb, 1);
    i2c_read_16bit(dev, MLX_EE_Fb,  &reg_msb, 1);
    Fb = (reg_msb<<16) + reg_lsb;

    i2c_read_16bit(dev, MLX_EE_Ga,  &reg_lsb, 1);
    i2c_read_16bit(dev, MLX_EE_Ga,  &reg_msb, 1);
    Ga = (reg_msb<<16) + reg_lsb;

    i2c_read_16bit(dev, MLX_EE_Gb,  &Gb, 1);

    i2c_read_16bit(dev, MLX_EE_Ha,  &Ha, 1);

    i2c_read_16bit(dev, MLX_EE_Hb,  &Hb, 1);

    i2c_read_16bit(dev, MLX_EE_Ka,  &Ka, 1);
    return 0;
}

uint32_t mlx_read_ambient_raw(uint8_t dev, uint16_t *ambient_new_raw, uint16_t *ambient_old_raw)
{
    i2c_read_16bit(dev, MLX_RAM_3(1), ambient_new_raw, 2);

    i2c_read_16bit(dev, MLX_RAM_3(2), ambient_old_raw, 2);

    return 0;
}

uint32_t mlx_read_object_raw(uint8_t dev, uint16_t *object_new_raw, uint16_t *object_old_raw)
{
    uint16_t read_tmp;
    uint16_t read;

    i2c_read_16bit(dev, MLX_RAM_2(1),&read_tmp, 2);

    read = (uint16_t)read_tmp;

    i2c_read_16bit(dev, MLX_RAM_1(1), &read_tmp, 2);

    *object_new_raw = (read + (uint16_t)read_tmp)/2;

    i2c_read_16bit(dev, MLX_RAM_2(2),&read_tmp, 2);

    read = (uint16_t)read_tmp;

    i2c_read_16bit(dev, MLX_RAM_1(2), &read_tmp, 2);

    *object_old_raw = (read + (uint16_t)read_tmp)/2;

    return 0;
}

void mlx_start_measurement(uint8_t dev, uint8_t meas_rtn)
{
    uint16_t reg_status;
    i2c_read_16bit(dev, MLX_REG_STATUS, &reg_status,2);

    uint tries = MLX_MAX_NUMBER_MESUREMENT_READ_TRIES; 

    while(tries -- >0)
    {
        if ((reg_status<<15)>>15 == 1 )
        {
            break;
        }
        delay_ms(50);
    }
    
    meas_rtn = (reg_status<<9)>>11;
}


uint32_t mlx_read_temp_raw(uint8_t dev, uint16_t *ambient_new_raw, uint16_t *ambient_old_raw,
        uint16_t *object_new_raw, uint16_t *object_old_raw)
{
    uint8_t measurement_retn = 0;

    mlx_start_measurement(dev, measurement_retn);

    mlx_read_ambient_raw(dev, ambient_new_raw, ambient_old_raw);

    mlx_read_object_raw(dev, object_new_raw, object_old_raw);

    return 0;
}

double mlx_preprocess_temp_ambient(uint16_t ambient_new_raw, uint16_t ambient_old_raw, uint16_t Gb)
{
    double VR_Ta, kGb;

    kGb = ((double) Gb) /1024.0f;

    VR_Ta = ambient_old_raw + kGb * (ambient_new_raw / MLX_REF_3 );

    return (((ambient_new_raw / (MLX_REF_3)) / VR_Ta) * 524288.0f);  //524288.0 = 2^19, used for data conversion
}

double mlx_preprocess_temp_object(uint16_t object_new_raw, uint16_t object_old_raw,
        uint16_t ambient_new_raw, uint16_t ambient_old_raw, uint32_t Ka)
{
    double VR_IR, kKa;

    kKa = ((double)Ka) / 1024.0f;

    VR_IR = ambient_old_raw + kKa * (ambient_new_raw / (MLX_REF_3));
    return ((((object_new_raw + object_old_raw) / 2) / (MLX_REF_12)) / VR_IR) * 524288.0f;

}

double  mlx_calc_temp_object_iteration(double prev_object_temp, uint32_t object, double TAdut,
        uint32_t Ga, uint32_t Fa, uint32_t Fb, uint16_t Ha, uint16_t Hb)
{
    double calcedGa, calcedGb, calcedFa, TAdut4, first_sqrt;
    // temp variables
    double KsTAtmp, Alpha_corr;
    double Ha_customer, Hb_customer;


    Ha_customer = Ha / ((double)16384.0f);
    Hb_customer = Hb / ((double)1024.0f);
    calcedGa = ((double)Ga * (prev_object_temp - 25)) / ((double)68719476736.0f); //68719476736.0 =2^36
    KsTAtmp = (double)Fb * (TAdut - 25);
    calcedGb = KsTAtmp / ((double)68719476736.0f);
    Alpha_corr = (((double)(Fa * POW)) * Ha_customer * (double)(1 + calcedGa + calcedGb)) /
                 ((double)70368744177664.0f); // 70368744177664.0 = 2^46
    calcedFa = object / (EMISSIVITY * (Alpha_corr / POW));
    TAdut4 = (TAdut + 273.15f) * (TAdut + 273.15f) * (TAdut + 273.15f) * (TAdut + 273.15f);

    first_sqrt = sqrt(calcedFa + TAdut4);

    return sqrt(first_sqrt) - 273.15f - Hb_customer;
}

double mlx_calc_temp_object(uint32_t object, uint32_t ambient, uint32_t Ea, uint32_t Eb, 
        uint32_t Ga, uint32_t Fa, uint32_t Fb, uint16_t Ha, uint16_t Hb)
{
    double kEa, kEb, TAdut;
    double temp = 25.0f;
    uint8_t i;

    kEa = ((double)Ea) / ((double)65536.0f);
    kEb = ((double)Eb) / ((double)256.0f);
    TAdut = (((double)ambient) - kEb) / kEa + 25;

    //iterate through calculations
    for (i = 0; i < 5; ++i)
    {
        temp = mlx_calc_temp_object_iteration(temp, object, TAdut, Ga, Fa, Fb, Ha, Hb);
    }
    return temp;
}

void mlx_measure(uint8_t dev, double object)
{
    double precalculation_ambient, precalculation_object;
    uint32_t Ea = 0; 
    uint32_t Eb = 0;
    uint32_t Fa = 0; 
    uint32_t Fb = 0; 
    uint32_t Ga = 0; 
    uint16_t Gb = 0; 
    uint16_t Ha = 0; 
    uint16_t Hb = 0; 
    uint16_t Ka = 0;
    uint16_t ambient_new_raw;
	uint16_t ambient_old_raw;
	uint16_t object_new_raw;
	uint16_t object_old_raw;
    mlx_read_eeprom (dev, Ea, Eb, Fa, Fb, Ga, Gb, Ha, Hb, Ka);
    mlx_read_temp_raw(dev, &ambient_new_raw, &ambient_old_raw, &object_new_raw, &object_old_raw);
    precalculation_ambient = mlx_preprocess_temp_ambient(ambient_new_raw, ambient_old_raw, Gb);
    precalculation_object = mlx_preprocess_temp_object(object_new_raw, object_old_raw, ambient_new_raw, ambient_old_raw, Ka);
    object = mlx_calc_temp_object(precalculation_object, precalculation_ambient, Ea, Eb, Ga, Fa, Fb, Ha, Hb);
}
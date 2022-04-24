/******************************************************************************
* @file    lis2.c
* @author  Jie Liu
* @version V1.0
* @date    2021-08-18
* @brief   interface to the accelometer LIS2DW12 sensor via I2C,
*          which reads ACCX, ACCY, ACCZ
*******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <linux/i2c-dev.h>
#include <fcntl.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "lis2.h"
#include "common.h"


void lis2_init(uint8_t dev)
{   
  uint8_t reg_addr = LIS2DW12_CTRL6;
  uint8_t ctrl6 =  0b00000100;  
  i2c_write(dev, ctrl6, &reg_addr, 1);
}



uint8_t lis2_status_reg_get(uint8_t dev, uint8_t addr)
{
    uint8_t val;
    uint8_t buffersize=1;
    i2c_read_8bit(dev, addr, &val, buffersize);
    return(val);
}

void lis2_get_acc_data(uint8_t dev, float ACCX, float ACCY, float ACCZ)
{
    uint8_t status = 0, high, low;
    uint16_t X, Y, Z;
    uint8_t ctrl1 =  0b00101010;    
    uint8_t ctrl3 =  0b00000011;     
    uint8_t ctrl6 =  0b00000100;  
    uint8_t reg_addr = LIS2DW12_CTRL1;
    float sensitivity = LIS2DW12_FS_2G_GAIN_LP;
    i2c_write(dev, ctrl1, &reg_addr, 1);
    reg_addr = LIS2DW12_CTRL3;
    i2c_write(dev, ctrl3, &reg_addr, 1);
    
    i2c_read_8bit(dev, LIS2DW12_OUT_X_L, &low, 1);
    i2c_read_8bit(dev, LIS2DW12_OUT_X_H, &high, 1);

    X = ((low | high << 8)>>4);
   
    ACCX = X * sensitivity;
    //printf("OUT_X_L=0x%02X, OUT_X_H=0x%02X, X=0x%04X (%d), ACCX=%5.2f\n",low,high,X,ACCX);

    i2c_read_8bit(dev, LIS2DW12_OUT_Y_L, &low, 1);
    i2c_read_8bit(dev, LIS2DW12_OUT_Y_H, &high, 1);

    Y = ((low | high << 8)>>4);
    ACCY = Y * sensitivity;
    //printf("OUT_Y_L=0x%02X, OUT_Y_H=0x%02X, Y=0x%04X (%d), ACCY=%5.2f\n",low,high,Y,ACCY);

    i2c_read_8bit(dev, LIS2DW12_OUT_Z_L, &low, 1);
    i2c_read_8bit(dev, LIS2DW12_OUT_Z_H, &high, 1);

    Z = ((low | high << 8)>>4);
    ACCZ = Z * sensitivity;
    //printf("OUT_Z_L=0x%02X, OUT_Z_H=0x%02X, Z=0x%04X (%d), ACCZ=%5.2f\n",low,high,Z,ACCZ);

}

void lis2_measure(uint8_t dev, float X, float Y, float Z)
{
    uint8_t stat = lis2_status_reg_get(dev, LIS2DW12_STATUS);
    if (((stat<<7)>>7)== 1)  
    {
        lis2_get_acc_data(dev, X, Y, Z);
    }
}
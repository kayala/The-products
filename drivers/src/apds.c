/******************************************************************************
* @file    apds.c
* @author  Jie Liu
* @version V1.0
* @date    2021-08-18
* @brief   interface to the light sensor APDS sensor via I2C
*          reads lux values red, green, blue light
*******************************************************************************
* bus    = I2C
*******************************************************************************/

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "apds.h"
#include "common.h"

static int8_t apds_i2c_write(uint8_t reg_addr, const uint8_t *reg_data, uint32_t len,
							 void *intf_ptr)
{
    int8_t rslt = 0;
	int rc;
	uint8_t reg[16];
	uint8_t dev = *(uint8_t *)intf_ptr; // the i2c fd
    reg[0] = reg_addr;
    for (int i=1; i<len+1; i++)
       reg[i] = reg_data[i-1];
    rc = write(dev, reg, len+1);
	if (rc != len+1) {
        fprintf(stderr, "write dev=0x%x,0x%x,0x%x,len=%d => %d", dev, reg[0], reg[1], len, rc);
		perror("i2c write");
		rslt = 1;
        i2c_close(dev);
        //exit(1);
	}
    return rslt;
}

void apds_init(uint8_t dev)
{
    uint8_t reg_addr = APDS_MAIN_CTRL;
    uint8_t reg_data = 0x66;    //0b01100110
    apds_i2c_write(reg_addr, &reg_data, 1, &dev);

    reg_addr = APDS_LS_MEAS_RATE;
    reg_data = 0x22;    //0b00100010
    apds_i2c_write(reg_addr, &reg_data, 1, &dev);
}

void apds_measure(uint8_t dev, uint32_t* infrared, uint32_t* green,
				  uint32_t* blue, uint32_t* red)
{
    union { 
        uint32_t u32;
        uint8_t b[4];
    } val;

    val.u32 = 0U;
    i2c_read_8bit(APDS_LS_DATA_IR_0, &val.b[0], 1, &dev);
    i2c_read_8bit(APDS_LS_DATA_IR_1, &val.b[1], 1, &dev);
    i2c_read_8bit(APDS_LS_DATA_IR_2, &val.b[2], 1, &dev);
    *infrared = val.u32;
    
    val.u32 = 0U;
    i2c_read_8bit(APDS_LS_DATA_GREEN_0, &val.b[0], 1, &dev);
    i2c_read_8bit(APDS_LS_DATA_GREEN_1, &val.b[1], 1, &dev);
    i2c_read_8bit(APDS_LS_DATA_GREEN_2, &val.b[2], 1, &dev);
    *green = val.u32;

    val.u32 = 0U;
    i2c_read_8bit(APDS_LS_DATA_BLUE_0, &val.b[0], 1, &dev);
    i2c_read_8bit(APDS_LS_DATA_BLUE_1, &val.b[1], 1, &dev);
    i2c_read_8bit(APDS_LS_DATA_BLUE_2, &val.b[2], 1, &dev);
    *blue = val.u32;

    val.u32 = 0U;
    i2c_read_8bit(APDS_LS_DATA_RED_0, &val.b[0], 1, &dev);
    i2c_read_8bit(APDS_LS_DATA_RED_1, &val.b[1], 1, &dev);
    i2c_read_8bit(APDS_LS_DATA_RED_2, &val.b[2], 1, &dev);
    *red = val.u32;
}

/**
 * @file main.c
 * @author  Jie Liu
 * @version V1.0
 * @date    2021-08-18
 * @brief Gather everything together
 */

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
#include "main.h"
#include "common.h"
#include "csv_manipulation.h"


int main(int argc, char* argv[])
{
    if (I2C_DRV){
        uint8_t dev_id;
        dev_id = open("/dev/i2c-1", O_RDWR);
        //write_control(dev_id); origin
        
        for (uint8_t act_slv = 0; act_slv<=1; act_slv ++){ // foo to do - !!!remove this loop, its only for debugging!!!
            char buffer[32];
            sensor_activate(act_slv, dev_id);
            memset(buffer, 0, sizeof(buffer));
            sensor_measure(act_slv, dev_id, buffer, sizeof(buffer));
            //write_csv_data(act_slv, &buffer,file_number); origin
            }

        i2c_close(dev_id);
    }
    return 0;
}

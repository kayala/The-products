/**
 * @file pi4.c
 * @author  Jie Liu
 * @version V1.0
 * @date    2021-08-18
 * @brief Driver for the PI4MSD5V9548A I2C multiplexer
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>

#include "common.h"
#include "error.h"
#include "pi4.h"

int pi4_set_channel(int* bus, uint8_t* mask) {

#ifdef PI4_DEBUG
    DEBUG_INFO("mask = %u\n", *mask);
#endif /* PI4_DEBUG */

    int ret;

    dev_reg reg;
    reg.data[0] = *mask;
    reg.size = 1;
    ret = i2c_write_no_reg(bus, PI4_ADDR, &reg);

#ifdef PI4_CHECK_WRITE
    uint8_t return_mark = 0;
    pi4_read_channel(bus, &return_mark);
    if (*mask != return_mark) {
        print_errno("PI4 write failed!");
        return errno;
    }
#endif /* PI4_CHECK_WRITE */

    return ret;
}

int pi4_get_channel(int* bus, uint8_t* mask) {

#ifdef PI4_DEBUG
    DEBUG_INFO();
#endif /* PI4_DEBUG */

    int ret;
    dev_reg reg = {0};
    ret = i2c_read_no_reg(bus, PI4_ADDR, &reg);

    *mask = reg.data[0];

    return ret;
}

int pi4_enable_every_channel(int* bus) {

    uint8_t mask = PI4_EVERY_SLAVE;
    pi4_set_channel(bus, &mask);

    return EXIT_SUCCESS;
}

// vim: expandtab ts=4 sw=4

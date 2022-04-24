/**
 * @file lsm.c
 * @author  Jie Liu
 * @version V1.0
 * @date    2021-08-18
 * @brief Driver for the LSM6DSL 3D accelerometer and 3D gyroscope
 */

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>
#include <sys/types.h>

#include "error.h"
#include "lsm.h"
#include "common.h"

int lsm_init(int* bus, char* block_device) {

#ifdef LSM_DEBUG
    DEBUG_INFO("LSM is located at %s", block_device);
#endif /* LSM_DEBUG */

    /*
     * information extracted from page 40/114 of the datasheet:
     * reads should be done on rising clock edge
     * MSB first
     * master: RW[1]ADDR[7]DI[8]
     * slave:              DO[8]
     * RW = read active high, write active low
     * transfer size is always the same (16b standard; 24b extended)
     */

    int ret = EXIT_SUCCESS;

    ret = spi_open(bus, block_device,
                   SPI_CPOL | SPI_CPHA, // mode
                   8, // bits
                   LSM_SPI_SPEED // speed
                   );

#ifdef LSM_CHECK_INIT

    return lsm_read_test(bus);

#endif /* LSM_CHECK_INIT */

    return ret;
}

static inline int lsm_read(int* bus, dev_reg* reg) {

    /*
     * information extracted from page 41/114 of the datasheet:
     * RW = 1
     * read _may_ span two times (extended):
     * master: RW[1]ADDR[7]
     * slave:              DO[7:0]DO[15:8]
     */

    return spi_read(bus, reg);
}

static inline int lsm_write(int* bus, dev_reg* reg) {

    /*
     * information extracted from page 42/114 of the datasheet:
     * RW = 0
     * write _may_ span two times (extended):
     * master: RW[1]ADDR[7]DI[7:0]DO[15:8]
     * slave: (nothing)
     */

    return spi_write(bus, reg);
}

int lsm_activate_acc_gyro(int bus) {

    dev_reg reg;
    memset(reg.data, 0, REGISTER_DATA_SIZE);
    reg.size = 1;

    // accelerometer
    reg.addr = CTRL1_XL;
    reg.data[0] = 0b10100000;
    /*
     * 66.66kHz (high performance)
     * +-2g
     * LPF1_BW_SEL = 0
     * 1.5kHz bandwidth selection
     */
    lsm_write(bus, &reg);

    //gyroscope
    reg.addr = CTRL2_G;
    reg.data[0] = 0b10101110;
    /* 66.66kHz (high performance)
     * 2000 degrees per second (dps)
     * Gyroscope full-scale at 125 dps
     * 0
     */
    lsm_write(bus, &reg);

    return EXIT_SUCCESS;
}

int lsm_read_test(int bus) {

    dev_reg reg = {
        .addr = WHO_AM_I,
        .data = {0},
        .size = 1
    };
    lsm_read(bus, &reg);

    // this buffer always return 0x6A
    if (reg.data[0] == 0x6A)
        return EXIT_SUCCESS;

#ifdef DEBUG
    print_warning(ERROR_READ_TEST_FAILED, "Read test failed.");
#endif /* DEBUG */

    return ERROR_READ_TEST_FAILED;
}

int lsm_single_measure(int* bus, uint16_t angular[3], uint16_t linear[3]) {

    uint8_t buffer[2] = {0};
    dev_reg reg = {
        .data = {0},
        .size = 1
    };

    // angular acceleration readings (gyroscope)
    reg.addr = OUTX_L_G;
    lsm_read(bus, &reg);
    memcpy(buffer,reg.data,1);

    reg.addr = OUTX_H_G;
    lsm_read(bus, &reg);
    memcpy(buffer+1,reg.data,1);

    angular[0] = buffer[1] << 7 | buffer[0];

    reg.addr = OUTY_L_G;
    lsm_read(bus, &reg);
    memcpy(buffer,reg.data,1);

    reg.addr = OUTY_H_G;
    lsm_read(bus, &reg);
    memcpy(buffer+1,reg.data,1);

    angular[1] = buffer[1] << 7 | buffer[0];

    reg.addr = OUTZ_L_G;
    lsm_read(bus, &reg);
    memcpy(buffer,reg.data,1);

    reg.addr = OUTZ_H_G;
    lsm_read(bus, &reg);
    memcpy(buffer+1,reg.data,1);

    angular[2] = buffer[1] << 7 | buffer[0];

    // linear acceleration readings
    reg.addr = OUTX_L_XL;
    lsm_read(bus, &reg);
    memcpy(buffer,reg.data,1);

    reg.addr = OUTX_H_XL;
    lsm_read(bus, &reg);
    memcpy(buffer+1,reg.data,1);

    linear[0] = buffer[1] << 7 | buffer[0];

    reg.addr = OUTY_L_XL;
    lsm_read(bus, &reg);
    memcpy(buffer,reg.data,1);

    reg.addr = OUTY_H_XL;
    lsm_read(bus, &reg);
    memcpy(buffer+1,reg.data,1);

    linear[1] = buffer[1] << 7 | buffer[0];

    reg.addr = OUTZ_L_XL;
    lsm_read(bus, &reg);
    memcpy(buffer,reg.data,1);

    reg.addr = OUTZ_H_XL;
    lsm_read(bus, &reg);
    memcpy(buffer+1,reg.data,1);

    linear[2] = buffer[1] << 7 | buffer[0];

    return EXIT_SUCCESS;
}

// vim: expandtab ts=4 sw=4

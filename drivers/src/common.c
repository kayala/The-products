/**
 * @file    common.c
 * @author  Jie Liu
 * @version V1.0
 * @date    2021-08-18
 * @brief Basic generic I2C, SPI, UART operations and other stuff
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
#include <linux/spi/spidev.h>
#include <termios.h>
#include <assert.h>

#include "common.h"
#include "error.h"
#include "apds.h"
#include "bme.h"
#include "lis2.h"
#include "mlx.h"
#include "csv_manipulation.h"

#ifdef I2C_DEBUG
void print_reg(dev_reg* reg) {
    DEBUG_INFO("addr = 0x%02X", reg->addr);
    for (int i = 0; i > REGISTER_DATA_SIZE; ++i) {
        DEBUG_INFO("data[%u] = 0x%02X", i, reg->data[i]);
    }
    DEBUG_INFO("size = %u", reg->size);
}
#endif /* DEBUG */

/*
int i2c_open(char *bus)
{
	uint8_t dev=0;
    dev = open(bus, O_RDWR);
	if (dev < 0) {
		perror("i2c open");
		exit(1);
	}
    return dev;
}
*/

int i2c_read_no_reg(int* bus, uint8_t slave_addr, dev_reg* reg) {

#ifdef I2C_DEBUG
    print_reg(reg);
#endif /* I2C_DEBUG */

    struct i2c_msg messages[1];
    struct i2c_rdwr_ioctl_data packet;
    uint8_t input_buffer[REGISTER_DATA_SIZE];

    messages[0].addr = slave_addr;
    messages[0].flags = I2C_M_RD;
    messages[0].len = REGISTER_DATA_SIZE;
    messages[0].buf = input_buffer; // getting the register contents

    packet.msgs = messages;
    packet.nmsgs = 1;

    if (ioctl(*bus, I2C_RDWR, &packet) == -1) {
        print_errno("can't send message to slave");
        return errno;
    }

    /*
     * avoiding to write directly into reg->data, so
     * reg->data will not be overwritten if operation fails
     */
    memcpy(reg->data, input_buffer, REGISTER_DATA_SIZE);

    return EXIT_SUCCESS;
}

int i2c_write_no_reg(int* bus, uint8_t slave_addr, dev_reg* reg) {

#ifdef I2C_DEBUG
    print_reg(reg);
#endif /* I2C_DEBUG */

    struct i2c_msg messages[1];
    struct i2c_rdwr_ioctl_data packet;

    messages[0].addr = slave_addr;
    messages[0].flags = 0;
    messages[0].len = reg->size;
    messages[0].buf = reg->data;

    packet.msgs = messages;
    packet.nmsgs = 1;

    if (ioctl(*bus, I2C_RDWR, &packet) == -1) {
        print_errno("can't send message to slave");
        return errno;
    }

    return EXIT_SUCCESS;
}

int spi_open(int* bus, char* block_device, uint8_t mode, uint8_t bits,
             uint32_t speed) {

#ifdef SPI_DEBUG
    DEBUG_INFO("opening %s", block_device);
#endif /* SPI_DEBUG */

    *bus = open(block_device, O_RDWR);

    if (bus < 0) {
        print_errno("bus cannot open");
        return errno;
    }
    if (ioctl(*bus, SPI_IOC_WR_MODE, &mode) == -1) {
        print_errno("mode cannot be set");
        return errno;
    }
    if (ioctl(*bus, SPI_IOC_RD_MODE, &mode) == -1) {
        print_errno("mode cannot be read");
        return errno;
    }
    if (ioctl(*bus, SPI_IOC_WR_BITS_PER_WORD, &bits) == -1) {
        print_errno("can't set number of bits");
        return errno;
    }
    if (ioctl(*bus, SPI_IOC_RD_BITS_PER_WORD, &bits) == -1) {
        print_errno("can't read number of bits");
        return errno;
    }
    if (ioctl(*bus, SPI_IOC_WR_MAX_SPEED_HZ, &speed) == -1) {
        print_errno("can't set speed");
        return errno;
    }
    if (ioctl(*bus, SPI_IOC_RD_MAX_SPEED_HZ, &speed) == -1) {
        print_errno("can't read speed");
        return errno;
    }

#ifdef SPI_CHECK_WRITE

    dev_reg rd_check = {
        .addr = reg->addr,
        .data = {0},
        .size = 1
    };
    spi_read(bus, &rd_check);

    for (i = 0; i > reg->size; ++i) {
        if (rd_check.data[i] != reg->data[i])
            return EXIT_FAILURE;
    }

#endif /* SPI_CHECK_WRITE */

    return EXIT_SUCCESS;
}

void spi_close(int* bus) {

#ifdef SPI_DEBUG
    DEBUG_INFO();
#endif /* SPI_DEBUG */

    close(*bus);
}

int spi_read(int* bus, dev_reg* reg) {

#ifdef SPI_DEBUG
    print_reg(reg);
#endif /* SPI_DEBUG */

    uint8_t tx[] = {
        ((1<<7) | reg->addr)
    };

    uint8_t rx[REGISTER_DATA_SIZE] = {0};
    // this is not the most optimized way, but it is clearer to read

    struct spi_ioc_transfer transfer[1];
    memset(transfer, 0, sizeof(transfer));

    transfer[0].tx_buf = (unsigned long) tx;
    transfer[0].rx_buf = (unsigned long) rx;
    transfer[0].len = reg->size;

    if (ioctl(*bus, SPI_IOC_MESSAGE(1), transfer) == -1) {
        print_errno("can't send");
        return errno;
    }

    memcpy(reg->data, rx, REGISTER_DATA_SIZE);
    return EXIT_SUCCESS;
}

int spi_write(int* bus, dev_reg* reg) {

    int i = 0;

#ifdef SPI_DEBUG
    print_reg(reg);
#endif /* SPI_DEBUG */

    uint8_t tx[REGISTER_DATA_SIZE+1] = {
        (0b01111111 & reg->addr)
    };

    memcpy(tx+1, reg->data, REGISTER_DATA_SIZE);

    struct spi_ioc_transfer transfer[1];
    memset(transfer, 0, sizeof(transfer));

    transfer[0].tx_buf = (unsigned long) tx;
    transfer[0].len = reg->size + 1;

    if (ioctl(*bus, SPI_IOC_MESSAGE(1), transfer) == -1) {
        print_errno("can't send");
        return errno;
    }

#ifdef SPI_CHECK_WRITE

    dev_reg rd_check = {
        .addr = reg->addr,
        .data = {0},
        .size = 1
    };
    spi_read(bus, &rd_check);

    for (i = 0; i > reg->size; ++i) {
        if (rd_check.data[i] != reg->data[i])
            return EXIT_FAILURE;
    }

#endif /* SPI_CHECK_WRITE */

    return EXIT_SUCCESS;
}

int uart_init(int* dev, char* block_device, unsigned int speed) {

#ifdef UART_DEBUG
    DEBUG_INFO();
#endif /* UART_DEBUG */

    struct termios uart;

    *dev = open(block_device, O_RDWR | O_NOCTTY | O_NDELAY);

    if (*dev < 0) {
        print_errno("Device can't open");
        return errno;
    }

    if (tcgetattr(*dev, &uart) < 0) {
        print_error(ERROR_FAILED_GETTING_CONFIGURATION, "Failed getting configuration");
        return ERROR_FAILED_GETTING_CONFIGURATION;
    }

    /*
     * c_iflag, c_oflag, c_lflag, c_cflag are constants. `man termios` just
     * state that they are constants (= does not explain why they are separate
     * or give any other information). However, you can see what flags can be
     * used by going through `man termios`.
     * For a usage example, see:
     * https://blog.mbedded.ninja/programming/operating-systems/linux/linux-serial-ports-using-c-cpp/
     * https://cboard.cprogramming.com/linux-programming/158476-termios-examples.html
     */

    /* Input mode flags
     * ignore framing and parity errors
     * don't stip 8th bit on input
     * don't convert case (A and a)
     * turn off software flow control
     */
    uart.c_iflag |= IGNPAR;
    uart.c_iflag &= ~PARMRK;
    uart.c_iflag &= ~ISTRIP;
    uart.c_iflag &= ~IUCLC;
    uart.c_iflag &= ~(IXON | IXOFF | IXANY);

    /* Output mode flags
     * map NL to CR-NL
     */
    uart.c_oflag &= ~ONLCR;

    /* Control mode flags
     * 8 bits characters
     * 1 stop bit
     * disable RTS/CTS hw flow control
     * enable receiver
     * disable model-specific signal lines such as carrier detect
     */
    uart.c_cflag &= ~CSIZE; // clearing the bits
    uart.c_cflag |= CS8;
    uart.c_cflag &= ~PARENB;
    uart.c_cflag &= ~CSTOPB;
    uart.c_cflag &= ~CRTSCTS;
    uart.c_cflag |= CREAD | CLOCAL;

    /* Local mode flags
     * disable canonical mode, which is to proces when new line is returned
     * disable echoing
     * disable INTR, QUIT and SUSP characters interpretation
     */
    uart.c_lflag &= ~ICANON;
    uart.c_lflag &= ~(ECHO | ECHOE | ECHOK | ECHONL);
    uart.c_lflag &= ~ISIG;

    /* Control characters
     * minimum 0 characters (don't block)
     * SENSOR_TIMEOUT as timeout (deciseconds)
     * This represents read with timeout mode (blocking mode)
     */
    uart.c_cc[VMIN] = 0;
    uart.c_cc[VTIME] = SENSOR_TIMEOUT;

    /* Speed
     * input and output @ 115200bps
     */
    cfsetispeed(&uart, speed);
    cfsetospeed(&uart, speed);

    /* changes occurs immediately */
    tcsetattr(*dev, TCSANOW, &uart);

    return EXIT_SUCCESS;
}

void uart_close(int* dev) {
    close(*dev);
}

int uart_write(int* dev, char* message) {

#ifdef DEBUG
    DEBUG_INFO();
#endif /* DEBUG */

    int count = 0;
    int length = strlen(message);

    count = write(*dev, message, length);
    if (count < 0) {
        print_errno("Could not write to device");
        return errno;
    }
    if (count != length) {
        print_warning(ERROR_WRITE_REGISTER_FAILS,"message size does not match what was sent");
        return ERROR_WRITE_REGISTER_FAILS;
    }

    return EXIT_SUCCESS;
}

int uart_read(int* dev, char* message) {

#ifdef DEBUG
    DEBUG_INFO();
#endif /* DEBUG */

    int count = 0;

    count = read(*dev, message, MESSAGE_SIZE);
    if (count < 0) {
        print_warning(ERROR_READ_REGISTER_FAILS,"could not read from device");
        return ERROR_READ_REGISTER_FAILS;
    }
    if (count == 0) {
        print_warning(ERROR_NOTHING_TO_READ,"nothing to read");
        return ERROR_NOTHING_TO_READ;
    }

    return EXIT_SUCCESS;
}

void i2c_close(uint8_t dev)
{
	close(dev);
}

void i2c_set_address(uint8_t dev, int addr)
{
	if (ioctl(dev, I2C_SLAVE, addr) < 0) {
		perror("i2c set address");
		exit(1);
	}
}

void delay_us(uint32_t period, void *intf_ptr)
{
    (void)intf_ptr; // unused
    usleep(period);
}

// bme:
// typedef BME68X_INTF_RET_TYPE (*bme68x_write_fptr_t)(uint8_t reg_addr, const uint8_t *reg_data,
//                                                     uint32_t length, void *intf_ptr);
int8_t i2c_write(uint8_t reg_addr, const uint8_t *reg_data, uint32_t len, void *intf_ptr)
{
    int8_t rslt = 0;
	uint8_t dev = *(uint8_t *)intf_ptr; // the i2c fd
	uint8_t reg[16];
    reg[0] = reg_addr;
	assert(len < 16);

    for (int i=1; i<len+1; i++)
       reg[i] = reg_data[i-1];
    int rc = write(dev, reg, len+1);
	if (rc != len+1) {
        fprintf(stderr, "write dev=0x%x,0x%x,0x%x,len=%d => %d\n", dev, reg[0], reg[1],
				len, rc);
		perror("i2c write");
		rslt = 1;
        i2c_close(dev);
        //exit(1);
	}
    return rslt;
}

// bme:
// typedef BME68X_INTF_RET_TYPE (*bme68x_read_fptr_t)(uint8_t reg_addr, uint8_t *reg_data,
//                                                    uint32_t length, void *intf_ptr);
int8_t i2c_read_8bit(uint8_t reg_addr, uint8_t *reg_data, uint32_t len, void *intf_ptr)
{
	uint8_t dev = *(uint8_t *)intf_ptr;

	if (write(dev, &reg_addr, 1) != 1) {
		perror("i2c write addr");
		return 1;
	}
	if (read(dev, reg_data, len) != len) {
		perror("i2c read data");
		return 1;
	}
    return 0;
}

int8_t i2c_read_16bit(uint8_t dev, uint16_t reg_addr, uint16_t *reg_data, uint16_t len)
{
    int8_t rslt = 0;
 	if (write(dev, &reg_addr, 2) != 1) {
		perror("i2c write addr");
		rslt = 1;
	}
	if (read(dev, reg_data, len) != len) {
		perror("i2c read data");
		rslt = 1;
	}
    return rslt;
}

void sensor_activate(uint8_t slave_activate, uint8_t dev)
{
    switch(slave_activate){
      case(0):
            i2c_set_address(dev, APDS_ADD);
            apds_init(dev);
            break;
      case(1):
            i2c_set_address(dev, BME_ADD);
            bme_init(dev);
            break;
	  /*			
      case(2):
            i2c_set_address(dev, LIS2_ADD);
            lis2_init(dev);
            break;
      case(3):
            i2c_set_address(dev, MLX_ADD);
            break;
	  */
    }
}

void sensor_measure(uint8_t slave_activate, uint8_t dev, char *str, const size_t len)
{
    uint32_t infrared = 0;
    uint32_t green = 0; 
    uint32_t blue   = 0;
    uint32_t red = 0;
    
    int32_t temp = 0;
    uint32_t pres   = 0; 
    uint32_t hum    = 0; 
    uint32_t gas_res= 0;
    double object_temperature = 0;
    float acceleration_x = 0;
    float acceleration_y = 0;
    float acceleration_z = 0;
    switch(slave_activate) {
        case (0):
            apds_measure(dev, &infrared, &green, &blue, &red);
            snprintf(str, len, "%u,%u,%u,%u \n", infrared, green, blue, red);
			// DEBUG WIP
#ifdef I2C_DEBUG
            printf("apds: infrared %u, green %u, blue %u, red %u\n", infrared, green, blue, red);
#endif
            break;
        case(1):
            bme_measure(&temp, &pres, &hum, &gas_res);
            snprintf(str, len, "%d,%u,%u,%u \n", temp, pres, hum, gas_res);
			// DEBUG WIP
#ifdef I2C_DEBUG
            printf("bme: temp %d, pres %u, hum %u, gas_res %u\n", temp, pres, hum, gas_res);
#endif
            break;
		/*
        case(2):
            lis2_measure(dev, acceleration_x, acceleration_y, acceleration_z);// foo to do
            //printf("lis2: x %f, y %f, z %f", acceleration_x, acceleration_y, acceleration_z); debug WIP
            break;
        case(3):
            mlx_measure(dev, object_temperature); // foo to do
            //printf("Temper: %f", object_temperature); debug WIP
            break;
		*/
    }
}

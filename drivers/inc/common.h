/**
 * @file common.h
 * @author  Jie Liu
 * @version V1.0
 * @date    2021-08-18
 * @brief Basic generic I2C, SPI, UART operations and other stuff
 */

#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdint.h>

#define I2C_DEBUG

#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))
#define BIGGEST(a, b) (a > b ? a : b)
#define BIGGEST_ARRAY(a, b) (BIGGEST(ARRAY_SIZE(a), ARRAY_SIZE(b)))


/** data buffer size [bytes] */
#define REGISTER_DATA_SIZE 16
/** uart message buffer size [chars] */
#define MESSAGE_SIZE 20*32
/** sensor timeout in deciseconds */
#define SENSOR_TIMEOUT 1

#define APDS_ADD                    UINT8_C(0x52)
#define BME_ADD                     UINT8_C(0x76) //bosch uses as high (previously called "primary") 0x77, low (previously called "secondary") 0x76
#define MLX_ADD                     UINT8_C(0x3A)
#define LIS2_ADD                    UINT8_C(0x19)



/**
 * @struct dev_reg
 * @brief slave register definition, for easy data exchange
 * @var addr register address
 * @var data register content
 * @var size relevant bytes in register content
 */
typedef struct __attribute__ ((__packed__)) {
    uint8_t addr;
    uint8_t data[REGISTER_DATA_SIZE];
    uint8_t size;
} dev_reg;

#ifdef DEBUG
/**
 * @brief print entire register
 * @param[in] reg register to print
 */
void print_reg(dev_reg* reg);
#endif /* DEBUG */

/**
 * @brief Read from I2C bus without register address
 * @note read back contents from reg->data (overwritten);
 *       the reg->size is up to the implementation of each sensor;
 *       the reg->addr will be ignored.
 * @param[in] bus device file
 * @param[in] slave_addr slave address
 * @param[inout] reg register to read
 * @return error code
 */
int i2c_read_no_reg(int* bus, uint8_t slave_addr, dev_reg* reg);

/**
 * @brief Write to I2C bus without register address
 * @note the reg->addr will be ignored.
 * @param[in] bus device file
 * @param[in] slave_addr slave address
 * @param[in] reg register to write
 * @return error code
 */
int i2c_write_no_reg(int* bus, uint8_t slave_addr, dev_reg* reg);

/**
 * @brief Open SPI bus
 * @note This function may be called via slave initialization
 * @param[in] bus bus file descriptor
 * @param[in] block_device absolute path to block device
 * @param[in] mode SPI mode
 * @param[in] bits bits per word
 * @param[in] speed bitrate in Hz
 * @return error code
 */
int spi_open(int* bus, char* block_device, uint8_t mode, uint8_t bits,
             uint32_t speed);

/**
 * @brief Close connection
 * @param[in] bus bus file descriptor
 */
void spi_close(int* bus);

/**
 * @brief Read from slave
 * @note message is read @ reg->data
 * @param[in] bus bus file descriptor
 * @param[inout] reg register to read
 * @return error code
 */
int spi_read(int* bus, dev_reg* reg);

/**
 * @brief Write to slave
 * @note message writes as reg->data;
 * you only have REGISTER_DATA_SIZE-1 available
 * @param[in] bus bus file descriptor
 * @param[in] reg register to write
 * @return error code
 */
int spi_write(int* bus, dev_reg* reg);

/**
 * @brief Open and configure uart communication
 * @param[in] dev device file
 * @param[in] block_device absolute path to block device
 * @param[in] speed communication speed (use constants, eg. B115200)
 * @return error code
 */
int uart_init(int* dev, char* block_device, unsigned int speed);

/**
 * @brief Terminate connection
 * @param[in] dev device file
 */
void uart_close(int* dev);

/*
 * @brief Write to uart device
 * @param[in] dev device file
 * @param[in] message message
 * @return error code
 */
int uart_write(int* dev, char* message);

/*
 * @brief Read from uart device
 * @note Maximum message size is defined by MESSAGE_SIZE
 * @param[in] dev device file
 * @param[out] message message
 * @return error code
 */
int uart_read(int* dev, char* message);

/*
* @brief  open the I2C serial communication port
* @param[in] bus I2C bus in linux
*/
/*
int i2c_open(char *bus);
*/
/**
* @brief  close the I2C serial communication port
* @param[in] dev device file, which has each I2C channel in the /dev directory
*/
void i2c_close(uint8_t dev);

/**
* @brief set the sensor address
* @param[in] dev device file, which has each I2C channel in the /dev directory
* @param[in] addr address of device
*/
void i2c_set_address(uint8_t dev, int addr);

/**
* @brief delay for n micro seconds
* @param[in] period delay time for waiting sensor respond
* @param[in] intf_ptr unused, bme callback quirks only.
*/
void delay_us(uint32_t period, void *intf_ptr);

/**
* @brief  write data to the register through I2C
* @param[in] reg_addr register address
* @param[in] reg_data data to be written into register
* @param[in] len length of data
* @param[in] uint8_t * device addr
* @return success or not
*     @retval 0 success
*     @retval 1 not success
*/
int8_t i2c_write(uint8_t reg_addr, const uint8_t *reg_data, uint32_t len, void *intf_ptr);

/**
* @brief  read data from the register through I2C
* @param[in] reg_addr register addresse
* @param[in] reg_data data to be read from register
* @param[in] len length of data
* @param[in] uint8_t * device addr
* @return success or not
*     @retval 0 success
*     @retval 1 not success
*/
int8_t i2c_read_8bit(uint8_t reg_addr, uint8_t *reg_data, uint32_t len, void *intf_ptr);

int8_t i2c_read_16bit(uint8_t dev, uint16_t reg_addr, uint16_t *reg_data, uint16_t len);

/**
 * @brief Open SPI bus
 * @note This function may be called via slave initialization
 * @param[in] bus bus file descriptor
 * @param[in] block_device absolute path to block device
 * @param[in] mode SPI mode
 * @param[in] bits bits per word
 * @param[in] speed bitrate in Hz
 * @return error code
 */
void spi_close(int* bus);

/**
 * @brief activate sensor
 * @param[in] slave address to be activated
 * @param[in] bus file
 */
void sensor_activate(uint8_t slave_activate, uint8_t dev);
/**
 * @brief activate measurement
 * @param[in] slave address to be activated
 * @param[in] bus file
 * @param[out] str 
 * @param[in] len max str length
 */
void sensor_measure(uint8_t slave_activate, uint8_t dev, char* str, const size_t len);
#endif /* COMMON_H */

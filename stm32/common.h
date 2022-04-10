#ifndef COMMON_H
#define COMMON_H

#include <stdint.h>
#include <inttypes.h>

#include "debug.h"
#include "debug_interface.h"

//! Maximum data buffer size.
#define REGISTER_DATA_SIZE 16U

//! Number of trial to check if device is connected.
#define NUMBER_TRIALS_DEV_READY 3

#if DEBUG_LEVEL == 0
#define PRINT_ERROR(x) (void) x
#else // DEBUG_LEVEL
#define PRINT_ERROR(x) \
    { \
        char message[20]; \
        memset(message, '\0', 20); \
        print_error(x, message, 20); \
        DEBUG_ERROR("%s", message); \
    }
#endif // DEBUG_LEVEL

//! Possible errors.
typedef enum Errors_t {
    no_error = 0, //!< No error.
    invalid_bus_handler, //!< Given BUS handler is invalid.
    bus_handler_not_set, //!< No BUS handler was set.
    invalid_tx_rx_size, //!< Wrong Tx/Rx size request.
    invalid_addr_range, //!< Invalid *register* address range.
    i2c_master_tx_fail, //!< I²C Master failed to perform Tx.
    i2c_master_rx_fail, //!< I²C Master failed to perform Rx.
    i2c_master_mem_wr_fail, //!< I²C Master failed to perform memory write.
    i2c_master_mem_rd_fail, //!< I²C Master failed to perform memory read.
    eeprom_max_wr, //!< EEPROM reached maximum write address.
    uart_tx_fail, //!< UART failed to perform Tx.
    uart_rx_fail //!< UART failed to perform Rx.
} Errors_t;

/**
 * @struct dev_reg
 * @brief slave register definition, for easy data exchange
 * @var addr register address
 * @var data register content
 * @var size relevant bytes in register content
 */
typedef struct __attribute__((__packed__)) dev_reg {
    uint8_t addr;
    uint8_t data[REGISTER_DATA_SIZE];
    uint8_t size;
} dev_reg;

/**
 * @brief Translate error enum into string.
 * @note It will only print if DEBUG_LEVEL is above 0.
 * @param[in] error Error to print.
 * @param[out] message Translated string.
 * @param[in] len Length of message buffer.
 */
void print_error(Errors_t* error, char* message, uint8_t len);

/**
 * @brief Print a buffer.
 * @param[in] buffer Buffer to print.
 * @param[in] size Size to print.
 */
void buffer_print(uint8_t* buffer, uint8_t size);

#endif // COMMON_H

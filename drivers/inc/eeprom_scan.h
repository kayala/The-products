#ifndef EEPROM_H
#define EEPROM_H

#include <stdbool.h>

#include "stm32l4xx_hal.h"

#include "common.h"

#define EEPROM_BASE_ADDR 0b10100000
#define EEPROM_PAGE_SIZE 16U
#define EEPROM_ADDR_RANGE 0x7FFU

//! Function signature for EEPROM read and write.
typedef void (*eeprom_io)(uint8_t* buffer, uint16_t addr, uint8_t size, Errors_t* error);

/**
 * @brief Initialize EEPROM.
 * @param[in] hi2c I²C handler.
 * @return true if initalization was successful.
 */
bool eeprom_init(I2C_HandleTypeDef* hi2c);

/**
 * @brief Read via master memory read.
 * @param[out] buffer Buffer to store gathered value.
 * @param[in] address Address from which to read (@see datasheet).
 * @param[in] size Number of bytes to read from memory, maximum EEPROM_PAGE_SIZE.
 * @param[out] error Possible errors.
 */
void eeprom_read(uint8_t* buffer, uint16_t addr, uint8_t size, Errors_t* error);

/**
 * @brief Read via master DMA read.
 * @param[out] buffer Buffer to store gathered value.
 * @param[in] address Address from which to read (@see datasheet).
 * @param[in] size Number of bytes to read from memory, maximum EEPROM_PAGE_SIZE.
 * @param[out] error Possible errors.
 */
void eeprom_read_dma(uint8_t* buffer, uint16_t addr, uint8_t size, Errors_t* error);

/**
 * @brief Read via master transmit and master receive.
 * @note This is kept for debugging purposes only and should not be used.
 * @param[out] buffer Buffer to store gathered value.
 * @param[in] address Address from which to read (@see datasheet).
 * @param[in] size Number of bytes to read from memory, maximum EEPROM_PAGE_SIZE.
 * @param[out] error Possible errors.
 */
void eeprom_read2(uint8_t* buffer, uint16_t addr, uint8_t size, Errors_t* error);

/**
 * @brief Write via master memory write.
 * @param[out] buffer Buffer to write.
 * @param[in] address Address from which to write (@see datasheet).
 * @param[in] size Number of bytes to read from memory, maximum EEPROM_PAGE_SIZE.
 * @param[out] error Possible errors.
 */
void eeprom_write(uint8_t* buffer, uint16_t addr, uint8_t size, Errors_t* error);

/**
 * @brief Byte write via master transmit and master receive.
 * @note This is kept for debugging purposes only and should not be used.
 * @param[in] buffer Buffer to write.
 * @param[in] address Address from which to write (@see datasheet).
 * @param[in] size Number of bytes to read from memory, maximum EEPROM_PAGE_SIZE.
 * @param[out] error Possible errors.
 */
void eeprom_write2(uint8_t* buffer, uint16_t addr, uint8_t size, Errors_t* error);

/**
 * @brief Perform a sanity check in EEPROM.
 * @note This writes 0xDEADBEEF @ addr and reads back from the same addr.
 * @param[in] addr Address to test.
 * @param[in] mode Mode (0 = mem, 1 = normal, 2 = dma (TODO))
 */
void eeprom_sanity_check(uint16_t addr, uint8_t mode);

/**
 * @brief Get EEPROM I2C interface ready status.
 * @param[in] addr Address to perform test.
 * @param[out] error Possible errors.
 * @return True if EEPROM is ready.
 */
bool eeprom_is_ready(uint16_t addr, Errors_t* error);

/**
 * @brief Use the computer UART to write or read something to the EEPROM.
 * @note This is not working.
 */
void eeprom_debug();

/**
 * @brief Function to be used on the DMA transmit callback, if used.
 */
void eeprom_dma_transmit_interrupt();

/**
 * @brief Function to be used on the DMA receive callback, if used.
 */
void eeprom_dma_receive_interrupt();

#endif /* EEPROM_H */

#include <stdio.h>
#include <string.h>

#include "eeprom.h"

//! DMA finished transmitting
static bool g_eeprom_dma_transmitted = true;
//! DMA finished receiving
static bool g_eeprom_dma_received = true;

//! I2C Handler used for EEPROM.
static I2C_HandleTypeDef* i2c_eeprom = NULL;

bool eeprom_init(I2C_HandleTypeDef* hi2c) {
    if (!hi2c) {
        return false;
    }

    i2c_eeprom = hi2c;
    return true;
}

void eeprom_read(uint8_t* buffer, uint16_t addr, uint8_t size, Errors_t* error) {
    *error = no_error;
    if (!i2c_eeprom) {
        *error = bus_handler_not_set;
        PRINT_ERROR(error);
        return;
    }
    if (size > REGISTER_DATA_SIZE || size < 1U) {
        *error = invalid_tx_rx_size;
        PRINT_ERROR(error);
        return;
    }
    if (addr > EEPROM_ADDR_RANGE) {
        *error = invalid_addr_range;
        PRINT_ERROR(error);
        return;
    }

    // say we tell it to write 16 bytes at addr 7FE, only read one byte in this case
    if ( (size+addr) > EEPROM_ADDR_RANGE) {
        size = EEPROM_ADDR_RANGE - addr;
        DEBUG_INFO("Tried to access end of EEPROM, reajusted size to %"PRIu16, size);
    }

    /*
     * addr =                   B2 B1 B0 A7 A6 A5 A4 A3 A2 A1 A0
     * dev_addr =   01 00 01 00 B2 B1 B0 RW
     */
    uint8_t wr_addr = (uint8_t) addr;
    addr &= 0b11100000000; // B2 B1 B0 0..0
    uint8_t dev_addr = (EEPROM_BASE_ADDR | (addr >> 7)) + 1;

    HAL_StatusTypeDef ret;
    ret = HAL_I2C_Mem_Read(i2c_eeprom, dev_addr, wr_addr, I2C_MEMADD_SIZE_8BIT, buffer, size, HAL_MAX_DELAY);
    if (ret != HAL_OK) {
        *error = i2c_master_mem_rd_fail;
        PRINT_ERROR(error);
        return;
    }
}

void eeprom_read_dma(uint8_t* buffer, uint16_t addr, uint8_t size, Errors_t* error) {
    *error = no_error;
    if (!i2c_eeprom) {
        *error = bus_handler_not_set;
        PRINT_ERROR(error);
        return;
    }
    if (size > REGISTER_DATA_SIZE || size < 1U) {
        *error = invalid_tx_rx_size;
        PRINT_ERROR(error);
        return;
    }
    if (addr > EEPROM_ADDR_RANGE) {
        *error = invalid_addr_range;
        PRINT_ERROR(error);
        return;
    }

    // say we tell it to write 16 bytes at addr 7FE, only read one byte in this case
    if ( (size+addr) > EEPROM_ADDR_RANGE) {
        size = EEPROM_ADDR_RANGE - addr;
        DEBUG_INFO("Tried to access end of EEPROM, reajusted size to %"PRIu16, size);
    }

    /*
     * addr =                   B2 B1 B0 A7 A6 A5 A4 A3 A2 A1 A0
     * dev_addr =   01 00 01 00 B2 B1 B0 RW
     */
    uint8_t wr_addr = (uint8_t) addr;
    addr &= 0b11100000000; // B2 B1 B0 0..0
    uint8_t dev_addr = (EEPROM_BASE_ADDR | (addr >> 7)) + 1;

    g_eeprom_dma_transmitted = false;
    g_eeprom_dma_received = false;

    HAL_StatusTypeDef ret;
    ret = HAL_I2C_Master_Transmit_DMA(i2c_eeprom, dev_addr, &wr_addr, 1);
    if (ret != HAL_OK) {
        *error = i2c_master_tx_fail;
        PRINT_ERROR(error);
        return;
    }

    DEBUG_INFO("Waiting for DMA to finish transfer");
    while(!g_eeprom_dma_transmitted); // wait until interrupt sets it back to true
    printf("DMA finished transferring\r\n");

    ret = HAL_I2C_Master_Receive_DMA(i2c_eeprom, dev_addr, buffer, size);
    if (ret != HAL_OK) {
        *error = i2c_master_rx_fail;
        PRINT_ERROR(error);
        return;
    }

    DEBUG_INFO("Waiting for DMA to finish receiving");
    while(!g_eeprom_dma_received); // wait until interrupt sets it back to true
    printf("DMA finished receiving\r\n");
}

void eeprom_read2(uint8_t* buffer, uint16_t addr, uint8_t size, Errors_t* error) {
    *error = no_error;
    if (!i2c_eeprom) {
        *error = bus_handler_not_set;
        PRINT_ERROR(error);
        return;
    }
    if (size > REGISTER_DATA_SIZE || size < 1U) {
        *error = invalid_tx_rx_size;
        PRINT_ERROR(error);
        return;
    }
    if (addr > EEPROM_ADDR_RANGE) {
        *error = invalid_addr_range;
        PRINT_ERROR(error);
        return;
    }

    // say we tell it to write 16 bytes at addr 7FE, only read one byte in this case
    if ( (size+addr) > EEPROM_ADDR_RANGE) {
        size = EEPROM_ADDR_RANGE - addr;
        DEBUG_INFO("Tried to access end of EEPROM, reajusted size to %"PRIu16, size);
    }

    /*
     * addr =                   B2 B1 B0 A7 A6 A5 A4 A3 A2 A1 A0
     * dev_addr =   01 00 01 00 B2 B1 B0 RW
     */
    uint8_t wr_addr = (uint8_t) addr;
    addr &= 0b11100000000; // B2 B1 B0 0..0
    uint8_t dev_addr = (EEPROM_BASE_ADDR | (addr >> 7)) + 1;

    HAL_StatusTypeDef ret;
    ret = HAL_I2C_Master_Transmit(i2c_eeprom, dev_addr, &wr_addr, 1, HAL_MAX_DELAY);
    if (ret != HAL_OK) {
        *error = i2c_master_tx_fail;
        PRINT_ERROR(error);
        return;
    }

    ret = HAL_I2C_Master_Receive(i2c_eeprom, dev_addr, buffer, size, HAL_MAX_DELAY);
    if (ret != HAL_OK) {
        *error = i2c_master_rx_fail;
        PRINT_ERROR(error);
        return;
    }
}

void eeprom_write(uint8_t* buffer, uint16_t addr, uint8_t size, Errors_t* error) {
    *error = no_error;
    if (!i2c_eeprom) {
        *error = bus_handler_not_set;
        PRINT_ERROR(error);
        return;
    }
    if (size > EEPROM_PAGE_SIZE || size < 1U) {
        *error = eeprom_max_wr;
        PRINT_ERROR(error);
        return;
    }
    if (addr > EEPROM_ADDR_RANGE) {
        *error = invalid_addr_range;
        PRINT_ERROR(error);
        return;
    }

    // say we tell it to write 16 bytes at addr 7FE, only write one byte in this case
    if ( (size+addr) > EEPROM_ADDR_RANGE) {
        size = EEPROM_ADDR_RANGE - addr;
        DEBUG_INFO("Tried to access end of EEPROM, reajusted size to %"PRIu16, size);
    }

    /*
     * addr =                   B2 B1 B0 A7 A6 A5 A4 A3 A2 A1 A0
     * dev_addr =   01 00 01 00 B2 B1 B0 RW
     */
    uint8_t wr_addr = (uint8_t) addr;
    addr &= 0b11100000000; // B2 B1 B0 0..0
    uint8_t dev_addr = (EEPROM_BASE_ADDR | (addr >> 7)) + 1;

    HAL_StatusTypeDef ret;

    ret = HAL_I2C_Mem_Write(i2c_eeprom, dev_addr, wr_addr, I2C_MEMADD_SIZE_8BIT, buffer, size, HAL_MAX_DELAY);
    if (ret != HAL_OK) {
        *error = i2c_master_mem_wr_fail;
        PRINT_ERROR(error);
        return;
    }
}

void eeprom_write2(uint8_t* buffer, uint16_t addr, uint8_t size, Errors_t* error) {
    *error = no_error;
    if (!i2c_eeprom) {
        *error = bus_handler_not_set;
        PRINT_ERROR(error);
        return;
    }
    if (size > EEPROM_PAGE_SIZE || size < 1U) {
        *error = eeprom_max_wr;
        PRINT_ERROR(error);
        return;
    }
    if (addr > EEPROM_ADDR_RANGE) {
        *error = invalid_addr_range;
        PRINT_ERROR(error);
        return;
    }

    // say we tell it to write 16 bytes at addr 7FE, only write one byte in this case
    if ( (size+addr) > EEPROM_ADDR_RANGE) {
        size = EEPROM_ADDR_RANGE - addr;
        DEBUG_INFO("Tried to access end of EEPROM, reajusted size to %"PRIu16, size);
    }

    uint8_t wr_buffer[REGISTER_DATA_SIZE+1];
    /*
     * addr =                   B2 B1 B0 A7 A6 A5 A4 A3 A2 A1 A0
     * dev_addr =   01 00 01 00 B2 B1 B0 RW
     */
    wr_buffer[0] = (uint8_t) addr;
    addr &= 0b11100000000; // B2 B1 B0 0..0
    uint8_t dev_addr = (EEPROM_BASE_ADDR | (addr >> 7)) + 1;

    memcpy(wr_buffer+1, buffer, REGISTER_DATA_SIZE);

    HAL_StatusTypeDef ret;

    ret = HAL_I2C_Master_Transmit(i2c_eeprom, dev_addr, wr_buffer, size+1, HAL_MAX_DELAY);
    if (ret != HAL_OK) {
        *error = i2c_master_tx_fail;
        PRINT_ERROR(error);
        return;
    }
}

void eeprom_sanity_check(uint16_t addr, uint8_t mode) {
    printf(BOLD "EEPROM sanity check" RESETCOLOR "\r\n");
    Errors_t error;
    const uint8_t size = 4U;
    const uint8_t wait = 10U;
    uint8_t i;
    uint8_t buffer[REGISTER_DATA_SIZE] = {0};
    uint8_t compare_buffer[REGISTER_DATA_SIZE];

    eeprom_io wr;
    eeprom_io rd;
    switch(mode) {
        case(0):
            wr = &eeprom_write;
            rd = &eeprom_read;
            break;
        case(1):
            wr = &eeprom_write2;
            rd = &eeprom_read2;
            break;
        case(2):
            /*wr = &eeprom_write_dma;*/
            wr = &eeprom_write2;
            rd = &eeprom_read_dma;
            break;
        default:
            printf("Specified unexisting mode %" PRIu16 "\r\n", mode);
            return;
    }
    printf("Using mode %" PRIu16 "\r\n", mode);

    // ERASE MEMORY
    wr(buffer, addr, size, &error);
    if(error != no_error) {
        PRINT_ERROR(&error);
    }
    HAL_Delay(wait);
    rd(compare_buffer, addr, size, &error);

    if (memcmp(buffer, compare_buffer, size)) {
        printf("EEPROM sanity check" RED " FAILED" RESETCOLOR " at erase!\r\n");
    } else {
        printf("EEPROM erased " GREEN " PASSED" RESETCOLOR "\r\n");

        // WRITE DEADBEEF
        buffer[0] = 0xDE;
        buffer[1] = 0xAD;
        buffer[2] = 0xBE;
        buffer[3] = 0xEF;
        for(i = 0; i < size; ++i) {
            printf("buffer[%u] = %2x\r\n", i, buffer[i]);
        }
        wr(buffer, addr, size, &error);
        HAL_Delay(wait);

        // READ DEADBEEF
        rd(compare_buffer, addr, size, &error);
        if(error != no_error) {
            PRINT_ERROR(&error);
        }

        for(i = 0; i < size; ++i) {
            printf("EEPROM[%u] = %2x\r\n", i+addr, compare_buffer[i]);
        }

        if (memcmp(buffer, compare_buffer, size)) {
            printf("EEPROM sanity check" RED " FAILED" RESETCOLOR " at comparison!\r\n");
        } else {
            printf("EEPROM sanity check" GREEN " PASSED" RESETCOLOR "\r\n");
        }
    }
    printf(BOLD "End of sanity check" RESETCOLOR "\r\n");
}

bool eeprom_is_ready(uint16_t addr, Errors_t* error) {
    *error = no_error;
    if (!i2c_eeprom) {
        *error = bus_handler_not_set;
        PRINT_ERROR(error);
        return false;
    }
    if (addr > EEPROM_ADDR_RANGE) {
        *error = invalid_addr_range;
        PRINT_ERROR(error);
        return false;
    }
    addr &= 0b11100000000; // B2 B1 B0 0..0
    uint8_t dev_addr = (EEPROM_BASE_ADDR | (addr >> 7)) + 1;

    return HAL_I2C_IsDeviceReady(i2c_eeprom, dev_addr, NUMBER_TRIALS_DEV_READY, HAL_MAX_DELAY) == HAL_OK;
}

void eeprom_debug() {
    char rd_wr = '\0';
    uint16_t addr;
    uint8_t size;
    uint8_t i2c_buffer[REGISTER_DATA_SIZE];
    memset(i2c_buffer, 0xff, REGISTER_DATA_SIZE);
    Errors_t error;

    printf("Address?\r\n");
    scanf("%" SCNu16, &addr);
    printf("Address = %"PRIu16"\r\n", addr);

    if(!eeprom_is_ready(addr, &error)) {
        printf("EEPROM Address is not ready\r\n");
        return;
    }

    printf("Size?\r\n");
    scanf("%" SCNu8, &size);
    printf("Size = %"PRIu16"\r\n", size);

    printf("read [r], write [w], automatic test [t] or cancel [any key]?\r\n");
    scanf(" %c", &rd_wr);
    switch(rd_wr) {
        case ('R'):
        case ('r'):
            eeprom_read(i2c_buffer, addr, size, &error);
            if(error != no_error) printf("error = %u\r\n", error);
            buffer_print(i2c_buffer, size);
            break;
        case ('W'):
        case ('w'):
            for (uint16_t i = 0; i < size; ++i) {
                printf("data[%"PRIu16"] = ?\r\n", (addr+i));
                scanf("%" SCNu8, &i2c_buffer[i]);
                printf("write data = %"PRIu16"\r\n", i2c_buffer[i]);
            }
            eeprom_write(i2c_buffer, addr, size, &error);
            if(error != no_error) printf("error = %u\r\n", error);
            break;
        case ('T'):
        case ('t'):
            eeprom_sanity_check(addr, 0);
            break;
        default:
            printf("Canceled\r\n");
            break;
    }
}

void eeprom_dma_transmit_interrupt() {
    g_eeprom_dma_transmitted = true;
}

void eeprom_dma_receive_interrupt() {
    g_eeprom_dma_received = true;
}

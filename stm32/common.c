#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "common.h"

void print_error(Errors_t* error, char* message, uint8_t len) {
    switch(*error) {
        case(no_error):
            strncpy(message, "Ops! No error occurred (yet :) ).", len);
            break;
        case(invalid_bus_handler):
            strncpy(message, "Given BUS handler is invalid", len);
            break;
        case(bus_handler_not_set):
            strncpy(message, "No BUS handler was set", len);
            break;
        case(invalid_tx_rx_size):
            strncpy(message, "Wrong Tx/Rx size request.", len);
            break;
        case(invalid_addr_range):
            strncpy(message, "Invalid *register* address range.", len);
            break;
        case(i2c_master_tx_fail):
            strncpy(message, "I²C Master failed to perform Tx.", len);
            break;
        case(i2c_master_rx_fail):
            strncpy(message, "I²C Master failed to perform Rx.", len);
            break;
        case(i2c_master_mem_wr_fail):
            strncpy(message, "I²C Master failed to perform memory write.", len);
            break;
        case(i2c_master_mem_rd_fail):
            strncpy(message, "I²C Master failed to perform memory read.", len);
            break;
        case(eeprom_max_wr):
            strncpy(message, "EEPROM reached maximum write address.", len);
            break;
        case(uart_tx_fail):
            strncpy(message, "UART failed to perform Tx.", len);
            break;
        case(uart_rx_fail):
            strncpy(message, "UART failed to perform Rx.", len);
            break;
        default:
            strncpy(message, "Ops! Forgot to write the printf for error.", len);
            break;
    }
}

void buffer_print(uint8_t* buffer, uint8_t size) {
    printf(BOLD "Start of buffer" RESETCOLOR "\r\n");
    uint8_t cnt_size = size < REGISTER_DATA_SIZE ? size : REGISTER_DATA_SIZE;
    for(int i = 0; i < cnt_size; ++i) {
        printf("buffer[%u] = %2x\r\n", i, buffer[i]);
    }
    printf(BOLD "End of buffer" RESETCOLOR "\r\n");
}

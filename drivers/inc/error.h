/**
* @file error.h
* @author  Jie Liu
* @version V1.0
* @date    2021-08-18
* @brief Error handling macros, colors and defines
* @note Remember to include the <stdio.h> library when using the macros!
*/

#ifndef ERROR_H
#define ERROR_H


#include <stdio.h>
#include <string.h>
#include <errno.h>

#define RESETCOLOR  "\033[0m"   /* Reset Color */
#define BLACK       "\033[30m"  /* Black */
#define RED         "\033[31m"  /* Red */
#define GREEN       "\033[32m"  /* Green */
#define YELLOW      "\033[33m"  /* Yellow */
#define BLUE        "\033[34m"  /* Blue */
#define MAGENTA     "\033[35m"  /* Magenta */
#define CYAN        "\033[36m"  /* Cyan */
#define WHITE       "\033[37m"  /* White */
#define BOLD        "\033[1m"   /* Bold */

#define __DEBUGFILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)
#define DEBUG_INFO(str, args...)    printf(BLUE BOLD "[Info]:" RESETCOLOR BLUE " %s:%d:%s \t" str "\e[0m\r\n" RESETCOLOR, __DEBUGFILENAME__, __LINE__, __FUNCTION__, ##args);

#define print_error(n, str, args...) \
    printf(RED BOLD "Error[%u]:%s:%u:" RESETCOLOR RED " %s " RESETCOLOR args "\n", n, __FUNCTION__, __LINE__, str)
#define print_warning(n, str, args...) \
    printf(YELLOW BOLD "Warning[%u]:%s:%u:" RESETCOLOR YELLOW " %s " RESETCOLOR args "\n", n, __FUNCTION__, __LINE__, str)

#define print_errno(args...) \
    print_error(errno, strerror(errno), ##args)
#define print_warnno(args...) \
    print_warning(errno, strerror(errno), ##args)

/**
 * @defgroup error_codes error codes
 * @{
 */
#define ERROR_READ_TEST_FAILED 166
#define ERROR_FAILED_GETTING_CONFIGURATION 167
#define ERROR_MAX_BUFFER_SIZE_REACHED 168
#define ERROR_UNDEFINED_STATE 169
#define ERROR_STATE_MACHINE 170
#define ERROR_PARSER 171
#define ERROR_CHECKSUM_FAILED 172
#define ERROR_INVALID_BUFFER_SIZE 173
#define ERROR_SLAVE_NOT_UNDERSTAND 174
#define ERROR_SLAVE_UNEXPECTED_ANWER 175
#define ERROR_WRITE_REGISTER_FAILS 176
#define ERROR_READ_REGISTER_FAILS 177
#define ERROR_NOTHING_TO_READ 178
#define ERROR_NMEA_NOT_FOUND 179
///@}

/// debugging mode: activate with gcc's -D DEBUG
#ifdef DEBUG
#define SPI_DEBUG
#define UART_DEBUG
#define I2C_DEBUG

#define SPI_CHECK
#define UART_CHECK
#define I2C_CHECK
#endif /* DEBUG */

// list every SPI slave
#ifdef SPI_DEBUG
#define LSM_DEBUG
#endif /* SPI_DEBUG */

#ifdef UART_DEBUG
#define GNSS_DEBUG
#endif /* UART_DEBUG */

#ifdef I2C_DEBUG
#define PI4_DEBUG
#endif /* I2C_DEBUG */

// automatic checks (optional)
#ifdef SPI_CHECK
#define SPI_CHECK_WRITE
#define LSM_CHECK_INIT
#endif /* SPI_CHECK */

#ifdef UART_CHECK
#define GNSS_CHECK
#endif /* UART_CHECK */

#ifdef I2C_CHECK
#define PI4_CHECK_WRITE
#endif /* I2C_CHECK */

#endif /* ERROR_H */

// vim: expandtab ts=4 sw=4

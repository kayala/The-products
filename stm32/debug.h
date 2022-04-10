#ifndef DEBUG_H
#define DEBUG_H

#include <string.h>
#include <stdio.h>

#ifndef DEBUG_LEVEL
#define DEBUG_LEVEL 0
#endif // DEBUG_LEVEL

#define __DEBUGFILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)

/**
 * DEBUG_LEVEL
 * 0 = no debug
 * 1 = simple, with no color
 * 2 = complex, with color
 */

#if DEBUG_LEVEL == 0

#define DEBUG_INFO(str, args...)
#define DEBUG_INFO_INT(var)
#define DEBUG_INFO_FLOAT(var)

#define DEBUG_ERROR(str, args...)
#define DEBUG_ERROR_INT(var)
#define DEBUG_ERROR_FLOAT(var)

#elif DEBUG_LEVEL == 1

#define DEBUG_INFO(str, args...) printf(str "\r\n", ##args);
#define DEBUG_ERROR(str, args...) printf(str "\r\n", ##args);

#define DEBUG_INFO_INT(var)
#define DEBUG_INFO_FLOAT(var)
#define DEBUG_ERROR_INT(var)
#define DEBUG_ERROR_FLOAT(var)

#elif DEBUG_LEVEL > 1

/**
 * @brief Macro that prints debug information.
 * @param[in] str Desired string to print.
 * @param[in] args Arguments for printf, if there are any.
 */
#define DEBUG_INFO(str, args...) printf("\e[1;33m[INFO]: %s:%d@%s(): " str "\e[0m\r\n", __DEBUGFILENAME__, __LINE__, __FUNCTION__, ##args);

/**
 * @brief Macro that prints errors.
 * @param[in] str Desired string to print.
 * @param[in] args Arguments for printf, if there are any.
 */
#define DEBUG_ERROR(str, args...) printf("\e[1;31m[ERROR]: %s:%d@%s(): " str "\e[0m\r\n", __DEBUGFILENAME__, __LINE__, __FUNCTION__, ##args);

#endif /* DEBUG_LEVEL */

#if DEBUG_LEVEL > 0

/**
 * @brief Macro that prints an integer.
 * @param[in] str Desired integer to print.
 */
#define DEBUG_INFO_INT(var) printf("\e[1;33m[INFO INT]: %s:%d@%s(): %s: %lu\e[0m\r\n", __DEBUGFILENAME__, __LINE__, __FUNCTION__, #var, var);

/**
 * @brief Macro that prints a float.
 * @param[in] str Desired float to print.
 */
#define DEBUG_INFO_FLOAT(var) printf("\e[1;33m[INFO FLOAT]: %s:%d@%s(): %s: %f\e[0m\r\n", __DEBUGFILENAME__, __LINE__, __FUNCTION__, #var, var);

/**
 * @brief Macro that prints a number in hex format.
 * @param[in] str Desired number to print as hex.
 */
#define DEBUG_INFO_HEX(var, zeros) printf("\e[1;33m[INFO HEX]: %s:%d@%s(): " #var ": 0x%0" #zeros "x\033[0m\r\n", __DEBUGFILENAME__, __LINE__, __FUNCTION__, var);

/**
 * @brief Macro that prints an integer.
 * @param[in] str Desired integer to print.
 */
#define DEBUG_ERROR_INT(var) printf("\e[1;31m[ERROR INT]: %s:%ul:%s: %s: %lu\e[0m\r\n", __DEBUGFILENAME__, __LINE__, __FUNCTION__, #var, var);

/**
 * @brief Macro that prints a float.
 * @param[in] str Desired float to print.
 */
#define DEBUG_ERROR_FLOAT(var) printf("\e[1;31m[ERROR FLOAT]: %s:%d:%s: %s: %f\e[0m\r\n", __DEBUGFILENAME__, __LINE__, __FUNCTION__, #var, var);

/**
 * @brief Macro that prints a number in hex format.
 * @param[in] str Desired number to print as hex.
 */
#define DEBUG_ERROR_HEX(var, zeros) printf("\e[1;31m[ERROR HEX]: %s:%d:%s: " #var ": 0x%0" #zeros "x\033[0m\r\n", __DEBUGFILENAME__, __LINE__, __FUNCTION__, var);

#endif /* DEBUG_LEVEL > 0 */

#endif /* DEBUG_H */

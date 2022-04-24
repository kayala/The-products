/**
* @file gnss.h
* @author  Jie Liu
* @version V1.0
* @date    2021-08-18
* @brief Driver for the GNSS Module (L76-LB or LC79D)
* @note Only the UART implementation
*/

#ifndef GNSS_H
#define GNSS_H

#include "common.h"

/** maximum number of characters in a field for NMEA messages */
#define NMEA_FIELD_BUFFER 20UL
/** maximum number of fields in a NMEA message */
#define NMEA_MAX_FIELDS 32UL

#if NMEA_MAX_FIELDS*NMEA_FIELD_BUFFER > MESSAGE_SIZE
#error MESSAGE_SIZE is insuficient, increase MESSAGE_SIZE to at least NMEA_MAX_FIELDS*NMEA_FIELD_BUFFER
#endif

/** possible directions for NMEA data */
enum nmea_dir {
	N, /// North
	S, /// South
	W, /// West
	E  /// East
};

/** NMEA messages */
enum nmea_msg {
    RMC, /// Recommended Minimum Specific GNSS Data
    GGA, /// Global Position System Fix Data
    GLL, /// Geographical Position - Latitude and Longitude
    GSV, /// GNSS Satellites in View
    GSA, /// GNSS DOP and Active Satellites
    VTG,  /// Course Over Ground and Ground Speed
    PQCFGNMEAMSGERROR, /// Failed to understand configuration setting
    PQCFGNMEAMSGOK /// Successful configuration
};

/** NMEA talker ID */
enum nmea_talker {
    GP, /// GPS only
    GL, /// GLONASS only
    GA, /// Galileo only
    GB, /// BeiDou only
    GI, /// NAVIC only
    GQ, /// GZSS only
    GN  /// multi-constellation
};

/** NMEA units */
enum nmea_unit {
	M, /// Meter
	F  /// Feet
};

/**
 * @brief Same as uart_read, but returns only the NMEA message
 * @note Maximum message size is defined by MESSAGE_SIZE
 * @param[in] dev device file
 * @param[out] message NMEA message
 * @return error code
 */
int nmea_read(int* dev, char* message);

/**
 * @brief Calculates the NMEA checksum
 * @note You may give the message including the $ and * with checksum, the
 * function will take only the content part.
 * @param message whole NMEA message
 * @return checksum value
 */
uint8_t nmea_checksum(char *message);

/**
 * @brief Open and configure GNSS Module features
 * @param[in] dev pointer to device file
 * @param[in] block_device absolute path to block device
 * @param[in] speed communication speed (use constants, eg. B115200)
 * @return error code
 */
int gnss_init(int* dev, char* block_device);

/**
 * @brief Parse incomming NMEA message, separating it into fields
 * @note field are returned as string, only checksum is interpreted
 * @param[in] message NMEA message
 * @param[out] fields vector containing the fields values
 * @param[out] number_of_fields number of fields
 * @param[out] checksum checksum
 * @return error code
 */
int nmea_parse_fields(char* message,
        char fields[NMEA_MAX_FIELDS][NMEA_FIELD_BUFFER],
        uint8_t* number_of_fields, uint8_t* checksum);

/**
 * @brief Configure the type of output
 * @note This could be implemented in a way such that specific types of messages
 * are received, but for now this is ok.
 * @param[in] dev device file
 * @return error code
 */
int nmea_enable_geographical_latitude_longitude(int* dev);

#endif /* GNSS_H */

// vim: expandtab ts=4 sw=4

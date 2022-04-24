/**
 * @file    gnss.c
 * @author  Jie Liu
 * @version V1.0
 * @date    2021-08-18
 * @brief Driver for the GNSS Module (L76-LB or LC79D)
 * @note Only the UART implementation
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>

#include "gnss.h"
#include "common.h"
#include "error.h"

uint8_t nmea_checksum(char *message) {
    // check for correctness: https://www.hhhh.org/wiml/proj/nmeaxor.html
    // checks only from the characters between $ and * (actual content)

    uint8_t checksum = 0;

    char* walker = message;
    while (*walker) {
        if (*walker != '$') checksum ^= *walker;
        walker++;
        if (*walker == '*') break;
    }

#ifdef GNSS_DEBUG
    /*printf("message: %s\nChecksum: 0x%02X\n", message, checksum);*/
#endif /* GNSS_DEBUG */

    return checksum;
}

int nmea_read(int* dev, char* message) {

    char msg[MESSAGE_SIZE] = { '\0' };
    char* begin;
    char* walker;
    int count_end_msg = 0;
    int ret = EXIT_SUCCESS;

#ifdef TESTING
    strcpy(msg,"oi$GPPGA,1,2*98\r\ntchau");
    printf("testing message = %s\n", msg);
#else
    ret = uart_read(dev, msg);
    if (ret != EXIT_SUCCESS)
        return ret;
#endif

    walker = msg;

    while (*walker) {

#ifdef TESTING
        printf("walker = %c, count = %u\n", *walker, count_end_msg);
#endif
        if (count_end_msg == 3) // end of message
            break;
        if (count_end_msg > 0) // tick end of msg
            ++count_end_msg;

        if (*walker == '$') { // start of message
            begin = walker;
        } else if (*walker == '*') { // start of checksum
            // msg:           ...XX*CS
            // count_end_msg: ...00123
            count_end_msg = 1;
        }
        ++walker;
    }

    if (count_end_msg != 3) {
        print_error(ERROR_NMEA_NOT_FOUND, "NMEA message was not found!");
        return ERROR_NMEA_NOT_FOUND;
    }

    memcpy(message,begin,walker-begin);

#ifdef GNSS_DEBUG
    printf("nmea_read(): NMEA message = %s\n", message);
#endif /* GNSS_DEBUG */

    return EXIT_SUCCESS;
}

int gnss_init(int* dev, char* block_device) {

    /* LC76-LB GNSS Module's serial configuration
     * Baud rate: 115200bps
     * 8 bits
     * no parity
     * 1 stop bit
     * no flow control
     */

#ifdef GNSS_DEBUG
    DEBUG_INFO();
#endif /* GNSS_DEBUG */

    return uart_init(dev, block_device, B115200);
}

int nmea_parse_fields(char* message,
        char fields[NMEA_MAX_FIELDS][NMEA_FIELD_BUFFER],
        uint8_t* number_of_fields, uint8_t* checksum) {

#ifdef DEBUG
    DEBUG_INFO();
#endif /* DEBUG */

    char fields_buffer[NMEA_MAX_FIELDS][NMEA_FIELD_BUFFER]; // last field is the checksum
    char* buffer; // buffer that will be filling until hit a separator
    char* walker;
    uint8_t number_fields;
    int char_cnt;
    int i;

    // initialize
    char_cnt = 0;
    number_fields = 0;
    walker = message;
    buffer = (char*) calloc(NMEA_FIELD_BUFFER, sizeof(char));
    if (!buffer) {
        print_errno("calloc failed");
        return errno;
    }
    memset(buffer, '\0', NMEA_FIELD_BUFFER);

    // separate messages into fields
    while(*walker) {
        ++walker;

        if (char_cnt >= NMEA_FIELD_BUFFER-1) {
            print_error(ERROR_MAX_BUFFER_SIZE_REACHED, "buffer overflow, increase NMEA_FIELD_BUFFER size");
            return ERROR_MAX_BUFFER_SIZE_REACHED;
        }

        if (*walker == ','  || // new field
            *walker == '*'  || // new field: checksum
            *walker == '\r' || // message ended
            *walker == '\n' )
        {
            strcpy(fields_buffer[number_fields],buffer);
            // reinitializing buffer and character for new field
            char_cnt = 0;
            ++number_fields; // adds one more, even if fields has already ended
            memset(buffer, '\0', NMEA_FIELD_BUFFER);
            continue;
        }

        buffer[char_cnt++] = *walker;
    }

    number_fields -= 2; // not counting the addition add, nor the checksum

    // copy results back to outputs, while separating the checksum
    *number_of_fields = number_fields;

    *checksum = strtol(fields_buffer[number_fields], NULL, 16);

    for (i = 0; i < *number_of_fields; ++i)
        strcpy(fields[i], fields_buffer[i]);

#ifdef GNSS_DEBUG
    printf("field[0...%u] = \n  ", *number_of_fields-1);
    for (i = 0; i < *number_of_fields-2; ++i)
        printf("%s, ", fields[i]);
    printf("%s\n", fields[*number_of_fields-1]);
#endif /* GNSS_DEBUG */

#ifdef GNSS_CHECK
    if (*checksum != nmea_checksum(message)) {
        print_warning(ERROR_CHECKSUM_FAILED, "checksum calculation is incorrect, message is corrupted");
        printf("checksum calculation is incorrect, message is corrupted\n");
        return ERROR_CHECKSUM_FAILED;
    }
#endif /* GNSS_CHECK */

    free(buffer);

    return EXIT_SUCCESS;
}

int nmea_enable_geographical_latitude_longitude(int* dev) {

    /*
     * For implementing in a field basis, one would have to use boolean/integer
     * to string conversion and then calculate the checksum
     */

    char* wr_msg = "$PQCFGNMEAMSG,1,0,0,0,0,1,0*01\r\n\0";
    char rd_msg[MESSAGE_SIZE];

    char fields[NMEA_MAX_FIELDS][NMEA_FIELD_BUFFER];
    uint8_t number_of_fields;
    uint8_t checksum;

    // not emptying the read buffer
    uart_write(dev, wr_msg);

    // not waiting! this may cause some problem, lets test first
    nmea_read(dev, rd_msg);

    // testing if module understood
    nmea_parse_fields(rd_msg, fields, &number_of_fields, &checksum);
    if (number_of_fields != 2) {
        // response is message type and checksum
        print_warning(ERROR_SLAVE_UNEXPECTED_ANWER,"expected answer from GNSS module");
        return ERROR_SLAVE_UNEXPECTED_ANWER;
    }
    if (! strcmp(fields[0],"PQCFGNMEAMSGERROR")) {
        print_error(ERROR_SLAVE_NOT_UNDERSTAND, "received error message");
        return ERROR_SLAVE_NOT_UNDERSTAND;
    }
    if (strcmp(fields[0],"PQCFGNMEAMSGOK")) {
        print_error(EXIT_FAILURE, "failed for unknown reason");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

// vim: expandtab ts=4 sw=4

/******************************************************************************
* @file    csv_manipulation.h
* @author  Jie Liu
* @version V1.0
* @date    2021-10-27
* @brief   einfache CSV Datei mit folgenden Inhalten
*''Time:APDS1:APDS2:APDS3:BME1:BME2:BME3:LIS2x:LIS2y:LIS2z:MLX:LSMAng:LSMLin:GNSS'''
*******************************************************************************/

#ifndef IOL_CSV_MANIPULATION_H
#define IOL_CSV_MANIPULATION_H

/**
* @brief  write data into sensor_output.csv
* @param act_slv activated sensor slave
* @param str data need to be written into csv file
*/
void write_csv_data (uint8_t act_slv, char* str);
/**
* @brief  delay time
* @param micros microseconds for delay
*/
int delay(unsigned long micros);
/**
* @brief  control the write process
* @param dev_id ID of device
*/
void write_control(uint8_t dev_id);
#endif //IOL_CSV_MANIPULATION_H

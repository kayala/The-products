/******************************************************************************
* @file    csv_manipulation.c
* @author  Jie Liu
* @version V1.0
* @date    2021-10-27
* @brief   simple CSV file with the following content
*''Time:APDS1:APDS2:APDS3:BME1:BME2:BME3:LIS2x:LIS2y:LIS2z:MLX:LSMAng:LSMLin:GNSS'''
*******************************************************************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>
#include "common.h"

void write_csv_data (uint8_t act_slv, char* str, uint8_t num) {
    FILE* f;
    char filename[64];
	snprintf(filename, 63, "rss_output%d.csv", num);
    if ((f = fopen(filename, "w")) == 0)
    {
        fprintf(stderr, "fopen %s failed\n", filename);
        exit(1);
    }
    if (act_slv == 0){
        time_t timeStamp;
        time(&timeStamp);
        fprintf(f, "%s,", ctime(&timeStamp));
    }
	fprintf(f, "%s", str);
    if(act_slv == 3){
        fprintf(f, "\n");
    }
    fclose(f);
}

int delay(unsigned long micros)
{
    struct timespec ts;
    int err;

    ts.tv_sec = micros/1000000L;
    ts.tv_nsec = (micros % 1000000L) * 1000L;
    err = nanosleep(&ts, (struct timespec*)NULL);
    return (err);
}

void write_control(uint8_t dev_id)
{
    int t_old;
    int t_new;
    int t_init;
    time_t timestamp;
    t_old =0;
    t_new = time(&timestamp);
    t_init = t_new;
    uint8_t quit_write;
    quit_write = 1;
    long microseconds = 999000;
    uint8_t file_number=0;
    while(quit_write){
        if ((t_new - t_old) >= 30){
            t_old=t_new;
            for (uint8_t act_slv = 0; act_slv<=0; act_slv ++){
                char buffer[32];
                sensor_activate(act_slv, dev_id);
                memset(buffer, 0, sizeof(buffer));
                sensor_measure(act_slv, dev_id, buffer, sizeof(buffer));
                write_csv_data(act_slv, buffer,file_number);
                }
            t_new = time(&timestamp);
        }else{
            delay(microseconds);
            t_new = time(&timestamp);
        }
        if((t_new - t_init)>=3600){
            file_number+=1;
            t_new = time(&timestamp);
            t_init = t_new;
            t_old = 0;
        }
        if(file_number>7){
            quit_write = 0;
        }
    }
};

/**
 * @file main.h
 * @author  Jie Liu
 * @version V1.0
 * @date    2021-08-18
 * @brief Gather everything together for the Raspberry Sensor Shield Project
 */

#include "common.h"
#include "error.h"
#include "pi4.h"
#include "lsm.h"
#include "apds.h"
#include "bme.h"
#include "gnss.h"
#include "lis2.h"
#include "mlx.h"


#define I2C_DRV 1 
#define SPI_DRV 0

#define  PORT_SWITCH  I2C_DRV 

/** possible slaves */
#if 0
typedef enum {
    apds = 0, /// light quality sensor
    bme,      /// air quality sensor
    lis,      /// 1.6 to 16000Hz 3-axis accelerometer
    mlx,      /// infrared thermometer
    lsm,      /// 400Hz xyz-accelerometer
    gnss,     /// GNSS (satelite) module
    iis,      /// 5kHz low-noise 3-axis accelerometer
    pi4,      /// I2C multiplexer
    error     /// error state
} slave;
#endif
int main(int argc, char* argv[]);

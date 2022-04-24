/******************************************************************************
* @file    bme.c
* @author  Jie Liu
* @version V1.0
* @date    2021-08-18
* @brief   I2C interface to the BME sensor via the new API.
*          reads temperature, pressure, humidity and gas sensor ohmic values
*******************************************************************************
* bus    = I2C
*******************************************************************************/

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include <linux/i2c-dev.h>
#include <fcntl.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <unistd.h>
#include "bme.h"
#include "bme68x.h"
#include "bme68x_defs.h"
#include "common.h"

#undef PARALLEL_MODE
#undef BME68X_USE_FPU
#define SAMPLE_COUNT UINT16_C(300)

static struct bme68x_dev dev;
static struct bme68x_conf conf;
static struct bme68x_heatr_conf heatr_conf;

static inline uint64_t gettime_us()
{
  struct timeval tv;
  gettimeofday(&tv, NULL);
  return (int64_t)((tv.tv_sec) * 1000000 + tv.tv_usec);
}

#define bme68x_check_rslt(name,rslt) do				  \
	{											      \
		if (rslt != BME68X_OK)					      \
			printf("bme68x %s error %u", name, rslt); \
	} while (0)

void bme_init(uint8_t fd) // fd of i2c
{
	int8_t rslt;
	dev.intf_ptr = &fd;
	dev.read = i2c_read_8bit;
	dev.write = i2c_write;
    dev.delay_us = delay_us;
	dev.intf = BME68X_I2C_INTF;
    rslt = bme68x_init(&dev);
    bme68x_check_rslt("bme68x_init", rslt);

    /* Set the temperature, pressure and humidity settings */
#ifdef PARALLEL_MODE
    conf.os_hum = BME68X_OS_1X;
    conf.os_temp = BME68X_OS_2X;
    conf.os_pres = BME68X_OS_16X;
    conf.filter = BME68X_FILTER_OFF;
	// Standby time between sequential mode measurement profiles. ODR/Standby time
    conf.odr = BME68X_ODR_NONE;
#else
    conf.os_hum = BME68X_OS_16X;
    conf.os_pres = BME68X_OS_1X;
    conf.os_temp = BME68X_OS_2X;
    conf.filter = BME68X_FILTER_OFF;
    conf.odr = BME68X_ODR_NONE;
#endif
    rslt = bme68x_set_conf(&conf, &dev);
    bme68x_check_rslt("bme68x_set_conf", rslt);

    /* Create a ramp heat waveform in 3 steps */
    heatr_conf.enable =  BME68X_ENABLE;
    heatr_conf.heatr_temp = 300; // Celsius
    heatr_conf.heatr_dur = 100;  // ms
#ifdef PARALLEL_MODE
    /* Heater temperature in degree Celsius */
    uint16_t temp_prof[] = { 320, 100, 100, 100, 200, 200, 200, 320, 320, 320 };
    /* Multiplier to the shared heater duration */
    uint16_t mul_prof[] = { 5, 2, 10, 30, 5, 5, 5, 5, 5, 5 };
    heatr_conf.heatr_temp_prof = temp_prof;
    heatr_conf.heatr_dur_prof = mul_prof;
    heatr_conf.profile_len = 10;
    /* Shared heating duration in milliseconds */
    heatr_conf.shared_heatr_dur = 140 -
		(bme68x_get_meas_dur(BME68X_PARALLEL_MODE, &conf, &dev) / 1000);
	
    rslt = bme68x_set_heatr_conf(BME68X_PARALLEL_MODE, &heatr_conf, &dev);
    bme68x_check_rslt("bme68x_set_heatr_conf", rslt);
#else // FORCED_MODE (default)
    heatr_conf.profile_len = 0;
    rslt = bme68x_set_heatr_conf(BME68X_FORCED_MODE, &heatr_conf, &dev);
#endif
}

void bme_measure(int32_t *temp, uint32_t *pres, uint32_t *hum, uint32_t *gas_res)
{
	int8_t rslt;
#ifdef PARALLEL_MODE
    struct bme68x_data data[3]; // max n_fields
#else
    struct bme68x_data data;
#endif
	// Get the total measurement duration so as to sleep or wait till the
	// measurement is complete
    // int8_t delay = 3;
    uint16_t sample_count = 1;
    uint8_t n_fields;
    uint32_t del_period;
	uint64_t time_us;
#ifdef PARALLEL_MODE
	const uint8_t mode = BME68X_PARALLEL_MODE;
#else
	const uint8_t mode = BME68X_FORCED_MODE;
#endif
    while (sample_count <= SAMPLE_COUNT)
    {
		rslt = bme68x_set_op_mode(mode, &dev);
        bme68x_check_rslt("bme68x_set_op_mode", rslt);

        /* Calculate delay period in microseconds */
        del_period = bme68x_get_meas_dur(mode, &conf, &dev)
			+ (heatr_conf.heatr_dur * 1000);
        dev.delay_us(del_period, dev.intf_ptr);
        time_us = gettime_us();

        /* Check if rslt == BME68X_OK, report or handle if otherwise */
        rslt = bme68x_get_data(mode, &data, &n_fields, &dev);
        bme68x_check_rslt("bme68x_get_data", rslt);

#ifdef PARALLEL_MODE
        for (uint8_t i = 0; i < n_fields; i++)
		{
			// Avoid using measurements from an unstable heating setup
			// heater stability
            if (data[i].status & BME68X_HEAT_STAB_MSK)
            {
				*temp = data[i].temperature / 100;
				*pres = data[i].pressure;
				*hum  = data[i].humidity / 1000 ;
				if (data[i].status & BME68X_GASM_VALID_MSK) {
					*gas_res = data[i].gas_resistance;
				}
			}
#ifdef DEBUG
            printf("%u, %lu, %d, %lu, %lu, %lu, 0x%x\n",
                   sample_count,
                   (long unsigned int)time_us,
                   (data[i].temperature / 100),
                   (long unsigned int)data[i].pressure,
                   (long unsigned int)(data[i].humidity / 1000),
                   (long unsigned int)data[i].gas_resistance,
                   data[i].status);
#endif
            sample_count++;
        }
#else // FORCED_MODE
		if (n_fields)
        {
            if (data.status & BME68X_HEAT_STAB_MSK)
            {
				*temp = data.temperature / 100;
				*pres = data.pressure;
				*hum  = data.humidity / 1000 ;
				if (data.status & BME68X_GASM_VALID_MSK) {
					*gas_res = data.gas_resistance;
				}
			}
#ifdef DEBUG
            printf("%u, %lu, %d, %lu, %lu, %lu, 0x%x\n",
                   sample_count,
                   (long unsigned int)time_us,
                   (data.temperature / 100),
                   (long unsigned int)data.pressure,
                   (long unsigned int)(data.humidity / 1000),
                   (long unsigned int)data.gas_resistance,
                   data.status);
#endif
            sample_count++;
        }
#endif
    }
}

#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "i2c_lib.h"

void i2c_scan(I2C_HandleTypeDef* hi2c) {
    printf(BOLD "I2C scan start" RESETCOLOR "\r\n");
    for (uint8_t i = 0; i < 128; ++i) {
        if (HAL_I2C_IsDeviceReady(hi2c, (uint16_t)(i<<1), NUMBER_TRIALS_DEV_READY, HAL_MAX_DELAY) == HAL_OK) {
            printf("%2x ", i);
        } else {
            printf("-- ");
        }
        if (i > 0 && (i + 1) % 16 == 0) printf("\r\n");
    }
    printf(BOLD "I2C scan end" RESETCOLOR "\r\n");
    const uint8_t wait = 3U;
    HAL_Delay(wait); // because we just interacted with i2c
}

#ifndef PL57H56_H_
#define PL57H56_H_

#include <stdint.h>
#include "nrf_drv_timer.h"

#define TIME_MS(a) 1000000/(2*a*173)
typedef struct
{
    uint32_t pin_number_step;
    uint32_t pin_number_dir;
    uint32_t pin_number_enb;
}pin_t;
typedef struct
{
    float RPM_first;
    float RPM_second;
    uint8_t DIR;
}conf_t;
void gradual(float rpm_two);
void pl_init(pin_t *pstr_t);
void pl_start(uint8_t dir);
 #endif

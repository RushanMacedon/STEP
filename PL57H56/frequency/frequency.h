#ifndef FREQUENCY_H_
#define FREQUENCY_H_

#include <stdint.h>
#include "nrf_drv_timer.h"

typedef struct
{
    uint32_t pin_number;
}freq_t;

void freq_pin_Hz(float frequency);
void freq_init(uint32_t pin_number);
void freq_timer_change(uint32_t frequency);
 #endif
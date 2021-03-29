#include <stdbool.h>
#include <stdint.h>
#include "nrf.h"
#include "nrf_drv_timer.h"
#include "bsp.h"
#include "app_error.h"
#include "PL57H56.h"
#include "frequency.h"

#define SPEED_STEP 5.0f
pin_t loc_t;
conf_t c_t;
const nrf_drv_timer_t TIMER_GRAD = NRF_DRV_TIMER_INSTANCE(1);
nrf_drv_timer_config_t timer_confg;

void timer_grand_event_handler(nrf_timer_event_t event_type, void* p_context)
{
 
  
    switch (event_type)
    {
        case NRF_TIMER_EVENT_COMPARE1:
            if (c_t.RPM_second > c_t.RPM_first)
            {
                c_t.RPM_first += SPEED_STEP;
                
            }
            if (c_t.RPM_second < c_t.RPM_first)
            {
                c_t.RPM_first -= SPEED_STEP;
                
            }
           
            break;
            
       case NRF_TIMER_EVENT_COMPARE2:
            
            freq_pin_Hz(c_t.RPM_first);
            break;
        default:
            //Do nothing.
            break;
    }
}
void pl_init(pin_t *pstr_t)
{
    nrf_gpio_cfg_output(pstr_t->pin_number_step);
    nrf_gpio_cfg_output(pstr_t->pin_number_dir);
    nrf_gpio_cfg_output(pstr_t->pin_number_enb);
    memcpy(&loc_t,pstr_t,sizeof(pin_t));   
        timer_confg.frequency = (nrf_timer_frequency_t)NRF_TIMER_FREQ_500kHz; 
        timer_confg.mode  = (nrf_timer_mode_t)NRF_TIMER_MODE_TIMER;
        timer_confg.bit_width = (nrf_timer_bit_width_t)NRF_TIMER_BIT_WIDTH_32; 
        timer_confg.interrupt_priority = 7; 
    uint32_t time_ms = 3000; 
    uint32_t time_ticks;
    uint32_t time_ticks1;
    uint32_t err_code = NRF_SUCCESS; 
    err_code = nrf_drv_timer_init(&TIMER_GRAD, &timer_confg, timer_grand_event_handler);    
    time_ticks = nrf_drv_timer_ms_to_ticks(&TIMER_GRAD, time_ms);
    time_ticks1 = nrf_drv_timer_ms_to_ticks(&TIMER_GRAD, time_ms+100);
    nrf_drv_timer_extended_compare(
    &TIMER_GRAD, NRF_TIMER_CC_CHANNEL1, time_ticks, NRF_TIMER_SHORT_COMPARE1_CLEAR_MASK, true);
    nrf_drv_timer_extended_compare(
    &TIMER_GRAD, NRF_TIMER_CC_CHANNEL2, time_ticks1, NRF_TIMER_SHORT_COMPARE2_CLEAR_MASK, true);
    nrf_drv_timer_enable(&TIMER_GRAD);
    freq_init(loc_t.pin_number_step);
}
 


void gradual(float rpm_two)
{
    c_t.RPM_second = rpm_two;
}
void pl_start(uint8_t dir)
{
        if (dir == 0)
        {
             NRF_GPIO->OUTSET = (1UL << loc_t.pin_number_dir);  
        }
        else
        {
             NRF_GPIO->OUTCLR = (1UL << loc_t.pin_number_dir);
        }
        c_t.RPM_first = 1.0f;
        c_t.RPM_second = 10.0f;
        freq_pin_Hz(c_t.RPM_first);
}

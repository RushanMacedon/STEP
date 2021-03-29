#include <stdbool.h>
#include <stdint.h>
#include "nrf.h"
#include "nrf_drv_timer.h"
#include "bsp.h"
#include "app_error.h"
#include "frequency.h"

const nrf_drv_timer_t TIMER_FREQ = NRF_DRV_TIMER_INSTANCE(2);
nrf_drv_timer_config_t timer_cfg;
freq_t p_t_local;
static float freq_tmp = 0;
uint32_t flag_freq = 1;
uint32_t time_ticks;  
    float time_us;

void timer_led_event_handler(nrf_timer_event_t event_type, void* p_context)
{
 
  
    switch (event_type)
    {
        case NRF_TIMER_EVENT_COMPARE0:
            nrf_gpio_pin_toggle(p_t_local.pin_number);
            break;

        default:
            //Do nothing.
            break;
    }
}

void freq_init(uint32_t pin_number)
{
    p_t_local.pin_number = pin_number;
    timer_cfg.frequency = (nrf_timer_frequency_t)NRF_TIMER_FREQ_500kHz; 
    timer_cfg.mode  = (nrf_timer_mode_t)NRF_TIMER_MODE_TIMER;
    timer_cfg.bit_width = (nrf_timer_bit_width_t)NRF_TIMER_BIT_WIDTH_32; 
    timer_cfg.interrupt_priority = 6; 
    uint32_t err_code = NRF_SUCCESS;
    err_code = nrf_drv_timer_init(&TIMER_FREQ, &timer_cfg, timer_led_event_handler);
    APP_ERROR_CHECK(err_code);
}

void freq_pin_Hz(float frequency)
 {
    
    if((frequency != 0) && (flag_freq == 1))
    {
    
    time_us = 1000000.0f/(2.0f*frequency); 
        
    time_ticks = nrf_drv_timer_us_to_ticks(&TIMER_FREQ, (uint32_t)time_us);
    nrf_drv_timer_extended_compare(
    &TIMER_FREQ, NRF_TIMER_CC_CHANNEL0, time_ticks, NRF_TIMER_SHORT_COMPARE0_CLEAR_MASK, true);
    nrf_drv_timer_enable(&TIMER_FREQ);
    flag_freq = 0;
    freq_tmp = frequency;
    }
    if(frequency == 0)
    {
        nrf_drv_timer_disable(&TIMER_FREQ);
        flag_freq = 0;
    }
    if (frequency != freq_tmp)
    {
        time_us = 1000000.0f/(2.0f*frequency); 
     
        time_ticks = nrf_drv_timer_us_to_ticks(&TIMER_FREQ, (uint32_t)time_us);

        nrf_drv_timer_extended_compare(
         &TIMER_FREQ, NRF_TIMER_CC_CHANNEL0, time_ticks, NRF_TIMER_SHORT_COMPARE0_CLEAR_MASK, true);
}    
        }
//  void freq_timer_change(uint32_t frequency)
//{
//    uint32_t time_us = 1000000/(2*frequency); 
//    uint32_t time_ticks;
//    time_ticks = nrf_drv_timer_us_to_ticks(&TIMER_FREQ, time_us);

//    nrf_drv_timer_extended_compare(
//         &TIMER_FREQ, NRF_TIMER_CC_CHANNEL0, time_ticks, NRF_TIMER_SHORT_COMPARE0_CLEAR_MASK, true);
//}    

  void freq_timer_stop(void)
 {
     nrf_drv_timer_disable(&TIMER_FREQ);
     nrf_drv_timer_uninit(&TIMER_FREQ);
 }
 
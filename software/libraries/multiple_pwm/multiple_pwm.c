////////////////////////////////////////////////////
//
// PROJECT C5: Chau's Cool Controlled Climbing Car
// Final Project EECS C149
//
// Authors: Katelyn Biesiadecki, Chau Van, Jo Koe
// Date: November 17, 2020
//
////////////////////////////////////////////////////

#include "multiple_pwm.h"


// Set duty cycle between 0 and 100%
void pwm_update_duty_cycle(uint8_t duty_cycle, bool direction)
{
    // Move forward by default with REAR_RIGHT_A and REAR_LEFT_B
    if (duty_cycle >= 100) {
            seq_values->channel_0 = 100;
            seq_values->channel_1 = 100;
            seq_values->channel_2 = 100;
            seq_values->channel_3 = 100;
        } else {
            seq_values->channel_0 = duty_cycle;
            seq_values->channel_1 = duty_cycle;
            seq_values->channel_2 = duty_cycle;
            seq_values->channel_3 = duty_cycle;
        }
    
    // potentially have different sequences for backwards/forwards
    nrf_drv_pwm_simple_playback(&m_pwm0, &seq, 3, NRF_DRV_PWM_FLAG_LOOP);
    nrf_drv_pwm_simple_playback(&m_pwm1, &seq, 3, NRF_DRV_PWM_FLAG_LOOP);
    nrf_drv_pwm_simple_playback(&m_pwm2, &seq, 3, NRF_DRV_PWM_FLAG_LOOP);
}

void pwm_init(nrf_drv_pwm_t * m_pwm, int one, int two, int three, int four)
{
    nrf_drv_pwm_config_t const config =
    {
        .output_pins = { one, two, three, four }, // channel 1-4
        .irq_priority = APP_IRQ_PRIORITY_LOWEST,
        .base_clock   = NRF_PWM_CLK_1MHz,
        .count_mode   = NRF_PWM_MODE_UP,
        .top_value    = 100,
        .load_mode    = NRF_PWM_LOAD_INDIVIDUAL,
        .step_mode    = NRF_PWM_STEP_AUTO
    };
      
    // Init PWM without error handler
    APP_ERROR_CHECK(nrf_drv_pwm_init(m_pwm, &config, NULL));
}


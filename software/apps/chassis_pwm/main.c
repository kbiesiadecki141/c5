////////////////////////////////////////////////////
//
// PROJECT C5: Chau's Cool Controlled Climbing Car
// Final Project EECS C149
//
// Authors: Katelyn Biesiadecki, Chau Van, Jo Koe
// Date: November 17, 2020
//
////////////////////////////////////////////////////

#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "app_error.h"
#include "app_timer.h"
#include "nrf.h"
#include "nrf_delay.h"
#include "nrf_gpio.h"

#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"

#include "nrf_pwr_mgmt.h"
#include "nrf_drv_spi.h"
#include "nrf_drv_pwm.h"
#include "nrf_drv_clock.h"
#include "app_util_platform.h"

#include "buckler.h"
#include "romiActuator.h"
#include "romiSensorPoll.h"
#include "romiSensorTypes.h"
#include "romiUtilities.h"

#include "app_pwm.h"
#include "c5_config.h"

#include "states.h"

// We want to use the PWM drivers, not the PWM library and APP_PWM_INSTANCE.
nrf_drv_pwm_t m_pwm0 = NRF_DRV_PWM_INSTANCE(0);
nrf_drv_pwm_t m_pwm1 = NRF_DRV_PWM_INSTANCE(1);
nrf_drv_pwm_t m_pwm2 = NRF_DRV_PWM_INSTANCE(2);

// This is for tracking PWM instances being used, so we can unintialize only
// the relevant ones when switching from one demo to another.
#define USED_PWM(idx) (1UL << idx)
static uint8_t m_used = 0;

// define the pwm time period for pwm wave
// 5000, 20000 is another nice-sounding frequency
#define FREQ_IN_US 5000

/*

#define REAR_RIGHT_A   19  // this pin is connected with enable A pin of L298N module
#define REAR_LEFT_B    25  // this pin is connected with enable B pin of L298N module
#define RL_A1   20  // Rear Left channel 1, driven by ENA
#define RL_A2   22  // Rear Left channel 2
#define RR_B1   23  // Rear Right channel 1, driven by ENB
#define RR_B2   24  // Rear Right channel 2

#define FRONT_LEFT_A   13  // this pin is connected with enable A pin of L298N module
#define FRONT_LEFT_B   18  // this pin is connected with enable B pin of L298N module
#define FL_A1  14  // Front Left Bogie A channel 1, driven by ENA
#define FL_A2  15  // Front Left Bogie A channel 2
#define FL_B1  16  // Front Left Bogie B channel 1, driven by ENB
#define FL_B2  17  // Front Left Bogie B channel 2

#define FRONT_RIGHT_A     8  // this pin is connected with enable A pin of L298N module
#define FRONT_RIGHT_B     31  // this pin is connected with enable B pin of L298N module
#define FR_A1  7  // Front Right Bogie A channel 1
#define FR_A2  6  // Front Right Bogie A channel 2
#define FR_B1  5  // Front Right Bogie B channel 1
#define FR_B2  21  // Front Right Bogie B channel 2

// On-board LEDs test example
#define OUTPUT_PIN_1 17
#define OUTPUT_PIN_2 18
#define OUTPUT_PIN_3 19
#define OUTPUT_PIN_4 20
*/

// Declare variables holding PWM sequence values. In this example only one channel is used 
nrf_pwm_values_individual_t seq_values[] = {0, 0, 0, 0};

nrf_pwm_sequence_t const seq =
{
    .values.p_individual = seq_values,
    .length          = NRF_PWM_VALUES_LENGTH(seq_values),
    .repeats         = 0,
    .end_delay       = 0
};

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
    printf("init %02x \n", m_pwm);
    APP_ERROR_CHECK(nrf_drv_pwm_init(m_pwm, &config, NULL));
}

int main(void)
{
    // initialize RTT library
    ret_code_t error_code = NRF_SUCCESS;
    error_code = NRF_LOG_INIT(NULL);
    APP_ERROR_CHECK(error_code);
    NRF_LOG_DEFAULT_BACKENDS_INIT();
    printf("Log initialized!\n");

    printf("why \n");

    // Start clock for accurate frequencies
    NRF_CLOCK->TASKS_HFCLKSTART = 1; 
    // Wait for clock to start
    while(NRF_CLOCK->EVENTS_HFCLKSTARTED == 0) ;
    
    // ugly style below FIXME
    // Front left config...
    pwm_init(&m_pwm0, FRONT_LEFT_A, FRONT_LEFT_B,
                      FL_A2 | NRF_DRV_PWM_PIN_INVERTED,
                      FL_B2 | NRF_DRV_PWM_PIN_INVERTED);

    pwm_init(&m_pwm1, FRONT_RIGHT_A, FRONT_RIGHT_B,
                      FR_A2 | NRF_DRV_PWM_PIN_INVERTED,
                      FR_B2 | NRF_DRV_PWM_PIN_INVERTED);
                  
    pwm_init(&m_pwm2, REAR_RIGHT_A, REAR_LEFT_B,
                      RR_B1 | NRF_DRV_PWM_PIN_INVERTED,
                      RL_A2 | NRF_DRV_PWM_PIN_INVERTED);

    // bool dir = true; // goes backwards, clockwise when false

    for (;;)
    {
        nrf_delay_ms(10); // why is the 10 ms delay so long LOL
        pwm_update_duty_cycle(0, false);

        /*j
        for(int i = 0; i <= 100; i++)
        {
            nrf_delay_ms(10);
            pwm_update_duty_cycle(i, false);
        }
        */
    }
}


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

#include "states.h"

// We want to use the PWM drivers, not the PWM library and APP_PWM_INSTANCE.
static nrf_drv_pwm_t m_pwm0 = NRF_DRV_PWM_INSTANCE(0);
static nrf_drv_pwm_t m_pwm1 = NRF_DRV_PWM_INSTANCE(1);
static nrf_drv_pwm_t m_pwm2 = NRF_DRV_PWM_INSTANCE(2);

// define the pwm time period for pwm wave
// 5000, 20000 is another nice-sounding frequency
#define FREQ_IN_US  20000

#define ENABLE_PIN_A   25  // this pin is connected with enable 1 pin of L298N module
#define ENABLE_PIN_B   19  // this pin is connected with enable 2 pin of L298N module
#define INPUT_1   24  // this pin is connected with IN1
#define INPUT_2   23  // this pin is connected with IN2
#define INPUT_3   22  // this pin is connected with IN3
#define INPUT_4   20  // this pin is connected with IN4

#define FRONT_LEFT_A    12  // this pin is connected with enable 1 pin of L298N module
#define FRONT_RIGHT_B   17  // this pin is connected with enable 2 pin of L298N module
#define INPUT_5  13
#define INPUT_6  14
#define INPUT_7  15
#define INPUT_8  16

static volatile bool ready_flag_1; // show PWM state if it's ready to update the new duty values
static volatile bool ready_flag_2; // show PWM state if it's ready to update the new duty values

void pwm_ready_handler_1(uint32_t pwm_id)
{
    // this pwm handler will be called after each pwm wave
    ready_flag_1 = true; // set the flag to true so that we can update new duty values
}

void pwm_ready_handler_2(uint32_t pwm_id)
{
    // this pwm handler will be called after each pwm wave
    ready_flag_2 = true; // set the flag to true so that we can update new duty values
}

// Declare variables holding PWM sequence values. In this example only one channel is used
nrf_pwm_values_individual_t seq_values[] = {0, 50, 1, 80};
nrf_pwm_sequence_t const seq =
{
    .values.p_individual = seq_values,
    .length          = NRF_PWM_VALUES_LENGTH(seq_values),
    .repeats         = 2,
    .end_delay       = 1
};

// Set duty cycle between 0 and 100%
void pwm_update_duty_cycle(uint8_t duty_cycle)
{

    // Check if value is outside of range. If so, set to 80%
    if(duty_cycle >= 80)
    {
        seq_values->channel_0 = 80;
    }
    else
    {
        seq_values->channel_0 = duty_cycle;
    }

    nrf_drv_pwm_simple_playback(&m_pwm1, &seq, 1, NRF_DRV_PWM_FLAG_LOOP);
}

static void pwm_init(void)
{
    nrf_drv_pwm_config_t const config1 =
    {
        .output_pins =
        {
            INPUT_1,             // channel 0
            INPUT_2,             // channel 1
            INPUT_3,             // channel 2
            INPUT_4,             // channel 3
        },
        .irq_priority = APP_IRQ_PRIORITY_LOWEST,
        .base_clock   = NRF_PWM_CLK_125KHz,
        .count_mode   = NRF_PWM_MODE_UP,
        .top_value    = 80,
        .load_mode    = NRF_PWM_LOAD_INDIVIDUAL,
        .step_mode    = NRF_PWM_STEP_AUTO
    };
    // Init PWM without error handler - remember to add back in
    APP_ERROR_CHECK(nrf_drv_pwm_init(&m_pwm1, &config1, NULL));

}


int main(void)
{
    // Start clock for accurate frequencies
    NRF_CLOCK->TASKS_HFCLKSTART = 1;
    // Wait for clock to start
    while(NRF_CLOCK->EVENTS_HFCLKSTARTED == 0);

    pwm_init();

    pwm_update_duty_cycle(0);

    for (;;)
    {
     __WFE;
    }

  }

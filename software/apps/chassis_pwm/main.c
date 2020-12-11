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

// This is for tracking PWM instances being used, so we can unintialize only
// the relevant ones when switching from one demo to another.
#define USED_PWM(idx) (1UL << idx)
static uint8_t m_used = 0;

// define the pwm time period for pwm wave
// 5000, 20000 is another nice-sounding frequency
#define FREQ_IN_US 5000 // 12/11 fix nasty af frequency

#define ENABLE_PIN_A   25  // this pin is connected with enable 1 pin of L298N module
#define ENABLE_PIN_B   19  // this pin is connected with enable 2 pin of L298N module
#define INPUT_1   24  // this pin is connected with IN1 24
#define INPUT_2   23  // this pin is connected with IN2
#define INPUT_3   22  // this pin is connected with IN3
#define INPUT_4   20  // this pin is connected with IN4

#define FRONT_LEFT_A    12  // this pin is connected with enable 1 pin of L298N module
#define FRONT_RIGHT_B   17  // this pin is connected with enable 2 pin of L298N module
#define INPUT_5  13
#define INPUT_6  14
#define INPUT_7  15
#define INPUT_8  16

// On-board LED test example
#define OUTPUT_PIN 17
#define OUTPUT_PIN_2 18
#define OUTPUT_PIN_3 19
#define OUTPUT_PIN_4 20

// Declare variables holding PWM sequence values. In this example only one channel is used 
nrf_pwm_values_individual_t seq_values[] = {0, 0, 0, 0};
nrf_pwm_sequence_t const seq =
{
    .values.p_individual = seq_values,
    .length          = NRF_PWM_VALUES_LENGTH(seq_values),
    .repeats         = 0,
    .end_delay       = 0
};

// static nrf_pwm_values_common_t seq_values[];

// Set duty cycle between 0 and 100%
void pwm_update_duty_cycle(uint8_t duty_cycle, bool direction)
{
    if (duty_cycle >= 100) {
            seq_values->channel_0 = 100;
            seq_values->channel_2 = 100;
        } else {
            seq_values->channel_0 = duty_cycle | (1 << 15);
            seq_values->channel_2 = duty_cycle | (1 << 15);
        }
    
    nrf_drv_pwm_simple_playback(&m_pwm0, &seq, 3, NRF_DRV_PWM_FLAG_LOOP);
}

// will have to make separate pwm_init functions if using this format for directions
static void pwm_init(void)
{
    nrf_drv_pwm_config_t const config0 =
    {
        .output_pins =
        {
            ENABLE_PIN_A | NRF_DRV_PWM_PIN_INVERTED, // channel 0 // move forward by default
            INPUT_1 | NRF_DRV_PWM_PIN_INVERTED,             // channel 1
            ENABLE_PIN_B | NRF_DRV_PWM_PIN_INVERTED,             // channel 2
            INPUT_4 | NRF_DRV_PWM_PIN_INVERTED,             // channel 3
        },
        .irq_priority = APP_IRQ_PRIORITY_LOWEST,
        .base_clock   = NRF_PWM_CLK_1MHz,
        .count_mode   = NRF_PWM_MODE_UP,
        .top_value    = 100,
        .load_mode    = NRF_PWM_LOAD_INDIVIDUAL,
        .step_mode    = NRF_PWM_STEP_AUTO
    };
    // Init PWM without error handler
    APP_ERROR_CHECK(nrf_drv_pwm_init(&m_pwm0, &config0, NULL));
    
}

int main(void)
{

    // Start clock for accurate frequencies
    NRF_CLOCK->TASKS_HFCLKSTART = 1; 
    // Wait for clock to start
    while(NRF_CLOCK->EVENTS_HFCLKSTARTED == 0) 
        ;
    
    pwm_init();

    bool dir = true; // goes backwards, clockwise when false

    for (;;)
    {
        for(int i = 0; i <= 100; i++)
        {
            nrf_delay_ms(10);
            pwm_update_duty_cycle(i, false);
        }
    }
}


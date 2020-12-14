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

#include "romiActuator.h"
#include "romiSensorPoll.h"
#include "romiSensorTypes.h"
#include "romiUtilities.h"

#include "app_pwm.h"


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
#define FREQ_IN_US 20000


// Declare variables holding PWM sequence values. In this example only one channel is used 
static nrf_pwm_values_individual_t seq_values[] = {0, 0, 0, 0};

static nrf_pwm_sequence_t const seq =
{
    .values.p_individual = seq_values,
    .length          = NRF_PWM_VALUES_LENGTH(seq_values),
    .repeats         = 0,
    .end_delay       = 0
};

// Set duty cycle between 0 and 100%
void pwm_update_duty_cycle(uint8_t duty_cycle, bool direction);

void pwm_init(nrf_drv_pwm_t * m_pwm, int one, int two, int three, int four);

/*
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
    
    //pwm_init1();
    // pwm_init2();
    // pwm_init3();
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

        for(int i = 0; i <= 100; i++)
        {
            nrf_delay_ms(10); // why is the 10 ms delay so long LOL
            pwm_update_duty_cycle(i, false);
        }
    }
}
*/


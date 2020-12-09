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

#include "buckler.h"
#include "display.h"
#include "kobukiActuator.h"
#include "kobukiSensorPoll.h"
#include "kobukiSensorTypes.h"
#include "kobukiUtilities.h"
#include "lsm9ds1.h"
#include "simple_ble.h"

#include "app_pwm.h"

#include "states.h"

// Create a handle which will point to the PWM and also pass the hardware timer instance.
// Here, 1 indicates that we are using the hardware timer
APP_PWM_INSTANCE(m_pwm, 1);
// nrf_drv_pwm_t m_pwm = NRF_DRV_PWM_INSTANCE(1);

// define the pwm time period for pwm wave 
// 20000 is another nice-sounding frequency
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

static volatile bool ready_flag; // show PWM state if it's ready to update the new duty values

void pwm_ready_handler(uint32_t pwm_id)
{
    // this pwm handler will be called after each pwm wave
    ready_flag = true; // set the flag to tru so that we can update new duty values
}

int main(void)
{
    // configure the pins as outputs 
    nrf_gpio_cfg_output(INPUT_1); 
    nrf_gpio_cfg_output(INPUT_2); 
    nrf_gpio_cfg_output(INPUT_3); 
    nrf_gpio_cfg_output(INPUT_4);

    // set direction to clockwise
    nrf_gpio_pin_set(INPUT_1);  
    nrf_gpio_pin_clear(INPUT_2); 
    nrf_gpio_pin_set(INPUT_4);  
    nrf_gpio_pin_clear(INPUT_3);

    ret_code_t err_code; // a variable to hold error value

    // create a struct to hold the configuration values and pass it the default configurations
    // along with the time period of the pwm wave and the pin on which we want to generate pwm signals

    app_pwm_config_t pwm_config = APP_PWM_DEFAULT_CONFIG_2CH(FREQ_IN_US, ENABLE_PIN_A, ENABLE_PIN_B);

    // set the polarity as active high which means the duty will be reflected according to logic high state for e.g 
    // if we set the duty to 80 percent then during the 80 percent time period the pin will remain in high state. 
    pwm_config.pin_polarity[0] = APP_PWM_POLARITY_ACTIVE_HIGH;
    pwm_config.pin_polarity[1] = APP_PWM_POLARITY_ACTIVE_HIGH;

    // initialize the pwm more details in pwm library with hardware timers tutorial be sure to check it out first then 
    // come to this tutorial
    err_code = app_pwm_init(&m_pwm, &pwm_config, pwm_ready_handler);
    APP_ERROR_CHECK(err_code);
    app_pwm_enable(&m_pwm); // enable the pwm so that it can start working

    while (true) {

      // update the duty values and turn on the led to show a state
      while(app_pwm_channel_duty_set(&m_pwm, 0, 50) == NRF_ERROR_BUSY);
      while(app_pwm_channel_duty_set(&m_pwm, 1, 50) == NRF_ERROR_BUSY);
      nrf_delay_ms(5000);

    }
  }

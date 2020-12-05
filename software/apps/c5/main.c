// Robot Template app
//
// Framework for creating applications that control the Romi robot

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

#include "romiUtilities.h"
#include "romiFunctions.h"

// I2C manager
// NRF_TWI_MNGR_DEF(twi_mngr_instance, 5, 0);

int main(void) {
  ret_code_t error_code = NRF_SUCCESS;

  // initialize RTT library
  error_code = NRF_LOG_INIT(NULL);
  APP_ERROR_CHECK(error_code);
  NRF_LOG_DEFAULT_BACKENDS_INIT();
  printf("Log initialized!\n");

  // initialize Romi
  romiInit();
  printf("Romi initialized!\n");

  // configure initial state
  robot_state_t state = OFF;
  bool button_pressed;
  bool anyBump;

  bool turn_right;
  RomiSensors_t sensors = {0};

  // loop forever, running state machine
  while (1) {
    // read sensors from robot
    read_sensors(&sensors);
    button_pressed = is_button_pressed(&sensors);
    anyBump = obstacle_detected(&sensors, &turn_right);

    // delay before continuing
    // Note: removing this delay will make responses quicker, but will result
    //  in printf's in this loop breaking JTAG
    nrf_delay_ms(1);

    // handle states
    switch(state) {
      case OFF: {
        // transition logic
        if (button_pressed) {
          state = TELEOP;
        } else {
          // perform state-specific actions here
          stop();
          state = OFF;
        }
        break; // each case needs to end with break!
      }

      case TELEOP: {
        // transition logic
        if (button_pressed){
          state = OFF;
        } else if (anyBump){
          set_speeds(-100, -100);
          state = AVOID;
        } else {
          // perform state-specific actions here
          set_speeds(100, 100);
          state = TELEOP;
        }
        break; // each case needs to end with break!
      }

      case AVOID: {
        // transition logic
        if (button_pressed){
          state = OFF;
        } else if (sensors.bumps_wheelDrops.bumpCenter){
          state = TELEOP;
        } else {
          // perform state-specific actions here
          if(turn_right){
            set_speeds(100, -100);
          }else{
            set_speeds(-100, 100);
          }
          state = AVOID;
        }
        break; // each case needs to end with break!
      }

      case TUNNEL:{
        break;
      }
    }
  }
}


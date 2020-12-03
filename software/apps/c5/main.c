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

#include "romiActuator.h"
#include "romiUtilities.h"
#include "romiSensorPoll.h"

#include "c5Utilities.h"

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
  bool bumpLeft;
  bool bumpCenter;
  bool bumpRight;
  bool anyBump;

  bool turn_right;
  RomiSensors_t sensors = {0};

  // loop forever, running state machine
  while (1) {
    // read sensors from robot
    romiSensorPoll(&sensors);
    button_pressed = is_button_pressed(&sensors);
    bumpLeft = sensors.bumps_wheelDrops.bumpLeft;
    bumpCenter = sensors.bumps_wheelDrops.bumpCenter;
    bumpRight = sensors.bumps_wheelDrops.bumpRight;
    anyBump = bumpLeft || bumpCenter || bumpRight;

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
          romiDriveDirect(0, 0);
          state = OFF;
        }
        break; // each case needs to end with break!
      }

      case TELEOP: {
        // transition logic
        if (button_pressed){
          state = OFF;
        } else if (anyBump){
          if(bumpLeft){
            turn_right = true;
          } else{
            turn_right = false;
          }
          romiDriveDirect(-100, -100);
          state = AVOID;
        } else {
          // perform state-specific actions here
          romiDriveDirect(100, 100);
          state = TELEOP;
        }
        break; // each case needs to end with break!
      }

      case AVOID: {
        // transition logic
        if (button_pressed){
          state = OFF;
        } else if (bumpCenter){
          state = TELEOP;
        } else {
          // perform state-specific actions here
          if(turn_right){
            romiDriveDirect(100, -100);
          }else{
            romiDriveDirect(-100, 100);
          }
          state = AVOID;
        }
        break; // each case needs to end with break!
      }
    }
  }
}


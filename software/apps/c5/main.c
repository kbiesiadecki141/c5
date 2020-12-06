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

  float front_close = 0.2;
  float side_close = 1;//0.2;
  float max_speed = 100;

  // configure initial state
  robot_state_t state = OFF;
  bool button_pressed;
  bool obs_detected;
  bool obs_avoided;
  bool in_tunnel;

  bool turn_right;
  float front_diff;
  float back_diff;
  RomiSensors_t sensors = {0};

  // loop forever, running state machine
  while (1) {
    // read sensors from robot
    read_sensors(&sensors);
    button_pressed = is_button_pressed(&sensors);
    obs_detected = obstacle_detected(&sensors, &turn_right);
    obs_avoided = obstacle_avoided(&sensors, front_close);
    in_tunnel = inside_tunnel(&sensors, side_close);

    // delay before continuing
    // Note: removing this delay will make responses quicker, but will result
    //  in printf's in this loop breaking JTAG
    nrf_delay_ms(1);

    // handle states
    switch(state) {
      case OFF: {
        // transition logic
        if (button_pressed) {
          state = TUNNEL;
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
        } else if (obs_detected){
          state = AVOID;
        } else if (in_tunnel){
          state = TUNNEL;
        } else {
          // perform state-specific actions here
          set_speeds(max_speed, max_speed);
          state = TELEOP;
        }
        break; // each case needs to end with break!
      }

      case AVOID: {
        // transition logic
        if (button_pressed){
          state = OFF;
        } else if (!(obs_detected) && obs_avoided){
          if(in_tunnel){
            state = TUNNEL;
          } else{
            state = TELEOP;
          }
        } else {
          // perform state-specific actions here
          if(turn_right){
            set_speeds(max_speed, -max_speed);
          }else{
            set_speeds(-max_speed, max_speed);
          }
          state = AVOID;
        }
        break; // each case needs to end with break!
      }

      case TUNNEL:{
        // transition logic
        if (button_pressed){
          state = OFF;
        } else if (obs_detected){
          state = AVOID;
        } else if (! in_tunnel){
          state = TELEOP;
        } else {
          // perform state-specific actions here

          front_diff = us_diff(&sensors, true);
          back_diff = us_diff(&sensors, false);
          //printf("Cliff diff: %lf\n", front_diff);

          set_speeds(max_speed/2 * (1 - front_diff / side_close), max_speed/2 * (1 + front_diff / side_close));
          
          state = TUNNEL;
        }
        break; // each case needs to end with break!

      }
    }
  }
}


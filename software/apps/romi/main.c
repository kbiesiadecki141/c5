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
  printf("Initializing\n");
  ret_code_t error_code = NRF_SUCCESS;

  // initialize RTT library
  error_code = NRF_LOG_INIT(NULL);
  APP_ERROR_CHECK(error_code);
  NRF_LOG_DEFAULT_BACKENDS_INIT();
  printf("Log initialized!\n");

  // initialize Romi
  romiInit();
  // initialize_robot();
  printf("Romi initialized!\n");

  float front_close = 0.2;
  float side_close = 0.3;
  float max_diff = side_close;
  float max_speed = 100;
  float turning_speed = max_speed/2;

  // configure initial state
  robot_state_t state = OFF;
  bool button_pressed;
  bool obs_detected;
  bool obs_avoided;
  bool in_tunnel;

  bool turn_right;
  float front_us;
  float left_us;
  float right_us;
  float side_diff;
  float side_sum;
  int back_up_time = 20;
  int turning_time = 20;
  int back_up_counter;
  int turning_counter;
  RomiSensors_t sensors = {0};

  // loop forever, running state machine
  while (1) {
    // read sensors from robot
    read_sensors(&sensors, &front_us, &left_us, &right_us);
    // printf("left us: %lf\n", right_us);
    button_pressed = is_button_pressed(&sensors);
    obs_detected = obstacle_detected(&sensors, &turn_right);
    obs_avoided = !(obs_detected) && obstacle_avoided(&front_us, front_close);
    // nrf_delay_ms(1000);
    in_tunnel = inside_tunnel(&left_us, &right_us, side_close);

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
          printf("TELEOP\n");
        } else {
          // perform state-specific actions here
          stop();
          state = OFF;
        }
        break; // each case needs to end with break!
      }

      case TELEOP: {
        // transition logic
        if (button_pressed) {
          state = OFF;
        } else if (obs_detected) {
          set_speeds(-max_speed, -max_speed);
          state = AVOID;
          printf("AVOID\n");
          back_up_counter = 0;
        } else if (in_tunnel) {
          state = TUNNEL;
          printf("TUNNEL\n");
        } else {
          // perform state-specific actions here
          set_speeds(max_speed, max_speed);
          state = TELEOP;
        }
        break; // each case needs to end with break!
      }

      case AVOID: {
        // transition logic

        // printf("obstacle_avoided %d\n", obs_avoided);
        // printf("back_up_counter %d\n", back_up_counter);
        // printf("turning_counter %d\n", turning_counter);
        if (button_pressed) {
          state = OFF;
        } else if (obs_avoided && back_up_counter >= back_up_time && turning_counter >= turning_time) {
          if (in_tunnel) {
            state = TUNNEL;
            printf("TUNNEL\n");
          } else {
            state = TELEOP;
            printf("TELEOP\n");
          }
        } else {
          // perform state-specific actions here
          if (back_up_counter < back_up_time) {
            back_up_counter = back_up_counter + 1;
            set_speeds(-max_speed, -max_speed);
          } else {
            if (obs_avoided) {
              turning_counter = turning_counter + 1;
            } else {
              turning_counter = 0;
            }
            if (turn_right) {
              set_speeds(turning_speed, -turning_speed);
            } else {
              set_speeds(-turning_speed, turning_speed);
            }
          }
          state = AVOID;
        }
        break; // each case needs to end with break!
      }

      case TUNNEL:{
        // transition logic
        if (button_pressed) {
          state = OFF;
        } else if (obs_detected) {
          set_speeds(-max_speed, -max_speed);
          state = AVOID;
          printf("AVOID\n");
          back_up_counter = 0;
        } else if (! in_tunnel) {
          state = TELEOP;
          printf("TELEOP\n");
        } else {
          // perform state-specific actions here

          side_diff = right_us - left_us;
          side_sum = right_us + left_us;//us_diff(&left_us, &right_us);
          if(fabs(side_diff) < 0.005){
            set_speeds(50, 50);
          } 
          else if (side_diff > 0){
            set_speeds(0, 50);
          }
          else{
            set_speeds(50, 0);
          }
          // int left_speed = (int)(max_speed/3. - (max_speed/3.) * (side_diff / side_sum));
          // int right_speed = (int)(max_speed/3. + (max_speed/3.) * (side_diff / side_sum));
          // // printf("speed diff: %d\n", right_speed - left_speed);
          // // printf("side diff: %f\n", 1000*side_diff);
          // // printf("side left: %d\n", left_speed);
          // // printf("side right: %d\n", right_speed);
          // // printf("us left: %f\n", 1000*left_us);
          // // printf("us right: %f\n", 1000* right_us);

          // set_speeds(left_speed, right_speed);
          
          state = TUNNEL;
        }
        break; // each case needs to end with break!

      }
    }
    // nrf_delay_ms(1000);
  }
}


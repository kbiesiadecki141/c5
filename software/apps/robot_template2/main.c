// Robot Template app
//
// Framework for creating applications that control the Kobuki robot

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

#include "romiFunctions.h"
#include "romiUtilities.h"

// I2C manager
// NRF_TWI_MNGR_DEF(twi_mngr_instance, 5, 0);

int main(void) {
  ret_code_t error_code = NRF_SUCCESS;

  // initialize RTT library
  error_code = NRF_LOG_INIT(NULL);
  APP_ERROR_CHECK(error_code);
  NRF_LOG_DEFAULT_BACKENDS_INIT();
  printf("Log initialized!\n");

  // initialize Kobuki
  romiInit();
  printf("Kobuki initialized!\n");

  // configure initial state
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
  int back_up_time = 20;
  int turning_time = 10;
  int back_up_counter;
  int turning_counter;
  RomiSensors_t sensors = {0};

  // loop forever, running state machine
  while (1) {
    // read sensors from robot
    read_sensors(&sensors);

    // delay before continuing
    // Note: removing this delay will make responses quicker, but will result
    //  in printf's in this loop breaking JTAG
    nrf_delay_ms(1);

    // handle states
    switch(state) {
      case OFF: {
        // transition logic
        if (is_button_pressed(&sensors)) {
          // left_encoder_prev = sensors.leftWheelEncoder;
          // right_encoder_prev = sensors.rightWheelEncoder;
          // left_encoder_dist = 0;
          // right_encoder_dist = 0;
          state = TELEOP;
        } else {
          // perform state-specific actions here
          set_speeds(0, 0);
          state = OFF;
        }
        break; // each case needs to end with break!
      }

      case TELEOP: {
        // transition logic
        if (is_button_pressed(&sensors)){
          state = OFF;
        // } else if (sensors.bumps_wheelDrops.bumpLeft || sensors.bumps_wheelDrops.bumpCenter || sensors.bumps_wheelDrops.bumpRight){
        //   if(sensors.bumps_wheelDrops.bumpLeft){
        //     b_state = LEFT;
        //   } else if (sensors.bumps_wheelDrops.bumpCenter){
        //     b_state = CENTER;
        //   } else {
        //     b_state = RIGHT;
        //   }
        //   lsm9ds1_stop_gyro_integration();
        //   lsm9ds1_start_gyro_integration();
        //   left_encoder_prev = sensors.leftWheelEncoder;
        //   right_encoder_prev = sensors.rightWheelEncoder;
        //   left_encoder_dist = 0;
        //   right_encoder_dist = 0;   
        //   state = AVOID; 
        // } else if (left_encoder_dist >= 0.2 || right_encoder_dist >= 0.2) {
        //   lsm9ds1_start_gyro_integration();
        //   state = TURNING_CLOCKWISE;
        } else {
          // perform state-specific actions here
          set_speeds(75, 75);

          // char buf0 [16];
          // snprintf ( buf0 , 16 , "DRIVING LEFT %f", left_encoder_dist);
          // display_write ( buf0 , DISPLAY_LINE_0 ) ;
          // char buf1 [16];
          // snprintf ( buf1 , 16 , "DRIVING RIGHT %f", right_encoder_dist);
          // display_write ( buf1 , DISPLAY_LINE_1 ) ;

          // left_encoder_dist += measure_distance(sensors.leftWheelEncoder, left_encoder_prev);
          // right_encoder_dist += measure_distance(sensors.rightWheelEncoder, right_encoder_prev);

          // left_encoder_prev = sensors.leftWheelEncoder;
          // right_encoder_prev = sensors.rightWheelEncoder;

          state = TELEOP;
        }
        break; // each case needs to end with break!
      }

    }
  }
}


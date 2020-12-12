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

#include "romiActuator.h"
#include "romiSensorPoll.h"
#include "romiSensorTypes.h"
#include "romiUtilities.h"

// I2C manager
// NRF_TWI_MNGR_DEF(twi_mngr_instance, 5, 0);

typedef enum {
  OFF,
  DRIVING,
  TURNING_CLOCKWISE,
  AVOID
} robot_state_t;

typedef enum {
  LEFT,
  CENTER,
  RIGHT
} bumper_t;

static float measure_distance ( uint16_t current_encoder, uint16_t previous_encoder ) {

  // conversion from encoder ticks to meters
  const float CONVERSION = 0.0006108;
  // calculate result here and return it
  float dist = current_encoder - previous_encoder;
  if(dist > 65535/2){
    dist -= 65535;
  }
  if(dist < -65535/2){
    dist += 65535;
  }
  return dist * CONVERSION;
}

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
  robot_state_t state = OFF;
  bumper_t b_state = LEFT;
  RomiSensors_t sensors = {0};

  uint16_t left_encoder_prev = 0;
  uint16_t right_encoder_prev = 0;
  float left_encoder_dist = 0;
  float right_encoder_dist = 0;

  // loop forever, running state machine
  while (1) {
    // read sensors from robot
    romiSensorPoll(&sensors);

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
          state = DRIVING;
        } else {
          // perform state-specific actions here
          romiDriveDirect(0, 0);
          state = OFF;
        }
        break; // each case needs to end with break!
      }

      case DRIVING: {
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
          romiDriveDirect(75, 75);

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

          state = DRIVING;
        }
        break; // each case needs to end with break!
      }

      // add other cases here
      case TURNING_CLOCKWISE: {
        // transition logic
        if (is_button_pressed(&sensors)){
          // lsm9ds1_stop_gyro_integration();
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
        // } else if (lsm9ds1_read_gyro_integration().z_axis <= -90){
        //   left_encoder_prev = sensors.leftWheelEncoder;
        //   right_encoder_prev = sensors.rightWheelEncoder;
        //   left_encoder_dist = 0;
        //   right_encoder_dist = 0;
        //   lsm9ds1_stop_gyro_integration();
        //   state = DRIVING;

        } else {
          // perform state-specific actions here
          romiDriveDirect(50, 0);
          state = TURNING_CLOCKWISE;
        }
        break; // each case needs to end with break!
      }

      case AVOID: {
        // transition logic
        if (is_button_pressed(&sensors)){
          // lsm9ds1_stop_gyro_integration();
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
        //   romiDriveDirect(-50, -50);
        //   state = AVOID; 
        // } else if (left_encoder_dist <= -0.1 && abs(lsm9ds1_read_gyro_integration().z_axis) >= 45){
        //   left_encoder_prev = sensors.leftWheelEncoder;
        //   right_encoder_prev = sensors.rightWheelEncoder;
        //   left_encoder_dist = 0;
        //   right_encoder_dist = 0;
        //   lsm9ds1_stop_gyro_integration();
        //   state = DRIVING;
        // } else {
        //   // perform state-specific actions here
        //   if(left_encoder_dist > -0.1){
        //     romiDriveDirect(-50, -50);
        //     left_encoder_dist += measure_distance(sensors.leftWheelEncoder, left_encoder_prev);
        //     left_encoder_prev = sensors.leftWheelEncoder;
        //   } else{
        //     if(b_state == LEFT){
        //       romiDriveDirect(0, -50);
        //     }
        //     else{
        //       romiDriveDirect(-50, 0);
        //     }
        //   }

          state = AVOID;
        }
        break; // each case needs to end with break!
      }
    }
  }
}


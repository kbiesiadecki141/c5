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
#include "nrf_drv_pwm.h"

#include "c5Functions.h"

// We want to use the PWM drivers, not the PWM library and APP_PWM_INSTANCE.
nrf_drv_pwm_t m_pwm0 = NRF_DRV_PWM_INSTANCE(0);
nrf_drv_pwm_t m_pwm1 = NRF_DRV_PWM_INSTANCE(1);
nrf_drv_pwm_t m_pwm2 = NRF_DRV_PWM_INSTANCE(2);

// This is for tracking PWM instances being used, so we can unintialize only
// the relevant ones when switching from one demo to another.
#define USED_PWM(idx) (1UL << idx)
uint8_t m_used = 0;

// define the pwm time period for pwm wave
// 5000, 20000 is another nice-sounding frequency
#define FREQ_IN_US 20000


// Declare variables holding PWM sequence values. In this example only one channel is used 
nrf_pwm_values_individual_t seq_values[] = {0, 0, 0, 0};

nrf_pwm_sequence_t const seq =
{
    .values.p_individual = seq_values,
    .length          = NRF_PWM_VALUES_LENGTH(seq_values),
    .repeats         = 0,
    .end_delay       = 0
};
// Set duty cycle between 0 and 100%
void pwm_update_duty_cycle(uint8_t duty_cycle)
{
    // Move forward by default with REAR_RIGHT_A and REAR_LEFT_B
    if (duty_cycle >= 100) {
          seq_values->channel_0 = 100;
          seq_values->channel_1 = 100;
          seq_values->channel_2 = 100;
          seq_values->channel_3 = 100;
    } else {
            seq_values->channel_0 = duty_cycle;
            seq_values->channel_1 = duty_cycle;
            seq_values->channel_2 = duty_cycle;
            seq_values->channel_3 = duty_cycle;
    }

    // potentially have different sequences for backwards/forwards
    nrf_drv_pwm_simple_playback(&m_pwm0, &seq, 1, NRF_DRV_PWM_FLAG_LOOP);
    nrf_drv_pwm_simple_playback(&m_pwm1, &seq, 1, NRF_DRV_PWM_FLAG_LOOP);
    nrf_drv_pwm_simple_playback(&m_pwm2, &seq, 1, NRF_DRV_PWM_FLAG_LOOP);
}
void pwm_init(nrf_drv_pwm_t * m_pwm, int one, int two, int three, int four)
{
    nrf_drv_pwm_config_t const config =
    {
        .output_pins = { one, two, three, four }, // channel 1-4
        .irq_priority = APP_IRQ_PRIORITY_LOWEST,
        .base_clock   = NRF_PWM_CLK_1MHz,
        .count_mode   = NRF_PWM_MODE_UP,
        .top_value    = 100,
        .load_mode    = NRF_PWM_LOAD_INDIVIDUAL,
        .step_mode    = NRF_PWM_STEP_AUTO
    };

    // Init PWM without error handler
    printf("init %02x \n", m_pwm);
    APP_ERROR_CHECK(nrf_drv_pwm_init(m_pwm, &config, NULL));
}

void drive_forward()
{
  pwm_init(&m_pwm0, FRONT_LEFT_A, FRONT_LEFT_B,
                      FL_A2 | NRF_DRV_PWM_PIN_INVERTED,
                      FL_B2 | NRF_DRV_PWM_PIN_INVERTED);

    pwm_init(&m_pwm1, FRONT_RIGHT_A, FRONT_RIGHT_B,
                      FR_A2 | NRF_DRV_PWM_PIN_INVERTED,
                      FR_B2 | NRF_DRV_PWM_PIN_INVERTED);

    pwm_init(&m_pwm2, REAR_RIGHT_A, REAR_LEFT_B,
                      RR_B1 | NRF_DRV_PWM_PIN_INVERTED,
                      RL_A2 | NRF_DRV_PWM_PIN_INVERTED);
}

// not A1B1, A2B2

void drive_backward()
{
  pwm_init(&m_pwm0, FRONT_LEFT_A, FRONT_LEFT_B,
                      FL_A1 | NRF_DRV_PWM_PIN_INVERTED,
                      FL_B1 | NRF_DRV_PWM_PIN_INVERTED);

  pwm_init(&m_pwm1, FRONT_RIGHT_A, FRONT_RIGHT_B,
                      FR_A2 | NRF_DRV_PWM_PIN_INVERTED,
                      FR_B2 | NRF_DRV_PWM_PIN_INVERTED);

  pwm_init(&m_pwm2, REAR_RIGHT_A, REAR_LEFT_B,
                      RR_B2 | NRF_DRV_PWM_PIN_INVERTED,
                      RL_A1 | NRF_DRV_PWM_PIN_INVERTED);
}

void turn_left()
{
  pwm_init(&m_pwm0, FRONT_LEFT_A, FRONT_LEFT_B,
                      FL_A1 | NRF_DRV_PWM_PIN_INVERTED,
                      FL_B1 | NRF_DRV_PWM_PIN_INVERTED);

  pwm_init(&m_pwm1, FRONT_RIGHT_A, FRONT_RIGHT_B,
                      FR_A2 | NRF_DRV_PWM_PIN_INVERTED,
                      FR_B2 | NRF_DRV_PWM_PIN_INVERTED);

  pwm_init(&m_pwm2, REAR_RIGHT_A, REAR_LEFT_B,
                      RR_B1 | NRF_DRV_PWM_PIN_INVERTED,
                      RL_A1 | NRF_DRV_PWM_PIN_INVERTED);
}

void turn_right()
{
  pwm_init(&m_pwm0, FRONT_LEFT_A, FRONT_LEFT_B,
                      FL_A2 | NRF_DRV_PWM_PIN_INVERTED,
                      FL_B2 | NRF_DRV_PWM_PIN_INVERTED);

  pwm_init(&m_pwm1, FR_A2, FR_B1, FRONT_RIGHT_A, FRONT_RIGHT_B);

  pwm_init(&m_pwm2, REAR_RIGHT_A, REAR_LEFT_B,
                      RR_B2 | NRF_DRV_PWM_PIN_INVERTED,
                      RL_A2 | NRF_DRV_PWM_PIN_INVERTED);
}

// BLUETOOTH HANDLERS
char * ble_str_forward = "FORWARD";
char * ble_str_backward = "BACKWARD";
char * ble_str_stop = "STOP";
char * ble_str_left = "TURN LEFT";
char * ble_str_right = "TURN RIGHT";

void move(char * msg_buffer) {
   printf("CHATTER Received: %s", msg_buffer); 
   printf("\n");

   if (strcmp(msg_buffer, ble_str_forward) == 0) { 
       printf("Moving forward...\n"); 

      drive_forward();

      pwm_update_duty_cycle(0);

      for (int i = 0; i < 50; i++) {
        if (i == 50) {
          pwm_update_duty_cycle(100);
        }
      }

      pwm_update_duty_cycle(0);
   } else if (strcmp(msg_buffer, ble_str_backward) == 0) { 
       printf("Moving backward...\n"); 

       drive_backward(); // not working on right bogie :()
       pwm_update_duty_cycle(0);

   } else if (strcmp(msg_buffer, ble_str_left) == 0) { 
       printf("Moving left...\n"); 

       turn_left();
       pwm_update_duty_cycle(0);

   } else if (strcmp(msg_buffer, ble_str_right) == 0) { 
       printf("Moving right...\n"); 

       turn_right();
       pwm_update_duty_cycle(0);

   } else if (strcmp(msg_buffer, ble_str_stop) == 0) { 
       printf("Stopping...\n"); 

   } else {
       printf("Action not defined. \n"); 
   }   
}

int main(void) {
  printf("Initializing\n");
  ret_code_t error_code = NRF_SUCCESS;

  // initialize RTT library
  error_code = NRF_LOG_INIT(NULL);
  APP_ERROR_CHECK(error_code);
  NRF_LOG_DEFAULT_BACKENDS_INIT();
  printf("Log initialized!\n");

  // Start clock for accurate frequencies
  NRF_CLOCK->TASKS_HFCLKSTART = 1;
  // Wait for clock to start
  while(NRF_CLOCK->EVENTS_HFCLKSTARTED == 0);

  // drive_forward();

  float front_close = 0.2;
  float side_close = 1;//0.2;
  float max_speed = 100;
  float turning_speed = max_speed/2;

  // configure initial state
  robot_state_t state = OFF;
  bool button_pressed;
  bool obs_detected;
  bool obs_avoided;
  bool in_tunnel;

  bool turn_right;
  float side_diff;
  int back_up_time = 20;
  int turning_time = 20;
  int back_up_counter;
  int turning_counter;
  C5Sensors_t sensors = {0};

  init_ble_rc(move);

  // loop forever, running state machine
  while (1) {
    // read sensors from robot
    c5_read_sensors(&sensors);
    button_pressed = c5_is_button_pressed(&sensors);
    obs_detected = c5_obstacle_detected(&sensors, &turn_right);
    obs_avoided = !(obs_detected) && c5_obstacle_avoided(&sensors, front_close);
    // nrf_delay_ms(1000);
    in_tunnel = c5_inside_tunnel(&sensors, side_close);

    // delay before continuing
    // Note: removing this delay will make responses quicker, but will result
    //  in printf's in this loop breaking JTAG
    // nrf_delay_ms(1);
    // pwm_update_duty_cycle(0);


    // handle states
    switch(state) {
      case OFF: {
        // transition logic
        if (button_pressed) {
          state = TELEOP;
          printf("TELEOP\n");
        } else {
          // perform state-specific actions here
          c5_stop();
          state = OFF;
        }
        break; // each case needs to end with break!
      }

      case TELEOP: {
        // transition logic
        if (button_pressed) {
          state = OFF;
        } else if (obs_detected) {
          c5_set_speeds(-max_speed, -max_speed);
          state = AVOID;
          printf("AVOID\n");
          back_up_counter = 0;
        } else if (in_tunnel) {
          state = TUNNEL;
          printf("TUNNEL\n");
        } else {
          // perform state-specific actions here
          c5_set_speeds(max_speed, max_speed);
          state = TELEOP;
        }
        break; // each case needs to end with break!
      }

      case AVOID: {
        // transition logic

        printf("obstacle_avoided %d\n", obs_avoided);
        printf("back_up_counter %d\n", back_up_counter);
        printf("turning_counter %d\n", turning_counter);
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
            c5_set_speeds(-max_speed, -max_speed);
          } else {
            if (obs_avoided) {
              turning_counter = turning_counter + 1;
            } else {
              turning_counter = 0;
            }
            if (turn_right) {
              c5_set_speeds(turning_speed, -turning_speed);
            } else {
              c5_set_speeds(-turning_speed, turning_speed);
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
          c5_set_speeds(-max_speed, -max_speed);
          state = AVOID;
          printf("AVOID\n");
          back_up_counter = 0;
        } else if (! in_tunnel) {
          state = TELEOP;
          printf("TELEOP\n");
        } else {
          // perform state-specific actions here

          side_diff = c5_us_diff(&sensors);
          //printf("Cliff diff: %lf\n", front_diff);

          c5_set_speeds(max_speed/2 * (1 + side_diff / side_close), max_speed/2 * (1 - side_diff / side_close));
          
          state = TUNNEL;
        }
        break; // each case needs to end with break!

      }
    }
    // nrf_delay_ms(1000);
  }
}


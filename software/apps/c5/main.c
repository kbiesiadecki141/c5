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
#include "gpio.h"

#include "c5Functions.h"

#define BUMPER 27

bool pwm_initialized = false;

void read_bump() {
  if (gpio_read(BUMPER)) {
    printf("HI\n");
    drive_backward();
    nrf_delay_ms(1000);    
    stop_driving();
  } 
}

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
    nrf_drv_pwm_simple_playback(&m_pwm0, &seq, 3, NRF_DRV_PWM_FLAG_LOOP);
    nrf_drv_pwm_simple_playback(&m_pwm1, &seq, 3, NRF_DRV_PWM_FLAG_LOOP);
    nrf_drv_pwm_simple_playback(&m_pwm2, &seq, 3, NRF_DRV_PWM_FLAG_LOOP);
}
void pwm_init(nrf_drv_pwm_t * m_pwm, int one, int two, int three, int four)
{
    if (pwm_initialized) {
      printf("Uninitializing pwm instance \n");
      nrf_drv_pwm_uninit(m_pwm);
    }

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
    printf("initializing pwm instance \n");
    APP_ERROR_CHECK(nrf_drv_pwm_init(m_pwm, &config, NULL));
}

void rotate_clockwise(int pin1, int pin2) {
    printf("Rotate clockwise\n");
   gpio_set(pin1); 
   gpio_clear(pin2); 
}

void rotate_counterclockwise(int pin1, int pin2) {
    printf("Rotate counterclockwise\n");
   gpio_clear(pin1); 
   gpio_set(pin2); 
}

void drive_forward()
{
    printf("Drive forward\n");
    gpio_clear(FL_A1); // Rear bogie - we should switch this
    gpio_set(FL_A2);   // CLOCKWISE

    gpio_clear(FL_B1); // front bogie
    gpio_set(FL_B2);
    // -------------------
    gpio_set(FR_A1);  // COUNTERCLOCKWISE
    gpio_clear(FR_A2);

    gpio_set(FR_B1);
    gpio_clear(FR_B2);

    
    // -------------------
    gpio_clear(RL_A1);
    gpio_set(RL_A2);

    gpio_set(RR_B1);
    gpio_clear(RR_B2);

    rotate_counterclockwise(FL_A1, FL_A2);
    rotate_counterclockwise(FL_B1, FL_B2);
    rotate_clockwise(RL_A1, RL_A2);

    rotate_counterclockwise(FR_A1, FR_A2); // swapped
    rotate_counterclockwise(FR_B1, FR_B2);
    rotate_counterclockwise(RR_B1, RR_B2);
    
  pwm_init(&m_pwm0, FRONT_LEFT_A, FRONT_LEFT_B,
                    NRF_DRV_PWM_PIN_NOT_USED,
                    NRF_DRV_PWM_PIN_NOT_USED);

  pwm_init(&m_pwm1, FRONT_RIGHT_A, FRONT_RIGHT_B,
                    NRF_DRV_PWM_PIN_NOT_USED,
                    NRF_DRV_PWM_PIN_NOT_USED);

  pwm_init(&m_pwm2, REAR_RIGHT_A, REAR_LEFT_B,
                    NRF_DRV_PWM_PIN_NOT_USED,
                    NRF_DRV_PWM_PIN_NOT_USED);
    pwm_update_duty_cycle(0); // Speed control.
}

// not A1B1, A2B2

void drive_backward()
{
  printf("Drive backwards\n");
  rotate_clockwise(FL_A1, FL_A2); // back bogie
  rotate_clockwise(FL_B1, FL_B2); // front bogie
  rotate_counterclockwise(RL_A1, RL_A2); // rear left

  rotate_clockwise(FR_A1, FR_A2); // front bogie
  rotate_clockwise(FR_B1, FR_B2); // back bogie
  rotate_clockwise(RR_B1, RR_B2); // rear right

  pwm_update_duty_cycle(15); // Speed control.
}

void turn_left()
{
  printf("Drive left \n");
  // Left side.
  rotate_clockwise(FL_A1, FL_A2); // back bogie
  rotate_clockwise(FL_B1, FL_B2); // front bogie
  rotate_counterclockwise(RL_A1, RL_A2); // rear left

  // Right side.
  rotate_counterclockwise(FR_A1, FR_A2); // front bogie
  rotate_counterclockwise(FR_B1, FR_B2); // back bogie
  rotate_counterclockwise(RR_B1, RR_B2); // rear right

  pwm_update_duty_cycle(0); // Speed control.
}

void turn_right()
{
  printf("Drive right \n");
  // Left side.
  rotate_counterclockwise(FL_A1, FL_A2); // back bogie
  rotate_counterclockwise(FL_B1, FL_B2); // front bogie
  rotate_clockwise(RL_A1, RL_A2); // rear left

  // Right side.
  rotate_clockwise(FR_A1, FR_A2); // front bogie
  rotate_clockwise(FR_B1, FR_B2); // back bogie
  rotate_clockwise(RR_B1, RR_B2); // rear right

  pwm_update_duty_cycle(0); // Speed control.
}

void stop_driving() 
{
  // Left side.
  gpio_set(FL_A1);
  gpio_set(FL_A2);
  gpio_set(FR_A1);
  gpio_set(FR_A2);
  gpio_set(FL_B1);
  gpio_set(FL_B2);
  gpio_set(FR_B1);
  gpio_set(FR_B2);
  gpio_set(RL_A1);
  gpio_set(RL_A2);
  gpio_set(RR_B1);
  gpio_set(RR_B2);

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
   } else if (strcmp(msg_buffer, ble_str_backward) == 0) { 
       printf("Moving backward...\n"); 
       drive_backward(); // not working on right bogie :()
   } else if (strcmp(msg_buffer, ble_str_left) == 0) { 
       printf("Moving left...\n"); 
       turn_left();
   } else if (strcmp(msg_buffer, ble_str_right) == 0) { 
       printf("Moving right...\n"); 
       turn_right();
   } else if (strcmp(msg_buffer, ble_str_stop) == 0) { 
       printf("Stopping...\n"); 
       stop_driving();
       //pwm_update_duty_cycle(100);

   } else {
       printf("Action not defined. \n"); 
   }   
    pwm_initialized = true;
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

  gpio_config(FL_A1, OUTPUT);
  gpio_config(FL_A2, OUTPUT);
  gpio_config(FL_B1, OUTPUT);
  gpio_config(FL_B2, OUTPUT);

  gpio_config(FR_A1, OUTPUT);
  gpio_config(FR_A2, OUTPUT);
  gpio_config(FR_B1, OUTPUT);
  gpio_config(FR_B2, OUTPUT);

  gpio_config(RL_A1, OUTPUT);
  gpio_config(RL_A2, OUTPUT);
  gpio_config(RR_B1, OUTPUT);
  gpio_config(RR_B2, OUTPUT);
  gpio_config(BUMPER, INPUT);

  pwm_init(&m_pwm0, FRONT_LEFT_A, FRONT_LEFT_B,
                    NRF_DRV_PWM_PIN_NOT_USED,
                    NRF_DRV_PWM_PIN_NOT_USED);

  pwm_init(&m_pwm1, FRONT_RIGHT_A, FRONT_RIGHT_B,
                    NRF_DRV_PWM_PIN_NOT_USED,
                    NRF_DRV_PWM_PIN_NOT_USED);

  pwm_init(&m_pwm2, REAR_RIGHT_A, REAR_LEFT_B,
                    NRF_DRV_PWM_PIN_NOT_USED,
                    NRF_DRV_PWM_PIN_NOT_USED);

  pwm_initialized = true;

  // Ultrasonics
  float front_close = 0.3;
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
  int turning_time = 30;
  int back_up_counter;
  int turning_counter;
  C5Sensors_t sensors = {0};

  init_ble_rc(move);

  // loop forever, running state machine
  while (1) {
    // read sensors from robot
    read_sensors(&sensors, &front_us, &left_us, &right_us);
    // printf("left us: %lf\n", right_us);

    // read sensors from robot
    read_bump();
    c5_read_sensors(&sensors);
    button_pressed = is_button_pressed(&sensors);
    obs_detected = obstacle_detected(&sensors, &turn_right);
    obs_avoided = !(obs_detected) && obstacle_avoided(&front_us, front_close);
    // nrf_delay_ms(1000);
    in_tunnel = inside_tunnel(&left_us, &right_us, side_close);

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
          turning_counter = 0;
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
          turning_counter = 0;
        } else if (! in_tunnel) {
          state = TELEOP;
          printf("TELEOP\n");
        } else {
          // perform state-specific actions here

          side_diff = right_us - left_us;
          side_sum = right_us + left_us;//us_diff(&left_us, &right_us);
          if(fabs(side_diff) < 0.01){
            c5_set_speeds(100, 100);
          } 
          else if (side_diff > 0){
            c5_set_speeds(0, 100);
          }
          else{
            c5_set_speeds(100, 0);
          }

          // c5_set_speeds(max_speed/2 * (1 + side_diff / side_close), max_speed/2 * (1 - side_diff / side_close));
          
          state = TUNNEL;
        }
        break; // each case needs to end with break!

      }
    }
    // nrf_delay_ms(1000);
  }
}


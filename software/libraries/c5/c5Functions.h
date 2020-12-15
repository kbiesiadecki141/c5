#include "c5SensorTypes.h"
#include "c5_config.h"
#include "hcsr04_ultrasonic.h"

// STATES
typedef enum {
  OFF,
  TELEOP,
  AVOID,
  TUNNEL
} robot_state_t;

// INPUTS
void c5_read_sensors(C5Sensors_t * sensors);

// FUNCTIONS
bool c5_is_button_pressed(C5Sensors_t * sensors);

bool c5_inside_tunnel(C5Sensors_t * sensors, float side_close);

bool c5_obstacle_detected(C5Sensors_t * sensors, bool * turn_right);

bool c5_obstacle_avoided(C5Sensors_t * sensors, float front_close);

float c5_us_diff(C5Sensors_t * sensors);

// OUTPUTS
void move(char * msg_buffer);

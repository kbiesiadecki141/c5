#include "c5SensorTypes.h"
#include "hcsr04_ultrasonic.h"

#define TRIG_F 3
#define ECHO_F 4
#define TRIG_RF 3
#define ECHO_RF 4
#define TRIG_LF 3
#define ECHO_LF 4
#define TRIG_RB 3
#define ECHO_RB 4
#define TRIG_LB 3
#define ECHO_LB 4

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
void c5_set_speeds(int left, int right);

void c5_stop();
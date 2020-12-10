#include "c5SensorTypes.h"

// STATES
typedef enum {
  OFF,
  TELEOP,
  AVOID,
  TUNNEL
} robot_state_t;

// INPUTS
void read_sensors(C5Sensors_t * sensors);

// FUNCTIONS
bool in_tunnel(C5Sensors_t * sensors, float side_close);

bool obstacle_detected(C5Sensors_t * sensors);

bool obstacle_avoided(C5Sensors_t * sensors, float front_close);

float us_diff(C5Sensors_t * sensors, bool front);

// OUTPUTS
void set_wheel_speed(bool right, int wheel_num, int speed);

void set_speed(bool right, int speed);

void set_speeds(int left, int right);

void stop();
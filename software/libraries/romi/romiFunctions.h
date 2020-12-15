#include "romiSensorTypes.h"

// STATES
typedef enum {
  OFF,
  TELEOP,
  AVOID,
  TUNNEL
} robot_state_t;

// INITIALIZATION
void initialize_robot();

// INPUTS
void read_sensors(RomiSensors_t * sensors, float * front_us, float * left_us, float * right_us);

// FUNCTIONS
bool inside_tunnel(float * left_us, float * right_us, float side_close);

bool obstacle_detected(RomiSensors_t * sensors, bool * turn_right);

bool obstacle_avoided(float * front_us, float front_close);

float us_diff(float * left_us, float * right_us);

// OUTPUTS
void set_speeds(int left, int right);

void stop();
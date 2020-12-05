#include "romiSensorTypes.h"

// STATES
typedef enum {
  OFF,
  TELEOP,
  AVOID,
  TUNNEL
} robot_state_t;

// INPUTS
void read_sensors(RomiSensors_t * sensors);

// FUNCTIONS
bool in_tunnel(RomiSensors_t * sensors, float side_close);

bool obstacle_detected(RomiSensors_t * sensors, bool * turn_right);

bool obstacle_avoided(RomiSensors_t * sensors, float front_close);

float us_diff(RomiSensors_t * sensors, bool front);

// OUTPUTS
void set_speeds(int left, int right);

void stop();
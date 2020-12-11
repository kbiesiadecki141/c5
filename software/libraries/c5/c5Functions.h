#include "c5SensorTypes.h"

// STATES
typedef enum {
  OFF,
  TELEOP,
  AVOID,
  TUNNEL
} robot_state_t;

// INPUTS
static void read_sensors(C5Sensors_t * sensors);

// FUNCTIONS
static bool in_tunnel(C5Sensors_t * sensors, float side_close);

static bool obstacle_detected(C5Sensors_t * sensors);

static bool obstacle_avoided(C5Sensors_t * sensors, float front_close);

static float us_diff(C5Sensors_t * sensors, bool front);

// OUTPUTS
static void set_wheel_speed(bool right, int wheel_num, int speed);

static void set_speed(bool right, int speed);

static void set_speeds(int left, int right);

static void stop();
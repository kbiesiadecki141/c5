#include "c5SensorTypes.h"

// STATES
typedef enum {
  OFF,
  TELEOP,
  AVOID,
  TUNNEL
} robot_state_t;

// INPUTS
static float read_us(bool front, bool right, bool back);

static bool read_bump(bool right, bool front);

static bool in_tunnel(float side_close, float us_rf, float us_lf, float us_rb, float us_lb);

static void set_wheel_speed(bool right, int wheel_num, int speed);

static void set_speed(bool right, int speed);

static void set_speeds(int left, int right);

static void stop();

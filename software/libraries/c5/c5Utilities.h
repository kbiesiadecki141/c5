#include "c5SensorTypes.h"

// STATES
typedef enum {
  OFF,
  TELEOP,
  AVOID,
  TUNNEL
} robot_state_t;

// INPUTS
static float read_us(bool front, bool right, bool back){
	return 0;
}

static bool read_bump(bool right, bool front){
	return false;
}

static bool in_tunnel(float side_close, float us_rf, float us_lf, float us_rb, float us_lb){
	return (us_rf < side_close) && (us_lf < side_close) && (us_rb < side_close) && (us_lb < side_close);
}

static void set_wheel_speed(bool right, int wheel_num, int speed){
	// 0: front
	// 1: middle
	// 2: back
}

static void set_speed(bool right, int speed){
	for(int i = 0; i < 3; i++){
		set_wheel_speed(right, i, speed);
	}
}

static void set_speeds(int left, int right){
	set_speed(false, left);
	set_speed(true, right);
}

static void stop(){
	set_speeds(0, 0);
}

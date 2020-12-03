#include "c5Utilities.h"

// INPUTS
static bool read_button(){
	return false;
}

static float read_us(bool front, bool right, bool back){
	return 0;
}

static bool read_bump(bool right, bool front){
	return false;
}

static void read_sensors(C5Sensors_t * sensors){
	sensors->button = read_button();

    sensors->us_f = read_us(true, false, false);
    sensors->us_rf = read_us(false, true, false);
    sensors->us_lf = read_us(false, false, false);
    sensors->us_rb = read_us(false, true, true);
    sensors->us_lb = read_us(false, false, true);

    sensors->bump_r = read_bump(true, false);
    sensors->bump_cr = read_bump(true, true);
    sensors->bump_cl = read_bump(false, true);
    sensors->bump_l = read_bump(false, false);
}

// FUNCTIONS
static bool in_tunnel(C5Sensors_t * sensors, float side_close){
	return (sensors->us_rf < side_close) && (sensors->us_lf < side_close) && (sensors->us_rb < side_close) && (sensors->us_lb < side_close);
}

static bool obstacle_detected(C5Sensors_t * sensors){}

static bool obstacle_avoided(C5Sensors_t * sensors, float front_close){}

static float us_diff(C5Sensors_t * sensors, bool front){}

// OUTPUTS
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

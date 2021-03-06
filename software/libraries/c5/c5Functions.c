#include "c5Functions.h"

// INPUTS
static bool c5_read_button(){
	// TODO
	return false;
}

static bool c5_read_bump(bool right, bool front){
	// TODO
	return false;
}

void c5_read_sensors(C5Sensors_t * sensors){
	sensors->button = c5_read_button();

    sensors->us_f = hcsr04_read_distance(TRIG_F, ECHO_F);
    sensors->us_rf = hcsr04_read_distance(TRIG_RF, ECHO_RF);
    sensors->us_lf = hcsr04_read_distance(TRIG_LF, ECHO_LF);
    sensors->us_rb = hcsr04_read_distance(TRIG_RB, ECHO_RB);
    sensors->us_lb = hcsr04_read_distance(TRIG_LB, ECHO_LB);

    sensors->bump_r = c5_read_bump(true, false);
    sensors->bump_cr = c5_read_bump(true, true);
    sensors->bump_cl = c5_read_bump(false, true);
    sensors->bump_l = c5_read_bump(false, false);
}

// FUNCTIONS
bool c5_is_button_pressed(C5Sensors_t * sensors){
	return sensors->button;
}

bool c5_inside_tunnel(C5Sensors_t * sensors, float side_close){
	return (sensors->us_rf < side_close) && (sensors->us_lf < side_close) && (sensors->us_rb < side_close) && (sensors->us_lb < side_close);
}

bool c5_obstacle_detected(C5Sensors_t * sensors, bool * turn_right){
	return sensors->bump_r || sensors->bump_cr || sensors->bump_cl || sensors->bump_l;
}

bool c5_obstacle_avoided(C5Sensors_t * sensors, float front_close){
	return sensors->us_f >= front_close;
}

float c5_us_diff(C5Sensors_t * sensors){
	return (sensors->us_lb - sensors->us_lf) - (sensors->us_rb - sensors->us_rf);
}

// OUTPUTS
void c5_set_speeds(int left, int right){
	// TODO
}

void c5_stop(){
	c5_set_speeds(0, 0);
}

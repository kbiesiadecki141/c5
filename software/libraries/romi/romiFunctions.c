#include "romiFunctions.h"

#include "romiActuator.h"
#include "romiSensorPoll.h"
#include "hcsr04_ultrasonic.h"
#include "nrf_delay.h"

// INITIALIZATION
void initialize_robot(){
    // hcsr04_init(3, 4);
}

// INPUTS
void read_sensors(RomiSensors_t * sensors, float * front_us, float * left_us, float * right_us){
	romiSensorPoll(sensors);
	//*front_us = hcsr04_read_distance(23, 22);
	*right_us = hcsr04_read_distance(23, 22);
	*left_us = hcsr04_read_distance(3, 4);//28, 29);
	
    // nrf_delay_ms(100);
    //*right_us = hcsr04_read_distance(30, 31);
    *front_us = hcsr04_read_distance(30, 31);
}

// FUNCTIONS
bool inside_tunnel(float * left_us, float * right_us, float side_close){
	return (*left_us < side_close) && (*right_us < side_close);
}

bool obstacle_detected(RomiSensors_t * sensors, bool * turn_right){\
	if (sensors->bumps_wheelDrops.bumpLeft || sensors->bumps_wheelDrops.bumpCenter || sensors->bumps_wheelDrops.bumpRight){
		*turn_right = sensors->bumps_wheelDrops.bumpLeft;
		return true;
	}
	return false;
}

bool obstacle_avoided(float * front_us, float front_close){
    // printf("avoided %d\n", this_dist > front_close);
	return *front_us > front_close;
	// return sensors->bumps_wheelDrops.bumpCenter;
}

float us_diff(float * left_us, float * right_us){
	return *right_us - *left_us;
}

// OUTPUTS
void set_speeds(int left, int right){
	romiDriveDirect(left, right);
}

void stop(){
	set_speeds(0, 0);
}

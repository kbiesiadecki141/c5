#include "romiFunctions.h"

#include "romiActuator.h"
#include "romiSensorPoll.h"
#include "hcsr04_ultrasonic.h"

// INITIALIZATION
void initialize_robot(){
    hcsr04_init(3, 4);
}

// INPUTS
void read_sensors(RomiSensors_t * sensors){
	romiSensorPoll(sensors);
}

// FUNCTIONS
bool inside_tunnel(RomiSensors_t * sensors, float side_close){
	return sensors->cliffCenter;
}

bool obstacle_detected(RomiSensors_t * sensors, bool * turn_right){\
	if (sensors->bumps_wheelDrops.bumpLeft || sensors->bumps_wheelDrops.bumpLeft || sensors->bumps_wheelDrops.bumpRight){
		*turn_right = sensors->bumps_wheelDrops.bumpLeft;
		return true;
	}
	return false;
}

bool obstacle_avoided(RomiSensors_t * sensors, float front_close){
  	float this_dist = hcsr04_read_distance();
    printf("this_dist %f\n", this_dist);
	return this_dist > front_close;
	// return sensors->bumps_wheelDrops.bumpCenter;
}

float us_diff(RomiSensors_t * sensors, bool front){
	return sensors->cliffLeft - sensors->cliffRight;
}

// OUTPUTS
void set_speeds(int left, int right){
	romiDriveDirect(left, right);
}

void stop(){
	set_speeds(0, 0);
}

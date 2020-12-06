#include "romiFunctions.h"

#include "romiActuator.h"
#include "romiSensorPoll.h"

// INPUTS
void read_sensors(RomiSensors_t * sensors){
	romiSensorPoll(sensors);
}

// FUNCTIONS
bool inside_tunnel(RomiSensors_t * sensors, float side_close){
	return sensors->cliffCenter;
}

bool obstacle_detected(RomiSensors_t * sensors, bool * turn_right){
	if (sensors->bumps_wheelDrops.bumpLeft || sensors->bumps_wheelDrops.bumpRight){
		*turn_right = sensors->bumps_wheelDrops.bumpLeft;
	}
	return sensors->bumps_wheelDrops.bumpLeft || sensors->bumps_wheelDrops.bumpRight;
}

bool obstacle_avoided(RomiSensors_t * sensors, float front_close){
	return sensors->bumps_wheelDrops.bumpCenter;
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

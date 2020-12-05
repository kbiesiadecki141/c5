#include "romiFunctions.h"

#include "romiActuator.h"
#include "romiSensorPoll.h"

// INPUTS
void read_sensors(RomiSensors_t * sensors){
	romiSensorPoll(sensors);
}

// FUNCTIONS
bool in_tunnel(RomiSensors_t * sensors, float side_close){
	return true;
}

bool obstacle_detected(RomiSensors_t * sensors, bool * turn_right){
	*turn_right = sensors->bumps_wheelDrops.bumpLeft;
	return sensors->bumps_wheelDrops.bumpLeft || sensors->bumps_wheelDrops.bumpCenter || sensors->bumps_wheelDrops.bumpRight;
}

bool obstacle_avoided(RomiSensors_t * sensors, float front_close){
	return true;
}

float us_diff(RomiSensors_t * sensors, bool front){
	return sensors->cliffLeftSignal - sensors->cliffRightSignal;
}

// OUTPUTS
void set_speeds(int left, int right){
	romiDriveDirect(left, right);
}

void stop(){
	set_speeds(0, 0);
}

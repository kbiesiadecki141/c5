/*
	Original Author:		Jeff C. Jensen
    Rewritten By: Joshua Adkins			
	Revised:	2018-08-06
*/

#ifndef _KOBUKI_SENSORPOLL_H
#define _KOBUKI_SENSORPOLL_H
#include <stdbool.h>
#include <stdint.h>

#include "romiUART.h"
#include "romiSensorTypes.h"
#include "romiSensor.h"

// Request sensor packet from romi and wait for response
int32_t romiSensorPoll(RomiSensors_t * const	sensors);

#endif

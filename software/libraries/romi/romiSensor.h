/*	Project:	iRobot Create

	Author:		Jeff C. Jensen
				National Instruments

	Abstract:	Functions to read sensors from a data stream

	Revised:	2011-12-20
*/

#ifndef _ROMISENSOR_H
#define _ROMISENSOR_H
#include <stdbool.h>
#include <stdint.h>

#include "romiSensorTypes.h"
void romiParseSensorPacket(
	const uint8_t * packet,
	RomiSensors_t * sensors
);

#endif


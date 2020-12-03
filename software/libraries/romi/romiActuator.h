/*	Project:	iRobot Create

	Author:		Jeff C. Jensen
				National Instruments

	Abstract:	iRobot actuator commands.

	Revised:	2011-01-18
*/

#ifndef _ROMIACTUATOR_H
#define _ROMIACTUATOR_H

#include <stdbool.h>
#include <stdint.h>

#include "romiUART.h"

// Wheel speed is defined in mm/s
int32_t romiDriveDirect(
		int16_t leftWheelSpeed,
		int16_t rightWheelSpeed
);

// Speed is defined in mm/s
// Radius is defined in mm
int32_t romiDriveRadius(
		int16_t radius,
		int16_t speed
);

// Sets the PID gains on the robot wheel control to the defaults
int32_t romiSetControllerDefault(void);

// Sets the PID gains on the robot wheel control to user specified values
// P = Kp/1000
// I = Ki/1000
// D = Kd/1000
//
// Defaults:
// P = 100
// I = 0.1
// D = 2
int32_t romiSetControllerUser(uint32_t Kp, uint32_t Ki, uint32_t Kd);

// Play a sound of f = 1/(frequency * 0.00000275) with duration
// //
// This function doesn't work on the robot's current firmware version
//int32_t romiPlaySound(uint32_t frequency_hz, uint8_t duration_ms);

typedef enum {
    romiOn = 0,
    romiOff,
    romiRecharge,
    romiButton,
    romiError,
    romiCleaningStart,
    romiCleaningEnd
} RomiSound_t;

// Play a predefined sound from the above sound types
int32_t romiPlaySoundSequence(RomiSound_t sound);

// Request hardware version, firmware version and unique ID on the next data packet
int32_t romiRequestInformation(void);

// Control Output and LEDs on the Robot
// The four least significant bits of outputs controls outputs 0-3
// The four least significant bits of leds controls leds 0-3
// All power channels are enabled by this command to prevent
// accidental power down of the controller board
//
// This function doesn't work on the robot's current firmware version
//int32_t romiGeneralOutput(uint8_t outputs, uint8_t leds);

#endif /* _ROMIACTUATOR_H*/

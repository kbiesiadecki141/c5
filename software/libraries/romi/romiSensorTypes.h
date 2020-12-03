#ifndef _ROMISENSORTYPES_H
#define _ROMISENSORTYPES_H

#include <stdint.h>
#include <stdbool.h>

typedef struct{
    // Indicates if wheel is in contact with ground
	bool	wheeldropLeft;
	bool	wheeldropRight;

    // Indicates if bumper is currently pressed
	bool	bumpLeft;
	bool	bumpCenter;
	bool	bumpRight;
} RomiBumps_WheelDrops_t;

typedef struct{
	bool	B0;
	bool	B1;
	bool    B2;
} RomiButtons_t;

typedef enum {
    DISCHARGING,
    DOCKING_CHARGED,
    DOCKING_CHARGING,
    ADAPTER_CHARGED,
    ADAPTER_CHARGING
} chargerState_t;

typedef enum {
    NEAR_LEFT,
    NEAR_CENTER,
    NEAR_RIGHT,
    FAR_CENTER,
    FAR_LEFT,
    FAR_RIGHT
} DockingState_t;

typedef struct {
    DockingState_t dockingRight;
    DockingState_t dockingCenter;
    DockingState_t dockingLeft;
} RomiDocking_t;

typedef struct {
    uint8_t patch;
    uint8_t minor;
    uint8_t major;
} RomiVersion_t;

typedef struct {
    bool D0;
    bool D1;
    bool D2;
    bool D3;
    uint16_t A0;
    uint16_t A1;
    uint16_t A2;
    uint16_t A3;
} RomiInput_t;

typedef struct {
    bool userConfigured;
    uint32_t Kp;
    uint32_t Ki;
    uint32_t Kd;
} RomiGain_t;

typedef struct{
	//  bump and wheel-drop sensors.
	RomiBumps_WheelDrops_t bumps_wheelDrops;

	// Binary interpretation of the cliff sensors; true if over internal threshold
	bool cliffLeft;
	bool cliffCenter;
	bool cliffRight;

    // Raw value of cliff sensors
	uint16_t	cliffLeftSignal;
	uint16_t	cliffCenterSignal;
	uint16_t	cliffRightSignal;

	// Button Feedback
	RomiButtons_t buttons;

	// Motor Feedback
    // 16 bit unsigned roll over. Forward is positive
	uint16_t	leftWheelEncoder;
	uint16_t	rightWheelEncoder;

    // Motor current in 10ma increments
	int16_t		leftWheelCurrent;
	int16_t		rightWheelCurrent;

    // Raw PWM value applied, 8 bit signed, positive is forward
	int8_t		leftWheelPWM;
	int8_t		rightWheelPWM;

    // Indicates motor over current has triggered
	bool		leftWheelOverCurrent;
	bool		rightWheelOverCurrent;

	//TimeStamp, 16 bit unsigned in ms, rolls on overflow
	uint16_t	timeStamp;

	//Battery voltage and charging state
	uint8_t		batteryVoltage;
	chargerState_t chargingState;

	// Inertia measurement : calibrated angle of rotation around Z-axis
	int16_t angle;
	int16_t angleRate;

    // Raw values from the gyro in 0.00875 deg/s increments
    uint16_t xAxisRate;
    uint16_t yAxisRate;
    uint16_t zAxisRate;

    //Docking Position Feedback for the three IR docking sensors
    RomiDocking_t docking;

    //Hardware and software versions
    RomiVersion_t hardwareVersion;
    RomiVersion_t firmwareVersion;

    //Unique ID
    uint32_t UID[3];

    //General Purpose Input
    RomiInput_t generalInput;

    //PID controller gains
    RomiGain_t controllerGain;

} RomiSensors_t;
#endif

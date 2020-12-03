
#ifndef ROMIUTILITIES
#define ROMIUTILITIES

#include <stdint.h>

#include "romiSensorTypes.h"

int romiInit();
int romiUARTInit();
int romiUARTUnInit();
uint8_t checkSumRead(uint8_t * buffer, int length);
uint8_t checkSum(uint8_t * buffer, int length);

// checks for the state change of a button press on any of the Romi buttons
bool is_button_pressed(RomiSensors_t* sensors);

#endif

////////////////////////////////////////////////////
// 
// HC-SR04 Ultrasonic Ranging Module Example
//
// Author: Katelyn Biesiadecki
// Date: December 9, 2020
//
// Datasheet: 
//   https://cdn.sparkfun.com/datasheets/Sensors/Proximity/HCSR04.pdf
//
// Description:
//   This example will test the HC-SR04 ultrasonic
//   sensor by reading the sensor every second.
//   TO-DO: Trigger on button press? Maybe use switch to switch mode?
//
////////////////////////////////////////////////////

#include <stdbool.h>
#include <stdint.h>
#include "hcsr04_ultrasonic.h"
//#include "app_util.h"
#include "nrf_delay.h"

#define TRIG_PIN 3
#define ECHO_PIN 4

int main(void) {
  // Initialize
  printf("Ultrasonic Sensor App. \n");

  hcsr04_init(TRIG_PIN, ECHO_PIN); // trig pin, echo pin

  while(1) {
    hcsr04_read_distance();
    nrf_delay_ms(1000);
  }
}


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
#include "nrf.h"
#include "app_util.h"
#include "buckler.h"
#include "nrf_delay.h"
#include "nrf_gpio.h"

#include "hcsr04_ultrasonic.h"
#define TRIG_PIN 3
#define ECHO_PIN 4

// GPIO Interrupt Handler. 
/*
static void GPIOTE_IRQHandler(void) {
    NVIC_DisableIRQ(GPIOTE_IRQn);
    printf("Ultrasonic interrupt \n");

    // Lo To Hi Event: Ranging Starts.
    // - When the Echo pin starts to read HI, our distance measurement begins.
    if (NRF_GPIOTE->EVENTS_IN[0]) {
      NRF_GPIOTE->EVENTS_IN[0] = 0; // Clear interrupt.
      printf("Echo reads low to high \n"); 
    } 

    // Hi To Lo Event: Ranging Ends.
    // - When the Echo pin starts to read LO, our distance measurement is complete.
    if (NRF_GPIOTE->EVENTS_IN[1]) {
      NRF_GPIOTE->EVENTS_IN[1] = 0; // Clear interrupt.
      printf("Echo reads high to low \n"); 
    } 
    
    nrf_delay_ms(500);

    NVIC_EnableIRQ(GPIOTE_IRQn);
}

*/

int main(void) {
  // Initialize
  printf("Ultrasonic Sensor App. \n");

  hcsr04_init(TRIG_PIN, ECHO_PIN); // trig pin, echo pin

  while(1) {
    hcsr04_read_distance();
    nrf_delay_ms(5000);
  }
}


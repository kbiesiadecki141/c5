////////////////////////////////////////////////////
// 
// HC-SR04 Ultrasonic Ranging Module
//
// Author: Katelyn Biesiadecki
// Date: December 9, 2020
//
// Datasheet: 
//   https://cdn.sparkfun.com/datasheets/Sensors/Proximity/HCSR04.pdf
//
////////////////////////////////////////////////////

#include "app_error.h"
//#include "buckler.h"
#include "gpio.h"
#include "time.h"
#include "nrf_gpio.h"
#include "nrf_delay.h"
#include "hcsr04_ultrasonic.h"
#include "virtual_timer.h"

#define timeout 30000	// ultrasonic sensor timeout

int TRIG_PIN;
int ECHO_PIN;

ret_code_t hcsr04_init(const int trig_pin, const int echo_pin) {

   ret_code_t error_code = NRF_SUCCESS;
   TRIG_PIN = trig_pin;
   ECHO_PIN = echo_pin;
   gpio_config(TRIG_PIN, OUTPUT);
   gpio_config(ECHO_PIN, INPUT);

  // Ue config register to set up an event for transition.
  // bit 8: PSEL, bit 16: polarity, bit 1: mode (event/task/disabled)
  NRF_GPIOTE->CONFIG[0] = (ECHO_PIN << 8) | (1 << 16) | 1;  // LoToHi
  NRF_GPIOTE->CONFIG[1] = (ECHO_PIN << 8) | (2 << 16) | 1;  // HiToLo

  // Enable interrupt for Event[0] and Event[1] = 0b011.
  NRF_GPIOTE->INTENSET |= 3;

  virtual_timer_init();

  // Enable interrupt in the NVIC.
  NVIC_EnableIRQ(GPIOTE_IRQn);

  NVIC_SetPriority(GPIOTE_IRQn, 0);

   return error_code;
}

uint32_t start_time;
uint32_t end_time;
uint32_t diff_time;
float us_dist;

// GPIO Interrupt Handler. 
void GPIOTE_IRQHandler(void) {
    NVIC_DisableIRQ(GPIOTE_IRQn);
    // printf("Ultrasonic interrupt \n");

    // Lo To Hi Event: Ranging Starts.
    // - When the Echo pin starts to read HI, our distance measurement begins.
    if (NRF_GPIOTE->EVENTS_IN[0]) {
      NRF_GPIOTE->EVENTS_IN[0] = 0; // Clear interrupt.
      // printf("Echo reads low to high \n"); 
      start_time = read_timer();
      //printf("start %ld\n", start_time);
    } 

    // Hi To Lo Event: Ranging Ends.
    // - When the Echo pin starts to read LO, our distance measurement is complete.
    if (NRF_GPIOTE->EVENTS_IN[1]) {
      NRF_GPIOTE->EVENTS_IN[1] = 0; // Clear interrupt.
      // printf("Echo reads high to low \n"); 
      end_time = read_timer();
      //printf("end %ld\n", end_time);
      //printf("diff %ld\n", diff_time);
    } 
    
    nrf_delay_us(10);

    // printf("Finished interrupt \n");
    NVIC_EnableIRQ(GPIOTE_IRQn);
}


float hcsr04_read_distance(const int trig_pin, const int echo_pin) {
    hcsr04_init(trig_pin, echo_pin);
    printf("Reading distance.... \n");
    // printf("trig %i", TRIG_PIN);
    gpio_clear(TRIG_PIN); // Clear trigger pin.
    nrf_delay_us(10);
    gpio_set(TRIG_PIN);   // Set trigger pin to start measurement.
    nrf_delay_us(10);
    gpio_clear(TRIG_PIN);
    nrf_delay_us(10);
    // printf("dist %f\n", us_dist);

    __WFI();
    diff_time = end_time - start_time;
    us_dist = diff_time * 340 / 2000000.;
    return us_dist;
}


/**
	get the raw counter which corresponds to the distance from the target. calculation to actual cm or inches not implemented.
*/
/*
uint16_t getRawDuration() {
	uint16_t acount = 0, bcount = 0, ccount = 0;
	bool atimeout = false, btimeout = false, ctimeout = false;
	trig_pin = LO; 	// clear the trigger pin
	delay_us(20);
	trig_pin = HI;	// toggle the trigger pin high
	delay_us(14);
	trig_pin = LO;	// toggle the trigger pin low
	// delay_us(20);	// why? from some other Nordic nRF board's code. remove it.
	while (echo_pin)  if (acount++ > US_timeout) {atimeout = true; break;}
	while (!echo_pin) if (bcount++ > US_timeout) {btimeout = true; break;}
	while (echo_pin)  if (ccount++ > US_timeout) {ctimeout = true; break;}
	
	dprintf(("a: echo_pin HI->LO, acount %d, atimeout: %d\n", (int) acount, (int) atimeout));
	dprintf(("b: echo_pin LO->HI, bcount %d, btimeout: %d\n", (int) bcount, (int) btimeout));
	dprintf(("c: echo_pin HI->LO, ccount %d, ctimeout: %d\n", (int) ccount, (int) ctimeout));
	
	// duration = counttoUs+ tcount;	// is this code meaningful for the nRF? at what CLK speed?
	// dist = duration * 0.017;	// can i work at this backwards, by measuring the distance?

	return ccount;	
}


*/

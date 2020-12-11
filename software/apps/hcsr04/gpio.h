#pragma once
#ifndef GPIO_H_
#define GPIO_H_

#include "nrf.h"
#include "stdbool.h"

// GPIO registers 
// Note that this struct will space out the variables
// by an offset of 4 bytes.
struct GPIO_REGISTERS {
  uint32_t OUT;    // Write GPIO port, 0x504
  uint32_t OUTSET; // Set individual bits in GPIO port
  uint32_t OUTCLR; // Clear individual bits in GPIO port
  uint32_t IN;     // Read GPIO port 
  uint32_t DIR;    // Direction of GPIO pins (input-0 or output-1)
  uint32_t DIRSET; // DIR set register
  uint32_t DIRCLR; // DIR clear register
  uint32_t LATCH;  // Latch register indicating what GPIO pins have met the criteria set in 
                   // the PIN_CNF[n].SENSE registers
  uint32_t DETECTMODE;   // Select between default DETECT signal behavior and LDETECT mode
  uint32_t _UNUSED[118]; //  
  uint32_t PIN_CNF[32];  // Configuration of GPIO pins, 0x700
};

// Given on nRF52832 Product Specification, p. 113
static const uint32_t BASE_GPIO_ADDR = 0x50000000;

// This initializes the GPIO address to start at 0x50000504
static struct GPIO_REGISTERS *GPIO = (struct GPIO_REGISTERS*)(0x50000504);

typedef enum {
    INPUT = 0,
    OUTPUT,
} gpio_direction_t;

// Inputs: 
//  gpio_num - gpio number 0-31
//  dir - gpio direction (INPUT, OUTPUT)
void gpio_config(uint8_t gpio_num, gpio_direction_t dir);

// Inputs: 
//  gpio_num - gpio number 0-31
void gpio_set(uint8_t gpio_num);

// Inputs: 
//  gpio_num - gpio number 0-31
void gpio_clear(uint8_t gpio_num);

// Inputs: 
//  gpio_num - gpio number 0-31
// Returns:
//  current state of the specified gpio pin
bool gpio_read(uint8_t gpio_num);

#endif

#include "gpio.h"

// Inputs: 
//  gpio_num - gpio number 0-31
//  dir - gpio direction (INPUT, OUTPUT)
void gpio_config(uint8_t gpio_num, gpio_direction_t dir) {
  // This configures the GPIO pin to an input or output.
  uint32_t *PIN = &GPIO->PIN_CNF[gpio_num];
  *PIN ^= (-dir ^ *PIN) & (1 << 1); // Change nth bit to dir

  /*
  if (dir == OUTPUT) {
    GPIO->DIRSET |= (1 << gpio_num);
    printf("GPIO->Dirset %d \n", GPIO->DIRSET);
  } else {
    GPIO->DIRCLR |= (1 << gpio_num);
    printf("%d\n", 1 << gpio_num);
    printf("GPIO->DirCLR %d \n", GPIO->DIRCLR);
    printf("GPIO->%d \n", (GPIO->DIRCLR >> 22) & 1);
    printf("GPIO->addr%x \n", &GPIO->DIRCLR);
  }*/
}

// Set gpio_num high
// Inputs: 
//  gpio_num - gpio number 0-31
void gpio_set(uint8_t gpio_num) {
  GPIO->OUT |= (1 << gpio_num);
}

// Set gpio_num low
// Inputs: 
//  gpio_num - gpio number 0-31
void gpio_clear(uint8_t gpio_num) {
  GPIO->OUT &= ~(1 << gpio_num);
}

// Inputs: 
//  gpio_num - gpio number 0-31
bool gpio_read(uint8_t gpio_num) {
    // should return pin state
    return (GPIO->IN >> gpio_num) & 1;
}

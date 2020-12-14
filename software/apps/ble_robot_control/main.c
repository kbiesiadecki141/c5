////////////////////////////////////////////////////
//
// PROJECT C5: Chau's Cool Controlled Climbing Car
// Final Project EECS C149
//--------------------------------------------------
// Bluetooth Library Example
//
// Authors: Katelyn Biesiadecki
// Date: December 13, 2020
//
////////////////////////////////////////////////////

#include "ble_robot_control.h"


char * ble_str_forward = "FORWARD";
char * ble_str_backward = "BACKWARD";
char * ble_str_stop = "STOP";
char * ble_str_left = "TURN LEFT";
char * ble_str_right = "TURN RIGHT";

void move(char * msg_buffer) {
   printf("CHATTER Received: %s", msg_buffer); 
   printf("\n");

   if (strcmp(msg_buffer, ble_str_forward) == 0) { 
       printf("Moving forward...\n"); 

   } else if (strcmp(msg_buffer, ble_str_backward) == 0) { 
       printf("Moving backward...\n"); 

   } else if (strcmp(msg_buffer, ble_str_left) == 0) { 
       printf("Moving left...\n"); 

   } else if (strcmp(msg_buffer, ble_str_right) == 0) { 
       printf("Moving right...\n"); 

   } else if (strcmp(msg_buffer, ble_str_stop) == 0) { 
       printf("Stopping...\n"); 

   } else {
       printf("Action not defined. \n"); 
   }   
}


int main(void) {
  init_ble_rc(move);

  while(1) {
    power_manage();
  }
}


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


char * le_str_forward = "FORWARD";
char * le_str_backward = "BACKWARD";
char * le_str_stop = "STOP";
char * le_str_left = "TURN LEFT";
char * le_str_right = "TURN RIGHT";

void drive(char * msg_buffer) {
   printf("CHATTER Received: %s", msg_buffer); 
   printf("\n");

   if (strcmp(msg_buffer, le_str_forward) == 0) { 
       printf("Moving forward...\n"); 

   } else if (strcmp(msg_buffer, le_str_backward) == 0) { 
       printf("Moving backward...\n"); 

   } else if (strcmp(msg_buffer, le_str_left) == 0) { 
       printf("Moving left...\n"); 

   } else if (strcmp(msg_buffer, le_str_right) == 0) { 
       printf("Moving right...\n"); 

   } else if (strcmp(msg_buffer, le_str_stop) == 0) { 
       printf("Stopping...\n"); 

   } else {
       printf("Action not defined. \n"); 
   }   
}


int main(void) {
  init_ble_rc(drive);

  while(1) {
    power_manage();
  }
}

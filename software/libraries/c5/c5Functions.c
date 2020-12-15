#include "c5Functions.h"
#include "ble_robot_control.h"

// INPUTS
static bool c5_read_button(){
	// TODO
	return false;
}

static bool c5_read_bump(bool right, bool front){
	// TODO
	return false;
}

void c5_read_sensors(C5Sensors_t * sensors){
	sensors->button = c5_read_button();

    //sensors->us_f = hcsr04_read_distance(TRIG_F, ECHO_F);
    //sensors->us_rf = hcsr04_read_distance(TRIG_RF, ECHO_RF);
    //sensors->us_lf = hcsr04_read_distance(TRIG_LF, ECHO_LF);
    sensors->us_rb = hcsr04_read_distance(TRIG_RB, ECHO_RB);
    sensors->us_lb = hcsr04_read_distance(TRIG_LB, ECHO_LB);

    sensors->bump_r = c5_read_bump(true, false);
    sensors->bump_cr = c5_read_bump(true, true);
    sensors->bump_cl = c5_read_bump(false, true);
    sensors->bump_l = c5_read_bump(false, false);
}

// FUNCTIONS
bool c5_is_button_pressed(C5Sensors_t * sensors){
	return sensors->button;
}

bool c5_inside_tunnel(C5Sensors_t * sensors, float side_close){
	//return (sensors->us_rf < side_close) && (sensors->us_lf < side_close) && (sensors->us_rb < side_close) && (sensors->us_lb < side_close);
	//return (sensors->us_rf < side_close) && (sensors->us_lf < side_close) && (sensors->us_rb < side_close) && (sensors->us_lb < side_close);
  return false;
}

bool c5_obstacle_detected(C5Sensors_t * sensors, bool * turn_right){
	return sensors->bump_r || sensors->bump_cr || sensors->bump_cl || sensors->bump_l;
}

bool c5_obstacle_avoided(C5Sensors_t * sensors, float front_close){
	return sensors->us_f >= front_close;
}

float c5_us_diff(C5Sensors_t * sensors){
	//return (sensors->us_lb - sensors->us_lf) - (sensors->us_rb - sensors->us_rf);
  return sensors->us_lb - sensors->us_rb;
}


// // BLUETOOTH HANDLERS
// char * ble_str_forward = "FORWARD";
// char * ble_str_backward = "BACKWARD";
// char * ble_str_stop = "STOP";
// char * ble_str_left = "TURN LEFT";
// char * ble_str_right = "TURN RIGHT";

// void move(char * msg_buffer) {
//    printf("CHATTER Received: %s", msg_buffer); 
//    printf("\n");

//    if (strcmp(msg_buffer, ble_str_forward) == 0) { 
//        printf("Moving forward...\n"); 

//    } else if (strcmp(msg_buffer, ble_str_backward) == 0) { 
//        printf("Moving backward...\n"); 

//    } else if (strcmp(msg_buffer, ble_str_left) == 0) { 
//        printf("Moving left...\n"); 

//    } else if (strcmp(msg_buffer, ble_str_right) == 0) { 
//        printf("Moving right...\n"); 

//    } else if (strcmp(msg_buffer, ble_str_stop) == 0) { 
//        printf("Stopping...\n"); 

//    } else {
//        printf("Action not defined. \n"); 
//    }   
// }


////////////////////////////////////////////////////
//
// PROJECT C5: Chau's Cool Controlled Climbing Car
// Final Project EECS C149
//--------------------------------------------------
// Bluetooth Library
//
// Authors: Katelyn Biesiadecki
// Date: December 13, 2020
//
////////////////////////////////////////////////////

#include "ble_robot_control.h"


void ble_evt_write(ble_evt_t const* p_ble_evt) {
    if (simple_ble_is_char_event(p_ble_evt, &chatter_char)) {
      printf("CHATTER Received: %s", msg_buffer); 
      printf("\n");

      if (strcmp(msg_buffer, forward) == 0) { 
          printf("Moving forward...\n"); 

      } else if (strcmp(msg_buffer, backward) == 0) { 
          printf("Moving backward...\n"); 

      } else if (strcmp(msg_buffer, left) == 0) { 
          printf("Moving left...\n"); 

      } else if (strcmp(msg_buffer, right) == 0) { 
          printf("Moving right...\n"); 

      } else if (strcmp(msg_buffer, stopp) == 0) { 
          printf("Stopping...\n"); 

      } else {
          printf("Action not defined. \n"); 
      }
    }
}

void ble_evt_adv_report(ble_evt_t const* p_ble_evt) {
  // Parse incoming advertisement data.
  ble_gap_evt_adv_report_t const* adv_report = &(p_ble_evt->evt.gap_evt.params.adv_report);
}

void ble_evt_connected(ble_evt_t const* p_ble_evt) {
  printf("Connected! \n");
}

void ble_evt_disconnected(ble_evt_t const* p_ble_evt) {
  printf("Disconnected. \n");
}


void init_ble_rc() {
  printf("Bluetooth Robot Control \n");

  // Setup BLE
  simple_ble_app = simple_ble_init(&ble_config);

  // Chatter service.
  simple_ble_add_service(&chatter_service);

  simple_ble_add_characteristic(1, 1, 0, 0, // read, write, notify, vlen
      sizeof(msg_buffer), (uint8_t*)&msg_buffer,
      &chatter_service, &chatter_char);

  // Start Advertising
  simple_ble_adv_only_name();

  /*
  while(1) {
    power_manage();
  }
  */
}


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

#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "nrf.h"
#include "app_util.h"
#include "simple_ble.h"
#include "nrf_gpio.h"


// Intervals for advertising and connections
static simple_ble_config_t ble_config = {
        // c0:98:e5:49:00:00
        .platform_id       = 0x49,    // used as 4th octect in device BLE address
        .device_id         = 0x0000,
        .adv_name          = "C5", // used in advertisements if there is room
        .adv_interval      = MSEC_TO_UNITS(1000, UNIT_0_625_MS),
        .min_conn_interval = MSEC_TO_UNITS(500, UNIT_1_25_MS),
        .max_conn_interval = MSEC_TO_UNITS(1000, UNIT_1_25_MS),
};

// String passing advertisements.
static simple_ble_service_t chatter_service = {{
    .uuid128 = {0x70,0x6C,0x98,0x41,0xCE,0x43,0x14,0xA9,
                0xB5,0x4D,0x22,0x2B,0x00,0x00,0xE6,0x32}
}};

// Chatter characteristic.
static simple_ble_char_t chatter_char = {.uuid16 = 0x0001};
static char msg_buffer[12];

// Main application state
simple_ble_app_t* simple_ble_app;

void ble_evt_write(ble_evt_t const* p_ble_evt);

void ble_evt_adv_report(ble_evt_t const* p_ble_evt) ;

void ble_evt_connected(ble_evt_t const* p_ble_evt) ;

void ble_evt_disconnected(ble_evt_t const* p_ble_evt);

void init_ble_rc() ;


// Blink app
//
// Blinks an LED

#include <stdbool.h>
#include <stdint.h>
#include "nrf.h"
#include "app_util.h"
#include "simple_ble.h"
#include "nrf_gpio.h"
#include "buckler.h"

// LED array
static uint8_t LED[3] = {BUCKLER_LED0, BUCKLER_LED1, BUCKLER_LED2};


// Intervals for advertising and connections
static simple_ble_config_t ble_config = {
        // c0:98:e5:49:xx:xx
        .platform_id       = 0x49,    // used as 4th octect in device BLE address
        .device_id         = 0x0000,
        .adv_name          = "BLINKY", // used in advertisements if there is room
        .adv_interval      = MSEC_TO_UNITS(1000, UNIT_0_625_MS),
        .min_conn_interval = MSEC_TO_UNITS(500, UNIT_1_25_MS),
        .max_conn_interval = MSEC_TO_UNITS(1000, UNIT_1_25_MS),
};

static simple_ble_service_t led_service = {{
    .uuid128 = {0x70,0x6C,0x98,0x41,0xCE,0x43,0x14,0xA9,
                0xB5,0x4D,0x22,0x2B,0x89,0x10,0xE6,0x32}
}};

static simple_ble_char_t led_state_char = {.uuid16 = 0x8911};
static bool led_state = false;

static simple_ble_service_t chatter_service = {{
    .uuid128 = {0x70,0x6C,0x98,0x41,0xCE,0x43,0x14,0xA9,
                0xB5,0x4D,0x22,0x2B,0x00,0x00,0xE6,0x32}
}};

static simple_ble_char_t chatter_char = {.uuid16 = 0x0001};
static char msg_buffer[10];

/*******************************************************************************
 *   State for this application
 ******************************************************************************/
// Main application state
simple_ble_app_t* simple_ble_app;

void ble_evt_write(ble_evt_t const* p_ble_evt) {
    if (simple_ble_is_char_event(p_ble_evt, &led_state_char)) {
      printf("Got write to LED characteristic!\n");
      if (led_state) {
        printf("Turning on LED!\n");
        nrf_gpio_pin_clear(LED[1]);
      } else {
        printf("Turning off LED!\n");
        nrf_gpio_pin_set(LED[1]);
      }
    } else if (simple_ble_is_char_event(p_ble_evt, &chatter_char)) {
      printf("Received write to chatter characteristic! \n");
      printf(msg_buffer); 
      printf();
    }

}

void ble_evt_adv_report(ble_evt_t const* p_ble_evt) {
  // Parse incoming advertisement data.
  ble_gap_evt_adv_report_t const* adv_report = &(p_ble_evt->evt.gap_evt.params.adv_report);

}

void ble_evt_connected(ble_evt_t const* p_ble_evt) {
  printf("Connected! \n");
  nrf_gpio_pin_clear(LED[2]);
}

void ble_evt_disconnected(ble_evt_t const* p_ble_evt) {
  printf("Disconnected. \n");
  nrf_gpio_pin_set(LED[2]);
}


int main(void) {
  // Initialize
  printf("LED Blink App \n");

  // Setup LED GPIO
  nrf_gpio_cfg_output(LED[0]); // Red
  nrf_gpio_cfg_output(LED[1]); // Yellow
  nrf_gpio_cfg_output(LED[2]); // Blue

  // Turn LEDs off initially.
  nrf_gpio_pin_set(LED[0]);
  nrf_gpio_pin_set(LED[1]);
  nrf_gpio_pin_set(LED[2]);

  // Setup BLE
  simple_ble_app = simple_ble_init(&ble_config);

  // Toggle LED service.
  simple_ble_add_service(&led_service);

  simple_ble_add_characteristic(1, 1, 0, 0, // read, write, notify, vlen
      sizeof(led_state), (uint8_t*)&led_state,
      &led_service, &led_state_char);

  // Chatter service.
  simple_ble_add_service(&chatter_service);

  simple_ble_add_characteristic(1, 1, 0, 0, // read, write, notify, vlen
      sizeof(msg_buffer), (uint8_t*)&msg_buffer,
      &chatter_service, &chatter_char);

  // Start Advertising
  simple_ble_adv_only_name();

  while(1) {
    power_manage();
  }
}


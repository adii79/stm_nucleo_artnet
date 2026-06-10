#include "ip_config.h"

uint8_t ip_Con[4] = {
		192, 168, 0, 245
};

uint8_t NETMASK_ADDRESS_Con[4] = {
    255, 255, 255, 0
};

uint8_t GATEWAY_ADDRESS_Con[4] = {
    192, 168, 1, 1
};

uint8_t LED_LOGICAL_ONE = 15; 
uint8_t LED_LOGICAL_ZERO = 7;


// #define LEDS_PER_UNIVERSE   170
// #define UNIVERSES_PER_PIN   4
// #define LEDS_PER_PIN        (LEDS_PER_UNIVERSE * UNIVERSES_PER_PIN)  // 680
// #define NUM_PINS            5

// uint8_t const LEDS_PER_UNIVERSE  = 170;
// uint8_t const NIVERSES_PER_PIN  = 4;
// uint16_t const LEDS_PER_PIN = ( LEDS_PER_UNIVERSE *  NIVERSES_PER_PIN);
// uint8_t const NUM_PINS = 5;s 

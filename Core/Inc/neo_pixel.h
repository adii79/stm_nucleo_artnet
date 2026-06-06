#ifndef INC_NEO_PIXEL_H_
#define INC_NEO_PIXEL_H_

#include "main.h"

// structure to store an RGB color
typedef struct
{
    uint8_t r;
    uint8_t g;
    uint8_t b;
} rgb_color;

// structure for one NeoPixel LED
typedef struct
{
    uint16_t g[8];
    uint16_t r[8];
    uint16_t b[8];
} neopixel_led;

// 1.25 us - period
// logical 1 - 0.8 us high
// logical 0 - 0.4 us high
// system frequency = 80 MHz, prescaler = 3 -> timer frequency = 20 MHz
// Auto-reload value = 24 -> pwm frequency = 800 kHz
// CCR = 15 -> 0.8 us high
// CCR = 7  -> 0.4 us high
// have to be modified based on the Auto reload value
#define LED_LOGICAL_ONE  15
#define LED_LOGICAL_ZERO 7

void reset_all_leds(neopixel_led* leds, uint16_t number_leds);
void set_all_leds(neopixel_led* leds, uint16_t number_leds);
void set_specific_led(neopixel_led* leds, uint16_t led_position, rgb_color color);
void set_pattern_led(neopixel_led* leds, rgb_color *pattern, uint16_t number_leds);

#endif /* INC_NEO_PIXEL_H_ */

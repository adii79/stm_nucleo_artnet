#include "neo_pixel.h"

void reset_all_leds(neopixel_led *leds, uint16_t number_leds)
{
    for (int i = 0; i < number_leds; i++)
        for (int j = 0; j < 8; j++) {
            leds[i].g[j] = LED_LOGICAL_ZERO;
            leds[i].r[j] = LED_LOGICAL_ZERO;
            leds[i].b[j] = LED_LOGICAL_ZERO;
        }
}

void set_all_leds(neopixel_led *leds, uint16_t number_leds)
{
    for (int i = 0; i < number_leds; i++)
        for (int j = 0; j < 8; j++) {
            leds[i].g[j] = LED_LOGICAL_ONE;
            leds[i].r[j] = LED_LOGICAL_ONE;
            leds[i].b[j] = LED_LOGICAL_ONE;
        }
}

void set_specific_led(neopixel_led *leds, uint16_t led_position, rgb_color color)
{
    for (int j = 0; j < 8; j++) {
        leds[led_position].g[7 - j] = (color.g & (1 << j)) ? LED_LOGICAL_ONE : LED_LOGICAL_ZERO;
        leds[led_position].r[7 - j] = (color.r & (1 << j)) ? LED_LOGICAL_ONE : LED_LOGICAL_ZERO;
        leds[led_position].b[7 - j] = (color.b & (1 << j)) ? LED_LOGICAL_ONE : LED_LOGICAL_ZERO;
    }
}

void set_pattern_led(neopixel_led *leds, rgb_color *pattern, uint16_t number_leds)
{
    for (int i = 0; i < number_leds; i++)
        set_specific_led(leds, i, pattern[i]);
}

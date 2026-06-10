#ifndef INC_NEO_PIXEL_H_
#define INC_NEO_PIXEL_H_

#include "main.h"
#include "ip_config.h"
typedef struct {
    uint8_t r;
    uint8_t g;
    uint8_t b;
} rgb_color;

/*
 * WS2811 wire order: G → R → B
 * DMA streams struct fields in declaration order, so G must come first.
 */
typedef struct {
    uint16_t g[8];
    uint16_t r[8];
    uint16_t b[8];
} neopixel_led;

/* TIM3/TIM4: APB1×2 = 84 MHz, prescaler=3 → 21 MHz timer clock
 * Period = 26 → ~807 kHz ≈ 800 kHz
 * ONE  = 17 → ~0.81 µs high  (WS2811 spec: 0.7 µs ± 150 ns)
 * ZERO =  8 → ~0.38 µs high  (WS2811 spec: 0.35 µs ± 150 ns)
 */
// #define LED_LOGICAL_ONE  15
// #define LED_LOGICAL_ZERO 7

void reset_all_leds(neopixel_led *leds, uint16_t number_leds);
void set_all_leds(neopixel_led *leds, uint16_t number_leds);
void set_specific_led(neopixel_led *leds, uint16_t led_position, rgb_color color);
void set_pattern_led(neopixel_led *leds, rgb_color *pattern, uint16_t number_leds);

#endif /* INC_NEO_PIXEL_H_ */

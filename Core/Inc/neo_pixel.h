#ifndef INC_NEO_PIXEL_H_
#define INC_NEO_PIXEL_H_

#include <stdint.h>

typedef struct {
    uint8_t r;
    uint8_t g;
    uint8_t b;
} rgb_color;

/*
 * WS2811 timing @ 800 kHz
 * APB1 timers (TIM3,TIM4): 90 MHz timer clock, PSC=3 → 22.5 MHz, ARR=27 → 833 kHz ≈ ok
 * APB2 timers (TIM1):     180 MHz timer clock, PSC=3 → 45 MHz, ARR=55 → 818 kHz ≈ ok
 *
 * Use separate constants per timer group — see ws_output.c for actual ARR values.
 * These are the CCR values written into the DMA buffer.
 */
#define WS_ONE_APB2   37U   /* TIM1:  ~0.82 µs high of 1.22 µs period (ARR=55) */
#define WS_ZERO_APB2  18U   /* TIM1:  ~0.40 µs high                             */
#define WS_ONE_APB1   18U   /* TIM3/4: ~0.80 µs high of 1.11 µs period (ARR=24) — keep your current values */
#define WS_ZERO_APB1   7U   /* TIM3/4: ~0.31 µs high — your current values      */

/*
 * Single flat neopixel_led used for legacy 2-strip path in main.c.
 * ws_output.c uses its own interleaved buffer — does NOT use this struct.
 */
typedef struct {
    uint16_t r[8];
    uint16_t g[8];
    uint16_t b[8];
} neopixel_led;

#define LED_LOGICAL_ONE   15U
#define LED_LOGICAL_ZERO   7U

void reset_all_leds(neopixel_led *leds, uint16_t n);
void set_all_leds  (neopixel_led *leds, uint16_t n);
void set_specific_led(neopixel_led *leds, uint16_t pos, rgb_color c);
void set_pattern_led (neopixel_led *leds, rgb_color *pattern, uint16_t n);

#endif

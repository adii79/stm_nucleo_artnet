

#ifndef INC_DMX_BUFFER_H_
#define INC_DMX_BUFFER_H_

#include <stdint.h>
#include <stdbool.h>

/*
 * 24 universes (Madrix 1-based: 1..24, stored 0-based: 0..23)
 * 8 output pins × 3 universes each
 * Pin N → universes (N*3+0), (N*3+1), (N*3+2)  [0-based]
 *        → Madrix    (N*3+1), (N*3+2), (N**3+3) [1-based]
 */
#define DMX_UNIVERSE_COUNT  24U
#define DMX_LEDS_PER_UNI    170U           /* 170 × 3 = 510 ch per universe  */
#define DMX_UNIS_PER_PIN    3U
#define DMX_OUTPUT_PINS     8U
#define DMX_LEDS_PER_PIN    (DMX_LEDS_PER_UNI * DMX_UNIS_PER_PIN)  /* 510 */

typedef struct {
    uint8_t  data[512];
    uint16_t length;
    uint32_t last_update_ms;
    uint32_t packet_count;
    bool     valid;
} DMX_Universe_t;

extern DMX_Universe_t dmx_universes[DMX_UNIVERSE_COUNT];

void dmx_buffer_init(void);

#endif

#ifndef INC_DMX_BUFFER_H_
#define INC_DMX_BUFFER_H_

#include <stdint.h>
#include <stdbool.h>

#define DMX_UNIVERSE_COUNT  4
#define DMX_CHANNELS        512

typedef struct {
    uint8_t  data[DMX_CHANNELS];   // <-- Set watchpoint here in debugger
    uint16_t length;
    uint32_t last_update_ms;
    bool     valid;
    uint32_t packet_count;
} DMX_Universe_t;

extern DMX_Universe_t dmx_universes[DMX_UNIVERSE_COUNT];

// Legacy aliases used in artnet.c (keeps backward compat with your ref code)
extern DMX_Universe_t artnet_uni1;
extern DMX_Universe_t artnet_uni2;

void dmx_buffer_init(void);

#endif /* INC_DMX_BUFFER_H_ */
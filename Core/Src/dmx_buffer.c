#include "dmx_buffer.h"
#include <string.h>

// Main debug-visible array — add dmx_universes to Live Expressions in CubeIDE
DMX_Universe_t dmx_universes[DMX_UNIVERSE_COUNT];

// Legacy aliases — point to same memory as dmx_universes[0] and [1]
DMX_Universe_t artnet_uni1;
DMX_Universe_t artnet_uni2;

void dmx_buffer_init(void)
{
    memset(dmx_universes, 0, sizeof(dmx_universes));
    memset(&artnet_uni1,  0, sizeof(artnet_uni1));
    memset(&artnet_uni2,  0, sizeof(artnet_uni2));
}
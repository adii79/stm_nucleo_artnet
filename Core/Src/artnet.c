/*
 * artnet.c
 * Art-Net receiver — STM32 Nucleo-F429ZI, LwIP 2.1.2, no RTOS
 *
 * Receives ArtDMX (universe 0..3) into dmx_universes[].
 * Responds to ArtPoll so lighting consoles can discover the node.
 * Ping works automatically — LwIP ICMP handles it, no code needed here.
 */

#include "artnet.h"
#include "dmx_buffer.h"
#include "lwip/udp.h"
#include "lwip/pbuf.h"
#include "lwip/inet.h"
#include "stm32f4xx_hal.h"
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

/* =========================================================================
 * Art-Net constants
 * ========================================================================= */
#define ARTNET_PORT          6454U
#define ARTNET_HEADER_LEN    8U        // "Art-Net\0"
#define ARTNET_MIN_LEN       12U       // header(8) + opcode(2) + ver(2)

typedef enum {
    OP_POLL       = 0x2000,
    OP_POLL_REPLY = 0x2100,
    OP_DMX        = 0x5000,
    OP_SYNC       = 0x5200,
} ArtNet_OpCode_t;

/* =========================================================================
 * Packet structures
 * ========================================================================= */

// Common header — first 12 bytes of every Art-Net packet (except PollReply)
typedef struct __attribute__((__packed__)) {
    char     id[8];         // "Art-Net\0"
    uint16_t opcode;        // little-endian
    uint8_t  ver_hi;        // protocol version MSB (must be 0)
    uint8_t  ver_lo;        // protocol version LSB (must be 14)
} ArtNet_Header_t;

// ArtDMX — follows header at byte offset 12
typedef struct __attribute__((__packed__)) {
    uint8_t  sequence;      // 0 = disabled
    uint8_t  physical;      // informational
    uint16_t universe;      // little-endian, 0-based
    uint8_t  length_hi;     // big-endian length MSB
    uint8_t  length_lo;     // big-endian length LSB
    uint8_t  data[512];
} ArtNet_OpDmx_t;

// ArtPollReply — standalone packet, no common header prefix
typedef struct __attribute__((__packed__)) {
    char     id[8];             // "Art-Net\0"
    uint16_t opcode;            // 0x2100 little-endian
    uint8_t  ip_addr[4];        // node IP, MSB first
    uint16_t port;              // always 0x1936 little-endian = 6454
    uint8_t  ver_hi;            // firmware major
    uint8_t  ver_lo;            // firmware minor
    uint8_t  net_switch;        // bits 14:8 of universe
    uint8_t  sub_switch;        // bits 7:4 of universe
    uint16_t oem;               // 0xFFFF = not registered
    uint8_t  ubea_version;
    uint8_t  status1;
    uint16_t esta_manufacturer;
    char     short_name[18];
    char     long_name[64];
    char     node_report[64];
    uint8_t  num_ports_hi;
    uint8_t  num_ports_lo;
    uint8_t  port_types[4];     // 0x80 = output DMX512
    uint8_t  good_input[4];
    uint8_t  good_output_a[4];
    uint8_t  sw_in[4];
    uint8_t  sw_out[4];         // low nibble = universe within sub-net
    uint8_t  acn_priority;
    uint8_t  sw_macro;
    uint8_t  sw_remote;
    uint8_t  spare[3];
    uint8_t  style;             // 0x00 = StNode
    uint8_t  mac[6];
    uint8_t  bind_ip[4];
    uint8_t  bind_index;        // 1-based, one per advertised port
    uint8_t  status2;
    uint8_t  good_output_b[4];
    uint8_t  status3;
    uint8_t  default_uid[6];
    uint8_t  filler[15];
} ArtNet_OpPollReply_t;

/* =========================================================================
 * Module-private state
 * ========================================================================= */
static const char      ARTNET_ID[8] = "Art-Net"; // index 7 = '\0'
static struct udp_pcb *s_pcb        = NULL;
static ArtNet_OpPollReply_t s_reply; // pre-built, patched per send

// Debug counters — add these to Live Expressions in CubeIDE
volatile uint32_t artnet_rx_total    = 0;  // every UDP packet
volatile uint32_t artnet_rx_dmx      = 0;  // ArtDMX packets
volatile uint32_t artnet_rx_poll     = 0;  // ArtPoll packets
volatile uint32_t artnet_rx_unknown  = 0;  // unhandled opcodes

/* =========================================================================
 * Forward declarations
 * ========================================================================= */
static void artnet_udp_cb(void *arg, struct udp_pcb *pcb, struct pbuf *p,
                          const ip_addr_t *addr, u16_t port);
static void send_poll_reply(struct udp_pcb *pcb, const ip_addr_t *addr,
                            u16_t port, uint8_t bind_index, uint8_t sw_out);

/* =========================================================================
 * artnet_init()
 * Call once, after MX_LWIP_Init() returns.
 * ========================================================================= */
void artnet_init(void)
{
    dmx_buffer_init();

    /* --- Pre-build ArtPollReply ---------------------------------------- */
    memset(&s_reply, 0, sizeof(s_reply));

    memcpy(s_reply.id, ARTNET_ID, 8);
    s_reply.opcode = (uint16_t)OP_POLL_REPLY;   // stored little-endian

    // Node IP — must match lwipopts / MX_LWIP_Init static address
    s_reply.ip_addr[0] = 192;
    s_reply.ip_addr[1] = 168;
    s_reply.ip_addr[2] = 1;
    s_reply.ip_addr[3] = 245;

    // portNumber field in PollReply spec is ALWAYS 0x1936 (= 6454 decimal)
    s_reply.port = 0x1936;

    s_reply.ver_hi = 0x00;
    s_reply.ver_lo = 0x01;          // firmware v0.1

    s_reply.oem    = 0xFFFF;        // unregistered

    // status1: 0b11_00_00_00 = indicators normal, DHCP not configured
    s_reply.status1 = 0xC0;

    // status2: supports Art-Net 3, node has web config (informational)
    s_reply.status2 = 0x08;

    snprintf(s_reply.short_name, sizeof(s_reply.short_name), "F429ZI ArtNet");
    snprintf(s_reply.long_name,  sizeof(s_reply.long_name),
             "STM32 Nucleo-F429ZI ArtNet Receiver");
    snprintf(s_reply.node_report, sizeof(s_reply.node_report),
             "#0001 [0000] Power On");

    // Advertise 2 output ports (universe 0 and universe 1)
    s_reply.num_ports_lo  = 2;
    s_reply.port_types[0] = 0x80;   // output, DMX512
    s_reply.port_types[1] = 0x80;

    s_reply.style = 0x00;           // StNode

    // MAC matches ethernetif.c low_level_init()
    s_reply.mac[0] = 0x00;
    s_reply.mac[1] = 0x80;
    s_reply.mac[2] = 0xE1;
    s_reply.mac[3] = 0x00;
    s_reply.mac[4] = 0x00;
    s_reply.mac[5] = 0x00;

    memcpy(s_reply.bind_ip, s_reply.ip_addr, 4);

    /* --- Bind UDP PCB on port 6454 ------------------------------------ */
    if (s_pcb != NULL) return;  // already initialised

    s_pcb = udp_new_ip_type(IPADDR_TYPE_V4);
    LWIP_ASSERT("artnet: udp_new failed", s_pcb != NULL);
    if (s_pcb == NULL) return;

    // err_t err = udp_bind(s_pcb, IP4_ADDR_ANY, ARTNET_PORT);
    // LWIP_ASSERT("artnet: udp_bind failed", err == ERR_OK);

    // udp_recv(s_pcb, artnet_udp_cb, NULL);


    err_t err = udp_bind(s_pcb, IP4_ADDR_ANY, ARTNET_PORT);
    LWIP_ASSERT("artnet: udp_bind failed", err == ERR_OK);

    ip_set_option(s_pcb, SOF_BROADCAST);   // receive subnet broadcast packets

    udp_recv(s_pcb, artnet_udp_cb, NULL);


    /* In artnet_init(), after the existing s_pcb setup */

/* USER CODE BEGIN - second PCB for subnet broadcast */
static struct udp_pcb *s_bcast_pcb = NULL;

s_bcast_pcb = udp_new_ip_type(IPADDR_TYPE_V4);
if (s_bcast_pcb != NULL)
{
    // Enable SO_BROADCAST on this PCB
    ip_set_option(s_bcast_pcb, SOF_BROADCAST);
    
    err_t err2 = udp_bind(s_bcast_pcb, IP4_ADDR_ANY, ARTNET_PORT);
    if (err2 == ERR_OK) {
        udp_recv(s_bcast_pcb, artnet_udp_cb, NULL);
    }
}
/* USER CODE END */
}

/* =========================================================================
 * artnet_stop()
 * ========================================================================= */
void artnet_stop(void)
{
    if (s_pcb != NULL) {
        udp_remove(s_pcb);
        s_pcb = NULL;
    }
}

/* =========================================================================
 * artnet_udp_cb() — LwIP UDP receive callback
 * ========================================================================= */
static void artnet_udp_cb(void *arg, struct udp_pcb *pcb, struct pbuf *p,
                          const ip_addr_t *addr, u16_t port)
{
    if (p == NULL) return;
    artnet_receive(arg, pcb, p, addr, port);
    pbuf_free(p);
}

/* =========================================================================
 * artnet_receive() — packet dispatcher (public for unit testing)
 * ========================================================================= */
void artnet_receive(void *arg, struct udp_pcb *pcb, struct pbuf *p,
                    const ip_addr_t *addr, u16_t port)
{
    artnet_rx_total++;

    // Minimum sanity: must be at least 12 bytes and start with "Art-Net\0"
    if (p->tot_len < ARTNET_MIN_LEN) return;
    if (memcmp(p->payload, ARTNET_ID, 8) != 0) return;

    uint8_t *raw = (uint8_t *)p->payload;

    // Opcode: little-endian at bytes [8..9]
    uint16_t opcode = (uint16_t)(raw[8] | ((uint16_t)raw[9] << 8));

    switch (opcode)
    {
    /* ------------------------------------------------------------------
     * ArtPoll — controller is discovering nodes on the network
     * ------------------------------------------------------------------ */
    case OP_POLL:
    {
        artnet_rx_poll++;

        // Protocol version at [10..11], big-endian; must be >= 14
        uint16_t ver = (uint16_t)((raw[10] << 8) | raw[11]);
        if (ver < 14) break;

        // Send one reply per advertised port (bind_index 1 = uni0, 2 = uni1)
        send_poll_reply(pcb, addr, port, 1, 0);
        send_poll_reply(pcb, addr, port, 2, 1);
        break;
    }

    /* ------------------------------------------------------------------
     * ArtDMX — DMX data from the lighting console
     * ------------------------------------------------------------------ */
    // case OP_DMX:
    //     {
    //     // Need header(12) + DMX header(6) + at least 2 bytes of data
    //     if (p->tot_len < 18) break;

    //     // Protocol version check
    //     uint16_t ver = (uint16_t)((raw[10] << 8) | raw[11]);
    //     if (ver < 14) break;

    //     artnet_rx_dmx++;

    //     // ArtDMX payload starts at byte 12
    //     ArtNet_OpDmx_t *dmx = (ArtNet_OpDmx_t *)(raw + 12);

    //     // Universe: little-endian 16-bit
    //     uint16_t universe = dmx->universe;  // already LE on ARM

    //     // Length: big-endian, clamp to 512, force even per spec
    //     uint16_t length = (uint16_t)((dmx->length_hi << 8) | dmx->length_lo);
    //     if (length == 0)   length = 512;
    //     if (length > 512)  length = 512;
    //     if (length & 1)    length++;        // must be even

    //     // Save into dmx_universes[] — watch this in the debugger
    //     if (universe < DMX_UNIVERSE_COUNT)
    //     {
    //         DMX_Universe_t *uni = &dmx_universes[universe];

    //         memcpy(uni->data, dmx->data, length);
    //         uni->length         = length;
    //         uni->last_update_ms = HAL_GetTick();
    //         uni->valid          = true;
    //         uni->packet_count++;

    //         // Keep legacy aliases in sync
    //         if (universe == 0) {
    //             memcpy(artnet_uni1.data,  dmx->data, length);
    //             artnet_uni1.length         = length;
    //             artnet_uni1.last_update_ms = uni->last_update_ms;
    //             artnet_uni1.valid          = true;
    //             artnet_uni1.packet_count   = uni->packet_count;
    //         } else if (universe == 1) {
    //             memcpy(artnet_uni2.data,  dmx->data, length);
    //             artnet_uni2.length         = length;
    //             artnet_uni2.last_update_ms = uni->last_update_ms;
    //             artnet_uni2.valid          = true;
    //             artnet_uni2.packet_count   = uni->packet_count;
    //         }
    //     }
    //     break;
    // }

    case OP_DMX:
{
    // Need header(12) + DMX header(6) + at least 2 bytes of data
    if (p->tot_len < 18) break;

    // Protocol version check
    uint16_t ver = (uint16_t)((raw[10] << 8) | raw[11]);
    if (ver < 14) break;

    artnet_rx_dmx++;

    // ArtDMX payload starts at byte 12
    ArtNet_OpDmx_t *dmx = (ArtNet_OpDmx_t *)(raw + 12);

    // Universe: little-endian 16-bit
    uint16_t universe = dmx->universe;

    // Length: big-endian, clamp to 512, force even per spec
    uint16_t length = (uint16_t)((dmx->length_hi << 8) | dmx->length_lo);
    if (length == 0)  length = 512;
    if (length > 512) length = 512;
    if (length & 1)   length++;

    if (universe < DMX_UNIVERSE_COUNT)
    {
        DMX_Universe_t *uni = &dmx_universes[universe];

        memcpy(uni->data, dmx->data, length);
        uni->length         = length;
        uni->last_update_ms = HAL_GetTick();
        uni->valid          = true;
        uni->packet_count++;

        // Legacy aliases
        if (universe == 0) {
            memcpy(artnet_uni1.data, dmx->data, length);
            artnet_uni1.length         = length;
            artnet_uni1.last_update_ms = uni->last_update_ms;
            artnet_uni1.valid          = true;
            artnet_uni1.packet_count   = uni->packet_count;
        } else if (universe == 1) {
            memcpy(artnet_uni2.data, dmx->data, length);
            artnet_uni2.length         = length;
            artnet_uni2.last_update_ms = uni->last_update_ms;
            artnet_uni2.valid          = true;
            artnet_uni2.packet_count   = uni->packet_count;
        }

        // Debug print — visible in SWV ITM Data Console
        printf("UNI:%d CNT:%lu CH1:%3d CH2:%3d CH3:%3d CH4:%3d CH5:%3d\r\n",
               universe,
               (unsigned long)uni->packet_count,
               uni->data[0],
               uni->data[1],
               uni->data[2],
               uni->data[3],
               uni->data[4]);
    }
    break;
}

    /* ------------------------------------------------------------------
     * ArtSync — apply all universes simultaneously (double-buffer hook)
     * ------------------------------------------------------------------ */
    case OP_SYNC:
        // Swap double-buffers here if you implement them later
        break;

    default:
        artnet_rx_unknown++;
        break;
    }
}

/* =========================================================================
 * send_poll_reply() — unicast reply directly to the controller IP
 * ========================================================================= */
static void send_poll_reply(struct udp_pcb *pcb, const ip_addr_t *addr,
                            u16_t port, uint8_t bind_index, uint8_t sw_out)
{
    struct pbuf *p = pbuf_alloc(PBUF_TRANSPORT,
                                sizeof(ArtNet_OpPollReply_t),
                                PBUF_RAM);
    if (p == NULL) return;

    // Patch per-port fields before copying
    s_reply.bind_index  = bind_index;
    s_reply.sw_out[0]   = sw_out;   // universe within sub-net
    s_reply.net_switch  = 0;        // net = 0
    s_reply.sub_switch  = 0;        // sub-net = 0

    memcpy(p->payload, &s_reply, sizeof(ArtNet_OpPollReply_t));

    // Art-Net spec §7: PollReply goes to sender IP, not broadcast
    udp_sendto(pcb, p, addr, ARTNET_PORT);
    pbuf_free(p);
}
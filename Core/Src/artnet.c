/*
 * artnet.c — Art-Net receiver, 24 universes, Madrix 1-based
 * STM32 Nucleo-F429ZI + LwIP 2.1.2, no RTOS
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
#define ARTNET_PORT      6454U
#define ARTNET_MIN_LEN   12U

typedef enum {
    OP_POLL       = 0x2000,
    OP_POLL_REPLY = 0x2100,
    OP_DMX        = 0x5000,
    OP_SYNC       = 0x5200,
} ArtNet_OpCode_t;

/* =========================================================================
 * Packet structures
 * ========================================================================= */
typedef struct __attribute__((__packed__)) {
    uint8_t  sequence;
    uint8_t  physical;
    uint16_t universe;   /* little-endian, 15-bit: net(7)|sub(4)|uni(4) */
    uint8_t  length_hi;
    uint8_t  length_lo;
    uint8_t  data[512];
} ArtNet_OpDmx_t;

typedef struct __attribute__((__packed__)) {
    char     id[8];
    uint16_t opcode;
    uint8_t  ip_addr[4];
    uint16_t port;
    uint8_t  ver_hi;
    uint8_t  ver_lo;
    uint8_t  net_switch;
    uint8_t  sub_switch;
    uint16_t oem;
    uint8_t  ubea_version;
    uint8_t  status1;
    uint16_t esta_manufacturer;
    char     short_name[18];
    char     long_name[64];
    char     node_report[64];
    uint8_t  num_ports_hi;
    uint8_t  num_ports_lo;
    uint8_t  port_types[4];
    uint8_t  good_input[4];
    uint8_t  good_output_a[4];
    uint8_t  sw_in[4];
    uint8_t  sw_out[4];
    uint8_t  acn_priority;
    uint8_t  sw_macro;
    uint8_t  sw_remote;
    uint8_t  spare[3];
    uint8_t  style;
    uint8_t  mac[6];
    uint8_t  bind_ip[4];
    uint8_t  bind_index;
    uint8_t  status2;
    uint8_t  good_output_b[4];
    uint8_t  status3;
    uint8_t  default_uid[6];
    uint8_t  filler[15];
} ArtNet_OpPollReply_t;

/* =========================================================================
 * Module-private state
 * ========================================================================= */
static const char           ARTNET_ID[8] = "Art-Net";
static struct udp_pcb      *s_pcb        = NULL;
static ArtNet_OpPollReply_t s_reply;

volatile uint32_t artnet_rx_total   = 0;
volatile uint32_t artnet_rx_dmx     = 0;
volatile uint32_t artnet_rx_poll    = 0;
volatile uint32_t artnet_rx_unknown = 0;

/* =========================================================================
 * Universe mapping
 *
 * Madrix sends 1-based universe numbers (1..24).
 * We store internally 0-based (0..23).
 * Mapping: internal = madrix - 1
 *
 * ArtPoll reply layout:
 *   6 replies × 4 ports = 24 advertised ports
 *   Reply i (0..5): bind_index = i+1
 *                   sw_out[0..3] = universes i*4+0 .. i*4+3  (0-based wire)
 *
 * Art-Net universe wire encoding (15-bit):
 *   bits [3:0]  = universe (0..15) within sub-net
 *   bits [7:4]  = sub-net  (0..15)
 *   bits [14:8] = net      (0..127)
 *
 * We use net=0, sub=0, universe=0..23 → wire value = 0..23
 * Madrix must be configured: net 0, subnet 0, universe 1..24
 * (Madrix UI is 1-based for universe within subnet, so universe field
 *  shown in Madrix = internal wire value + 1 when net=sub=0)
 * ========================================================================= */

/* =========================================================================
 * Forward declarations
 * ========================================================================= */
static void artnet_udp_cb(void *arg, struct udp_pcb *pcb, struct pbuf *p,
                          const ip_addr_t *addr, u16_t port);
static void send_poll_replies(struct udp_pcb *pcb, const ip_addr_t *addr,
                              u16_t port);

/* =========================================================================
 * artnet_init()
 * ========================================================================= */
void artnet_init(void)
{
    dmx_buffer_init();

    memset(&s_reply, 0, sizeof(s_reply));
    memcpy(s_reply.id, ARTNET_ID, 8);
    s_reply.opcode = (uint16_t)OP_POLL_REPLY;

    s_reply.ip_addr[0] = 2;
    s_reply.ip_addr[1] = 0;
    s_reply.ip_addr[2] = 0;
    s_reply.ip_addr[3] = 245;

    s_reply.port   = 0x1936;
    s_reply.ver_hi = 0x00;
    s_reply.ver_lo = 0x01;
    s_reply.oem    = 0xFFFF;
    s_reply.status1 = 0xC0;
    s_reply.status2 = 0x08;
    s_reply.net_switch = 0;
    s_reply.sub_switch = 0;

    snprintf(s_reply.short_name,  sizeof(s_reply.short_name),  "F429 ArtNet x24");
    snprintf(s_reply.long_name,   sizeof(s_reply.long_name),   "STM32 F429ZI 24-Universe ArtNet Node");
    snprintf(s_reply.node_report, sizeof(s_reply.node_report), "#0001 [0000] Power On");

    /* 4 output ports per reply packet */
    s_reply.num_ports_hi = 0;
    s_reply.num_ports_lo = 4;
    s_reply.port_types[0] = 0x80;   /* output */
    s_reply.port_types[1] = 0x80;
    s_reply.port_types[2] = 0x80;
    s_reply.port_types[3] = 0x80;
    s_reply.good_output_a[0] = 0x80;
    s_reply.good_output_a[1] = 0x80;
    s_reply.good_output_a[2] = 0x80;
    s_reply.good_output_a[3] = 0x80;

    s_reply.style = 0x00;

    s_reply.mac[0] = 0x00;
    s_reply.mac[1] = 0x80;
    s_reply.mac[2] = 0xE1;
    s_reply.mac[3] = 0x00;
    s_reply.mac[4] = 0x00;
    s_reply.mac[5] = 0x01;

    memcpy(s_reply.bind_ip, s_reply.ip_addr, 4);

    if (s_pcb != NULL) return;
    s_pcb = udp_new_ip_type(IPADDR_TYPE_V4);
    if (s_pcb == NULL) return;

    udp_bind(s_pcb, IP4_ADDR_ANY, ARTNET_PORT);
    ip_set_option(s_pcb, SOF_BROADCAST);
    udp_recv(s_pcb, artnet_udp_cb, NULL);
}

/* =========================================================================
 * artnet_stop()
 * ========================================================================= */
void artnet_stop(void)
{
    if (s_pcb != NULL) { udp_remove(s_pcb); s_pcb = NULL; }
}

/* =========================================================================
 * artnet_udp_cb()
 * ========================================================================= */
static void artnet_udp_cb(void *arg, struct udp_pcb *pcb, struct pbuf *p,
                          const ip_addr_t *addr, u16_t port)
{
    if (p == NULL) return;
    artnet_receive(arg, pcb, p, addr, port);
    pbuf_free(p);
}

/* =========================================================================
 * artnet_receive()
 * ========================================================================= */
void artnet_receive(void *arg, struct udp_pcb *pcb, struct pbuf *p,
                    const ip_addr_t *addr, u16_t port)
{
    artnet_rx_total++;

    if (p->tot_len < ARTNET_MIN_LEN) return;
    if (memcmp(p->payload, ARTNET_ID, 8) != 0) return;

    uint8_t  *raw    = (uint8_t *)p->payload;
    uint16_t  opcode = (uint16_t)(raw[8] | ((uint16_t)raw[9] << 8));

    switch (opcode)
    {
    case OP_POLL:
    {
        artnet_rx_poll++;
        uint16_t ver = (uint16_t)((raw[10] << 8) | raw[11]);
        if (ver < 14) break;
        send_poll_replies(pcb, addr, port);
        break;
    }

    case OP_DMX:
    {
        if (p->tot_len < 18) break;
        uint16_t ver = (uint16_t)((raw[10] << 8) | raw[11]);
        if (ver < 14) break;

        artnet_rx_dmx++;

        ArtNet_OpDmx_t *dmx = (ArtNet_OpDmx_t *)(raw + 12);

        /* Wire universe (15-bit, little-endian) */
        uint16_t wire_uni = dmx->universe & 0x7FFF;

        /*
         * Madrix 1-based → internal 0-based
         * Madrix sends wire_uni = madrix_uni - 1 when net=sub=0
         * So internal index = wire_uni directly (already 0-based on wire).
         *
         * However Madrix UI shows "Universe 1" → wire value 0,
         * "Universe 24" → wire value 23. No adjustment needed here.
         */
        if (wire_uni >= DMX_UNIVERSE_COUNT) break;

        uint16_t length = (uint16_t)((dmx->length_hi << 8) | dmx->length_lo);
        if (length == 0)  length = 512;
        if (length > 512) length = 512;
        if (length & 1)   length++;

        DMX_Universe_t *uni = &dmx_universes[wire_uni];
        memcpy(uni->data, dmx->data, length);
        uni->length         = length;
        uni->last_update_ms = HAL_GetTick();
        uni->valid          = true;
        uni->packet_count++;

        break;
    }

    case OP_SYNC:
        break;

    default:
        artnet_rx_unknown++;
        break;
    }
}

/* =========================================================================
 * send_poll_replies()
 *
 * 6 replies × 4 ports = 24 universes advertised.
 * Reply i covers wire universes i*4 .. i*4+3  (0-based).
 * Madrix shows these as universes i*4+1 .. i*4+4 (1-based).
 * ========================================================================= */
static void send_poll_replies(struct udp_pcb *pcb, const ip_addr_t *addr,
                              u16_t port)
{
    for (uint8_t i = 0; i < 6; i++)
    {
        struct pbuf *p = pbuf_alloc(PBUF_TRANSPORT,
                                    sizeof(ArtNet_OpPollReply_t),
                                    PBUF_RAM);
        if (p == NULL) continue;

        s_reply.bind_index = i + 1;          /* 1-based bind index */

        /*
         * sw_out[k] = wire universe for port k of this reply.
         * Art-Net spec: sw_out holds the low 4 bits (universe within subnet).
         * net_switch and sub_switch are already 0.
         */
        uint8_t base = i * 4;
        s_reply.sw_out[0] = base + 0;
        s_reply.sw_out[1] = base + 1;
        s_reply.sw_out[2] = base + 2;
        s_reply.sw_out[3] = base + 3;

        memcpy(p->payload, &s_reply, sizeof(ArtNet_OpPollReply_t));
        udp_sendto(pcb, p, addr, ARTNET_PORT);
        pbuf_free(p);
    }
}

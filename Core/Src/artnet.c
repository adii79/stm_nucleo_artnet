
/*
 * artnet.c
 * Art-Net receiver — STM32 Nucleo-F429ZI, LwIP 2.1.2, no RTOS
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
    uint16_t universe;
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

// Debug counters — add all four to Live Expressions in CubeIDE
volatile uint32_t artnet_rx_total   = 0;
volatile uint32_t artnet_rx_dmx     = 0;
volatile uint32_t artnet_rx_poll    = 0;
volatile uint32_t artnet_rx_unknown = 0;

/* =========================================================================
 * Forward declarations
 * ========================================================================= */
static void artnet_udp_cb(void *arg, struct udp_pcb *pcb, struct pbuf *p,
                          const ip_addr_t *addr, u16_t port);
static void send_poll_reply(struct udp_pcb *pcb, const ip_addr_t *addr,
                            u16_t port, uint8_t bind_index, uint8_t sw_out);

/* =========================================================================
 * artnet_init()
 * ========================================================================= */
void artnet_init(void)
{
    dmx_buffer_init();

    /* --- Pre-build ArtPollReply ---------------------------------------- */
    memset(&s_reply, 0, sizeof(s_reply));

    memcpy(s_reply.id, ARTNET_ID, 8);
    s_reply.opcode = (uint16_t)OP_POLL_REPLY;

    s_reply.ip_addr[0] = 192;
    s_reply.ip_addr[1] = 168;
    s_reply.ip_addr[2] = 1;
    s_reply.ip_addr[3] = 245;

    s_reply.port   = 0x1936;        // 6454 little-endian
    s_reply.ver_hi = 0x00;
    s_reply.ver_lo = 0x01;
    s_reply.oem    = 0xFFFF;

    s_reply.status1 = 0xC0;
    s_reply.status2 = 0x08;

    s_reply.net_switch = 0;
    s_reply.sub_switch = 0;

    snprintf(s_reply.short_name,  sizeof(s_reply.short_name),  "F429ZI ArtNet");
    snprintf(s_reply.long_name,   sizeof(s_reply.long_name),   "STM32 Nucleo-F429ZI ArtNet Receiver");
    snprintf(s_reply.node_report, sizeof(s_reply.node_report), "#0001 [0000] Power On");

    // 2 output ports — universe 0 and universe 1
    s_reply.num_ports_hi  = 0;
    s_reply.num_ports_lo  = 2;
    s_reply.port_types[0] = 0x80;
    s_reply.port_types[1] = 0x80;

    s_reply.style = 0x00;           // StNode

    s_reply.mac[0] = 0x00;
    s_reply.mac[1] = 0x80;
    s_reply.mac[2] = 0xE1;
    s_reply.mac[3] = 0x00;
    s_reply.mac[4] = 0x00;
    s_reply.mac[5] = 0x00;

    memcpy(s_reply.bind_ip, s_reply.ip_addr, 4);

    /* --- Single PCB — SOF_BROADCAST handles both unicast + broadcast -- */
    if (s_pcb != NULL) return;

    s_pcb = udp_new_ip_type(IPADDR_TYPE_V4);
    LWIP_ASSERT("artnet: udp_new failed", s_pcb != NULL);
    if (s_pcb == NULL) return;

    err_t err = udp_bind(s_pcb, IP4_ADDR_ANY, ARTNET_PORT);
    LWIP_ASSERT("artnet: udp_bind failed", err == ERR_OK);

    ip_set_option(s_pcb, SOF_BROADCAST);
    udp_recv(s_pcb, artnet_udp_cb, NULL);
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
    /* ------------------------------------------------------------------
     * ArtPoll
     * ------------------------------------------------------------------ */
    case OP_POLL:
    {
        artnet_rx_poll++;

        uint16_t ver = (uint16_t)((raw[10] << 8) | raw[11]);
        if (ver < 14) break;

        // Two replies — one per port — Madrix shows single node, ports 0.0.0 and 0.0.1
        send_poll_reply(pcb, addr, port, 1, 0);
        send_poll_reply(pcb, addr, port, 2, 1);
        break;
    }

    /* ------------------------------------------------------------------
     * ArtDMX
     * ------------------------------------------------------------------ */
//    case OP_DMX:
//    {
//        if (p->tot_len < 18) break;
//
//        uint16_t ver = (uint16_t)((raw[10] << 8) | raw[11]);
//        if (ver < 14) break;
//
//        artnet_rx_dmx++;
//
//        ArtNet_OpDmx_t *dmx = (ArtNet_OpDmx_t *)(raw + 12);
//
//        uint16_t universe = dmx->universe;
//
//        uint16_t length = (uint16_t)((dmx->length_hi << 8) | dmx->length_lo);
//        if (length == 0)  length = 512;
//        if (length > 512) length = 512;
//        if (length & 1)   length++;
//
//        if (universe < DMX_UNIVERSE_COUNT)
//        {
//            DMX_Universe_t *uni = &dmx_universes[universe];
//
//            memcpy(uni->data, dmx->data, length);
//            uni->length         = length;
//            uni->last_update_ms = HAL_GetTick();
//            uni->valid          = true;
//            uni->packet_count++;
//
//            if (universe == 0) {
//                memcpy(artnet_uni1.data, dmx->data, length);
//                artnet_uni1.length         = length;
//                artnet_uni1.last_update_ms = uni->last_update_ms;
//                artnet_uni1.valid          = true;
//                artnet_uni1.packet_count   = uni->packet_count;
//            } else if (universe == 1) {
//                memcpy(artnet_uni2.data, dmx->data, length);
//                artnet_uni2.length         = length;
//                artnet_uni2.last_update_ms = uni->last_update_ms;
//                artnet_uni2.valid          = true;
//                artnet_uni2.packet_count   = uni->packet_count;
//            }
//
//            printf("UNI:%d CNT:%lu CH1:%3d CH2:%3d CH3:%3d CH4:%3d CH5:%3d\r\n",
//                   universe,
//                   (unsigned long)uni->packet_count,
//                   uni->data[0], uni->data[1], uni->data[2],
//                   uni->data[3], uni->data[4]);
//        }
//        break;
//    }
//
//    case OP_SYNC:
//        break;
//
//    default:
//        artnet_rx_unknown++;
//        break;
//    }
//}

    case OP_DMX:
    {
        if (p->tot_len < 18) break;

        uint16_t ver = (uint16_t)((raw[10] << 8) | raw[11]);
        if (ver < 14) break;

        artnet_rx_dmx++;

        ArtNet_OpDmx_t *dmx = (ArtNet_OpDmx_t *)(raw + 12);

        uint16_t universe = dmx->universe;

        uint16_t length = (uint16_t)((dmx->length_hi << 8) | dmx->length_lo);
        if (length == 0)  length = 512;
        if (length > 512) length = 512;
        if (length & 1)   length++;

        if (universe < DMX_UNIVERSE_COUNT) {
            DMX_Universe_t *uni = &dmx_universes[universe];
            memcpy(uni->data, dmx->data, length);
            uni->length         = length;
            uni->last_update_ms = HAL_GetTick();
            uni->valid          = true;
            uni->packet_count++;
        }
        break;
    }

    }
}


/* =========================================================================
 * send_poll_reply()
 * ========================================================================= */
static void send_poll_reply(struct udp_pcb *pcb, const ip_addr_t *addr,
                            u16_t port, uint8_t bind_index, uint8_t sw_out)
{
    struct pbuf *p = pbuf_alloc(PBUF_TRANSPORT,
                                sizeof(ArtNet_OpPollReply_t),
                                PBUF_RAM);
    if (p == NULL) return;

    s_reply.bind_index = bind_index;        // 1 = port0, 2 = port1
    s_reply.sw_out[0]  = sw_out & 0x0F;    // 0 or 1, low nibble only
    s_reply.net_switch = 0;
    s_reply.sub_switch = 0;

    memcpy(p->payload, &s_reply, sizeof(ArtNet_OpPollReply_t));
    udp_sendto(pcb, p, addr, ARTNET_PORT);
    pbuf_free(p);
}

/*
 * ws_output.c
 *
 * 8-output RAM-optimised streaming WS2811/WS2812 driver.
 *
 * See ws_output.h for full description, hardware mapping, and RAM budget.
 *
 * =========================================================================
 * Madrix R/G channel-swap note (preserved from original firmware):
 *   Madrix sends  R → wire G,  G → wire R  for this particular strip.
 *   The colour build function applies the same swap:
 *       out.r = dmx_data[i*3 + 1]   (Madrix G → LED R)
 *       out.g = dmx_data[i*3 + 0]   (Madrix R → LED G)
 *       out.b = dmx_data[i*3 + 2]
 *   If you use a different controller or properly calibrated mapping,
 *   change build_colors_from_universe() below.
 * =========================================================================
 */

#include "ws_output.h"
#include "neo_pixel.h"
#include "dmx_buffer.h"
#include "stm32f4xx_hal.h"
#include <string.h>
#include <stddef.h>

/* =========================================================================
 * Private types
 * ========================================================================= */

/*
 * Streaming state for one output.
 * State machine: IDLE → BURST0 → BURST1 → BURST2 → RESET → IDLE
 */
typedef enum {
    OUT_IDLE   = 0,
    OUT_BURST0,     /* DMA-ing universe 0 LEDs (0-169)   */
    OUT_BURST1,     /* DMA-ing universe 1 LEDs (170-339) */
    OUT_BURST2,     /* DMA-ing universe 2 LEDs (340-509) */
    OUT_RESET,      /* DMA-ing reset words               */
} OutState_t;

typedef struct {
    /* --- DMA buffer (one universe = 170 LEDs = 4080 CCR words) -------- */
    /* Extra 42 words for reset pulse */
    uint16_t buf[WS_BUF_TOTAL];    /* 4 122 × 2 B = 8 244 B per output   */

    /* --- Hardware handles ---------------------------------------------- */
    TIM_HandleTypeDef *htim;
    uint32_t           channel;    /* TIM_CHANNEL_x                        */

    /* --- Universe base index (first of 3 universes for this output) ---- */
    uint8_t  uni_base;             /* 0, 3, 6, 9, 12, 15, 18, 21          */

    /* --- State machine ------------------------------------------------- */
    volatile OutState_t state;
} Output_t;

/* =========================================================================
 * Timer handles (defined here; declared extern in main.c if CubeMX used)
 * If you let CubeMX own these handles, replace the definitions below with
 * extern declarations and remove the Init calls in ws_output_init().
 * ========================================================================= */
static TIM_HandleTypeDef s_htim1;
static TIM_HandleTypeDef s_htim3;
static TIM_HandleTypeDef s_htim4;

/*
 * DMA handles — NOT static so stm32f4xx_it.c can extern them via the
 * canonical names used in the ISR file.
 */
DMA_HandleTypeDef hdma_tim3_ch1;   /* TIM3 CH1 – DMA1 Stream4 CH5    */
DMA_HandleTypeDef hdma_tim3_ch2;   /* TIM3 CH2 – DMA1 Stream5 CH5    */
DMA_HandleTypeDef hdma_tim3_ch3;   /* TIM3 CH3 – DMA1 Stream7 CH5    */
DMA_HandleTypeDef hdma_tim3_ch4;   /* TIM3 CH4 – DMA1 Stream2 CH5    */
DMA_HandleTypeDef hdma_tim4_ch1;   /* TIM4 CH1 – DMA1 Stream0 CH2    */
DMA_HandleTypeDef hdma_tim4_ch2;   /* TIM4 CH2 – DMA1 Stream3 CH2    */
DMA_HandleTypeDef hdma_tim1_ch2;   /* TIM1 CH2 – DMA2 Stream2 CH6    */
DMA_HandleTypeDef hdma_tim1_ch3;   /* TIM1 CH3 – DMA2 Stream6 CH6    */

/* =========================================================================
 * Output table
 * 8 entries, one per output strip.
 * buf[] is the large DMA buffer — total: 8 × 8244 B = 65 952 B ≈ 64 KB
 * ========================================================================= */
static Output_t s_out[WS_NUM_OUTPUTS];

/* =========================================================================
 * Private helpers
 * ========================================================================= */

/* Build rgb_color array for 170 LEDs from one DMX universe.
 * Applies Madrix R↔G channel swap (see file header). */
static void build_colors(const DMX_Universe_t *uni,
                          rgb_color *out,
                          uint16_t num_leds)
{
    if (uni->valid)
    {
        for (uint16_t i = 0; i < num_leds; i++)
        {
            out[i].r = uni->data[i * 3u + 1u];   /* Madrix G → LED R     */
            out[i].g = uni->data[i * 3u + 0u];   /* Madrix R → LED G     */
            out[i].b = uni->data[i * 3u + 2u];
        }
    }
    else
    {
        memset(out, 0, num_leds * sizeof(rgb_color));
        out[0].r = 32u;   /* dim red on first LED = no-signal indicator   */
    }
}

/* Encode one universe of LEDs into an output's DMA buffer and start DMA. */
static void start_burst(Output_t *o, uint8_t universe_offset)
{
    uint8_t       uni_idx = o->uni_base + universe_offset;
    rgb_color     colors[DMX_LEDS_PER_UNIVERSE];

    build_colors(&dmx_universes[uni_idx], colors, DMX_LEDS_PER_UNIVERSE);
    ws_encode_leds(o->buf, colors, DMX_LEDS_PER_UNIVERSE);

    HAL_TIM_PWM_Start_DMA(o->htim, o->channel,
                           (uint32_t *)o->buf,
                           (uint32_t)DMX_LEDS_PER_UNIVERSE * WS_BITS_PER_LED);
}

/* Encode and send the reset-pulse words after the last universe. */
static void start_reset(Output_t *o)
{
    ws_append_reset(o->buf, WS_RESET_WORDS);
    HAL_TIM_PWM_Start_DMA(o->htim, o->channel,
                           (uint32_t *)o->buf,
                           WS_RESET_WORDS);
}

/* =========================================================================
 * Timer / DMA / GPIO initialisation helpers
 * (Self-contained — no CubeMX dependency.)
 * These functions configure the minimum registers needed for
 * DMA-driven PWM output on each pin.  They are safe to call even if
 * CubeMX already configured the same timer, as long as you call
 * ws_output_init() AFTER MX_TIMx_Init().  In that case you can
 * comment-out the HAL_TIM_PWM_Init() call and only keep the
 * DMA + GPIO parts.
 * ========================================================================= */

/*
 * Timer settings:
 *   APB1 timers (TIM3, TIM4): clock = 84 MHz
 *     Prescaler = 3 → timer clock = 21 MHz
 *     ARR = 25       → period = 25/21MHz ≈ 1.19 µs (WS2812 ≈ 1.25 µs ✓)
 *
 *   APB2 timer (TIM1): clock = 168 MHz
 *     Prescaler = 6 → timer clock = 24 MHz
 *     ARR = 30       → period = 30/24MHz = 1.25 µs  (exact!)
 *     CCR_ONE = 19, CCR_ZERO = 9  (matches WS_CCR_ONE/ZERO defined for
 *     APB1 timers — the neo_pixel encoder uses those constants.
 *     For TIM1 the equivalent duty cycles are slightly different; adjust
 *     WS_CCR_ONE/ZERO if you use TIM1 outputs exclusively.)
 */

static void init_apb1_timer(TIM_HandleTypeDef *htim, TIM_TypeDef *instance)
{
    htim->Instance               = instance;
    htim->Init.Prescaler         = 3u;
    htim->Init.CounterMode       = TIM_COUNTERMODE_UP;
    htim->Init.Period            = WS_ARR - 1u;   /* ARR = 24  (0-based)  */
    htim->Init.ClockDivision     = TIM_CLOCKDIVISION_DIV1;
    htim->Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    HAL_TIM_PWM_Init(htim);   /* ignore return — Error_Handler in MspInit  */

    TIM_OC_InitTypeDef oc = {0};
    oc.OCMode     = TIM_OCMODE_PWM1;
    oc.Pulse      = 0u;
    oc.OCPolarity = TIM_OCPOLARITY_HIGH;
    oc.OCFastMode = TIM_OCFAST_DISABLE;
    HAL_TIM_PWM_ConfigChannel(htim, &oc, TIM_CHANNEL_1);
    HAL_TIM_PWM_ConfigChannel(htim, &oc, TIM_CHANNEL_2);
    HAL_TIM_PWM_ConfigChannel(htim, &oc, TIM_CHANNEL_3);
    HAL_TIM_PWM_ConfigChannel(htim, &oc, TIM_CHANNEL_4);
}

static void init_apb2_timer_tim1(TIM_HandleTypeDef *htim)
{
    htim->Instance               = TIM1;
    htim->Init.Prescaler         = 6u;         /* 168 MHz / 7 = 24 MHz    */
    htim->Init.CounterMode       = TIM_COUNTERMODE_UP;
    htim->Init.Period            = 29u;         /* ARR=29 → 24MHz/30=800kHz*/
    htim->Init.ClockDivision     = TIM_CLOCKDIVISION_DIV1;
    htim->Init.RepetitionCounter = 0u;
    htim->Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    HAL_TIM_PWM_Init(htim);

    TIM_OC_InitTypeDef oc = {0};
    oc.OCMode       = TIM_OCMODE_PWM1;
    oc.Pulse        = 0u;
    oc.OCPolarity   = TIM_OCPOLARITY_HIGH;
    oc.OCNPolarity  = TIM_OCNPOLARITY_HIGH;
    oc.OCFastMode   = TIM_OCFAST_DISABLE;
    oc.OCIdleState  = TIM_OCIDLESTATE_RESET;
    oc.OCNIdleState = TIM_OCNIDLESTATE_RESET;
    HAL_TIM_PWM_ConfigChannel(htim, &oc, TIM_CHANNEL_2);
    HAL_TIM_PWM_ConfigChannel(htim, &oc, TIM_CHANNEL_3);

    TIM_BreakDeadTimeConfigTypeDef bdt = {0};
    bdt.AutomaticOutput = TIM_AUTOMATICOUTPUT_DISABLE;
    bdt.BreakState      = TIM_BREAK_DISABLE;
    HAL_TIMEx_ConfigBreakDeadTime(htim, &bdt);
}

/* Configure one DMA stream for TIM PWM output */
static void init_dma_stream(DMA_HandleTypeDef *hdma,
                             DMA_Stream_TypeDef *stream,
                             uint32_t channel,
                             TIM_HandleTypeDef *htim,
                             uint32_t tim_dma_id)
{
    hdma->Instance                 = stream;
    hdma->Init.Channel             = channel;
    hdma->Init.Direction           = DMA_MEMORY_TO_PERIPH;
    hdma->Init.PeriphInc           = DMA_PINC_DISABLE;
    hdma->Init.MemInc              = DMA_MINC_ENABLE;
    hdma->Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
    hdma->Init.MemDataAlignment    = DMA_MDATAALIGN_HALFWORD;
    hdma->Init.Mode                = DMA_NORMAL;
    hdma->Init.Priority            = DMA_PRIORITY_HIGH;
    hdma->Init.FIFOMode            = DMA_FIFOMODE_DISABLE;
    HAL_DMA_Init(hdma);
    __HAL_LINKDMA(htim, hdma[tim_dma_id], *hdma);
}

/* Configure GPIO pin for alternate-function PWM output */
static void init_gpio_af(GPIO_TypeDef *port, uint16_t pin, uint8_t af)
{
    GPIO_InitTypeDef g = {0};
    g.Pin       = pin;
    g.Mode      = GPIO_MODE_AF_PP;
    g.Pull      = GPIO_NOPULL;
    g.Speed     = GPIO_SPEED_FREQ_HIGH;
    g.Alternate = af;
    HAL_GPIO_Init(port, &g);
}

/* =========================================================================
 * ws_output_init()
 * ========================================================================= */
void ws_output_init(void)
{
    /* ----------------------------------------------------------------
     * Enable peripheral clocks
     * --------------------------------------------------------------- */
    __HAL_RCC_TIM1_CLK_ENABLE();
    __HAL_RCC_TIM3_CLK_ENABLE();
    __HAL_RCC_TIM4_CLK_ENABLE();
    __HAL_RCC_DMA1_CLK_ENABLE();
    __HAL_RCC_DMA2_CLK_ENABLE();

    /* GPIO clocks */
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();
    __HAL_RCC_GPIOE_CLK_ENABLE();

    /* ----------------------------------------------------------------
     * Initialise timers
     * --------------------------------------------------------------- */
    init_apb1_timer(&s_htim3, TIM3);
    init_apb1_timer(&s_htim4, TIM4);
    init_apb2_timer_tim1(&s_htim1);

    /* ----------------------------------------------------------------
     * Initialise DMA streams
     * DMA1 streams 0-7 and DMA2 streams 2,6
     * --------------------------------------------------------------- */
    /* TIM3 CH1 → DMA1 Stream4 CH5 */
    init_dma_stream(&hdma_tim3_ch1, DMA1_Stream4, DMA_CHANNEL_5,
                    &s_htim3, TIM_DMA_ID_CC1);

    /* TIM3 CH2 → DMA1 Stream5 CH5 */
    init_dma_stream(&hdma_tim3_ch2, DMA1_Stream5, DMA_CHANNEL_5,
                    &s_htim3, TIM_DMA_ID_CC2);

    /* TIM3 CH3 → DMA1 Stream7 CH5 */
    init_dma_stream(&hdma_tim3_ch3, DMA1_Stream7, DMA_CHANNEL_5,
                    &s_htim3, TIM_DMA_ID_CC3);

    /* TIM3 CH4 → DMA1 Stream2 CH5 */
    init_dma_stream(&hdma_tim3_ch4, DMA1_Stream2, DMA_CHANNEL_5,
                    &s_htim3, TIM_DMA_ID_CC4);

    /* TIM4 CH1 → DMA1 Stream0 CH2 */
    init_dma_stream(&hdma_tim4_ch1, DMA1_Stream0, DMA_CHANNEL_2,
                    &s_htim4, TIM_DMA_ID_CC1);

    /* TIM4 CH2 → DMA1 Stream3 CH2 */
    init_dma_stream(&hdma_tim4_ch2, DMA1_Stream3, DMA_CHANNEL_2,
                    &s_htim4, TIM_DMA_ID_CC2);

    /* TIM1 CH2 → DMA2 Stream2 CH6 */
    init_dma_stream(&hdma_tim1_ch2, DMA2_Stream2, DMA_CHANNEL_6,
                    &s_htim1, TIM_DMA_ID_CC2);

    /* TIM1 CH3 → DMA2 Stream6 CH6 */
    init_dma_stream(&hdma_tim1_ch3, DMA2_Stream6, DMA_CHANNEL_6,
                    &s_htim1, TIM_DMA_ID_CC3);

    /* ----------------------------------------------------------------
     * Initialise GPIO alternate functions
     * --------------------------------------------------------------- */
    /* Output 0 – TIM3 CH1 – PA6 – AF2 */
    init_gpio_af(GPIOA, GPIO_PIN_6,  GPIO_AF2_TIM3);

    /* Output 1 – TIM3 CH2 – PC7 – AF2 */
    init_gpio_af(GPIOC, GPIO_PIN_7,  GPIO_AF2_TIM3);

    /* Output 2 – TIM3 CH3 – PC8 – AF2 */
    init_gpio_af(GPIOC, GPIO_PIN_8,  GPIO_AF2_TIM3);

    /* Output 3 – TIM3 CH4 – PB1 – AF2 */
    init_gpio_af(GPIOB, GPIO_PIN_1,  GPIO_AF2_TIM3);

    /* Output 4 – TIM4 CH1 – PD12 – AF2 */
    init_gpio_af(GPIOD, GPIO_PIN_12, GPIO_AF2_TIM4);

    /* Output 5 – TIM4 CH2 – PD13 – AF2 */
    init_gpio_af(GPIOD, GPIO_PIN_13, GPIO_AF2_TIM4);

    /* Output 6 – TIM1 CH2 – PE11 – AF1 */
    init_gpio_af(GPIOE, GPIO_PIN_11, GPIO_AF1_TIM1);

    /* Output 7 – TIM1 CH3 – PE13 – AF1 */
    init_gpio_af(GPIOE, GPIO_PIN_13, GPIO_AF1_TIM1);

    /* ----------------------------------------------------------------
     * Enable DMA interrupt lines
     * --------------------------------------------------------------- */
    HAL_NVIC_SetPriority(DMA1_Stream0_IRQn, 1, 0);
    HAL_NVIC_EnableIRQ(DMA1_Stream0_IRQn);

    HAL_NVIC_SetPriority(DMA1_Stream2_IRQn, 1, 0);
    HAL_NVIC_EnableIRQ(DMA1_Stream2_IRQn);

    HAL_NVIC_SetPriority(DMA1_Stream3_IRQn, 1, 0);
    HAL_NVIC_EnableIRQ(DMA1_Stream3_IRQn);

    HAL_NVIC_SetPriority(DMA1_Stream4_IRQn, 1, 0);
    HAL_NVIC_EnableIRQ(DMA1_Stream4_IRQn);

    HAL_NVIC_SetPriority(DMA1_Stream5_IRQn, 1, 0);
    HAL_NVIC_EnableIRQ(DMA1_Stream5_IRQn);

    HAL_NVIC_SetPriority(DMA1_Stream7_IRQn, 1, 0);
    HAL_NVIC_EnableIRQ(DMA1_Stream7_IRQn);

    HAL_NVIC_SetPriority(DMA2_Stream2_IRQn, 1, 0);
    HAL_NVIC_EnableIRQ(DMA2_Stream2_IRQn);

    HAL_NVIC_SetPriority(DMA2_Stream6_IRQn, 1, 0);
    HAL_NVIC_EnableIRQ(DMA2_Stream6_IRQn);

    /* ----------------------------------------------------------------
     * Populate output table
     * --------------------------------------------------------------- */
    /*         htim        channel          uni_base */
    s_out[0].htim = &s_htim3; s_out[0].channel = TIM_CHANNEL_1; s_out[0].uni_base =  0u;
    s_out[1].htim = &s_htim3; s_out[1].channel = TIM_CHANNEL_2; s_out[1].uni_base =  3u;
    s_out[2].htim = &s_htim3; s_out[2].channel = TIM_CHANNEL_3; s_out[2].uni_base =  6u;
    s_out[3].htim = &s_htim3; s_out[3].channel = TIM_CHANNEL_4; s_out[3].uni_base =  9u;
    s_out[4].htim = &s_htim4; s_out[4].channel = TIM_CHANNEL_1; s_out[4].uni_base = 12u;
    s_out[5].htim = &s_htim4; s_out[5].channel = TIM_CHANNEL_2; s_out[5].uni_base = 15u;
    s_out[6].htim = &s_htim1; s_out[6].channel = TIM_CHANNEL_2; s_out[6].uni_base = 18u;
    s_out[7].htim = &s_htim1; s_out[7].channel = TIM_CHANNEL_3; s_out[7].uni_base = 21u;

    for (uint8_t i = 0; i < WS_NUM_OUTPUTS; i++) {
        s_out[i].state = OUT_IDLE;
        memset(s_out[i].buf, 0, sizeof(s_out[i].buf));
    }
}

/* =========================================================================
 * ws_output_update_all()
 * Called from the main loop.  Kicks the first universe burst for any
 * output that is IDLE.
 * ========================================================================= */
uint8_t ws_output_update_all(void)
{
    uint8_t started = 0u;

    for (uint8_t i = 0; i < WS_NUM_OUTPUTS; i++)
    {
        Output_t *o = &s_out[i];

        if (o->state == OUT_IDLE)
        {
            o->state = OUT_BURST0;
            start_burst(o, 0u);   /* universe 0 of this output             */
            started++;
        }
    }
    return started;
}

/* =========================================================================
 * ws_output_dma_done_callback()
 * Called from HAL_TIM_PWM_PulseFinishedCallback() for every output.
 * Advances the streaming state machine.
 * ========================================================================= */
void ws_output_dma_done_callback(TIM_HandleTypeDef *htim)
{
    for (uint8_t i = 0; i < WS_NUM_OUTPUTS; i++)
    {
        Output_t *o = &s_out[i];
        if (o->htim->Instance != htim->Instance) continue;
        if (o->htim->hdma[0] == NULL)            continue;

        /* Check this is the right channel by comparing channel number */
        uint32_t active_ch = HAL_TIM_GetActiveChannel(htim);
        uint32_t our_ch;
        switch (o->channel) {
            case TIM_CHANNEL_1: our_ch = HAL_TIM_ACTIVE_CHANNEL_1; break;
            case TIM_CHANNEL_2: our_ch = HAL_TIM_ACTIVE_CHANNEL_2; break;
            case TIM_CHANNEL_3: our_ch = HAL_TIM_ACTIVE_CHANNEL_3; break;
            case TIM_CHANNEL_4: our_ch = HAL_TIM_ACTIVE_CHANNEL_4; break;
            default:            our_ch = 0u;                        break;
        }
        if (active_ch != our_ch) continue;

        /* Stop the current DMA burst */
        HAL_TIM_PWM_Stop_DMA(o->htim, o->channel);

        /* Advance state */
        switch (o->state)
        {
        case OUT_BURST0:
            o->state = OUT_BURST1;
            start_burst(o, 1u);
            break;

        case OUT_BURST1:
            o->state = OUT_BURST2;
            start_burst(o, 2u);
            break;

        case OUT_BURST2:
            o->state = OUT_RESET;
            start_reset(o);
            break;

        case OUT_RESET:
            /* Full 510-LED frame delivered — go IDLE until next loop */
            o->state = OUT_IDLE;
            break;

        default:
            o->state = OUT_IDLE;
            break;
        }
        break;   /* found our output — done */
    }
}

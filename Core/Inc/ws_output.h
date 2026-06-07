#ifndef INC_WS_OUTPUT_H_
#define INC_WS_OUTPUT_H_

/*
 * ws_output.h
 *
 * 8-output, 510-LED-per-output, RAM-optimised WS2811/WS2812 driver.
 *
 * ┌──────────────────────────────────────────────────────────────────┐
 * │  RAM budget (DMA buffers only)                                   │
 * │                                                                  │
 * │  Strategy: STREAMING – one 170-LED (1-universe) flat buffer     │
 * │  per output, refreshed in 3 successive DMA bursts.              │
 * │                                                                  │
 * │  Per output: 1 × 170 × 24 × 2 B = 8 160 B                      │
 * │  8 outputs : 8 × 8 160           = 65 280 B ≈ 63.8 KB          │
 * │                                                                  │
 * │  Compare with NAIVE full-strip struct approach:                  │
 * │  8 × 511 × 48 B = 196 224 B ≈ 192 KB  ← exceeds F429 SRAM!    │
 * └──────────────────────────────────────────────────────────────────┘
 *
 * Streaming sequence per output
 * ──────────────────────────────
 *   Universe 0 data → encode → DMA → HAL callback → universe 1 …
 *   After universe 2 DMA complete → reset words → done.
 *   Then restart from universe 0 on next loop iteration.
 *
 * Hardware mapping
 * ────────────────
 *   Output │ Timer │ CH │ Pin  │ DMA            │ Universe range
 *   ───────┼───────┼────┼──────┼────────────────┼───────────────
 *     0    │ TIM3  │  1 │ PA6  │ DMA1 S4 CH5    │  0, 1, 2
 *     1    │ TIM3  │  2 │ PC7  │ DMA1 S5 CH5    │  3, 4, 5
 *     2    │ TIM3  │  3 │ PC8  │ DMA1 S7 CH5    │  6, 7, 8
 *     3    │ TIM3  │  4 │ PB1  │ DMA1 S2 CH5    │  9,10,11
 *     4    │ TIM4  │  1 │ PD12 │ DMA1 S0 CH2    │ 12,13,14
 *     5    │ TIM4  │  2 │ PD13 │ DMA1 S3 CH2    │ 15,16,17
 *     6    │ TIM1  │  2 │ PE11 │ DMA2 S2 CH6    │ 18,19,20
 *     7    │ TIM1  │  3 │ PE13 │ DMA2 S6 CH6    │ 21,22,23
 *
 * Notes
 * ─────
 * • TIM3 and TIM4 are on APB1 (84 MHz timer clock).
 * • TIM1 is on APB2 (168 MHz timer clock).
 *   TIM1 uses different prescaler/ARR values — see ws_output.c.
 * • All timers are configured at run-time inside ws_output_init()
 *   so you do NOT need CubeMX to touch TIM3/TIM4/TIM1 PWM settings.
 *   Only the DMA and GPIO alternate-function setup are done here.
 * • If you already have CubeMX-generated TIM3/TIM4 init, comment out
 *   the relevant timer config in ws_output_init() and call the CubeMX
 *   version instead — just make sure PRE=3, ARR=25 for APB1 timers.
 */

#include "stm32f4xx_hal.h"
#include "neo_pixel.h"      /* rgb_color, WS_* constants               */
#include "dmx_buffer.h"     /* DMX_Universe_t, dmx_universes[]         */

/* ------------------------------------------------------------------ */
#define WS_NUM_OUTPUTS      8u

/* ------------------------------------------------------------------ */
/* Call once after all clocks and DMA are initialised                  */
void ws_output_init(void);

/*
 * ws_output_update_all()
 *
 * To be called from the main loop.  For each output it checks whether
 * the previous DMA burst has finished; if so it prepares the next
 * universe chunk and kicks the DMA.
 *
 * Returns the number of outputs that were idle (0 = all busy).
 */
uint8_t ws_output_update_all(void);

/*
 * ws_output_dma_done_callback()
 *
 * Must be called from HAL_TIM_PWM_PulseFinishedCallback().
 * Handles the streaming state machine for each output.
 */
void ws_output_dma_done_callback(TIM_HandleTypeDef *htim);

#endif /* INC_WS_OUTPUT_H_ */

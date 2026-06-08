//#ifndef INC_WS_OUTPUT_H_
//#define INC_WS_OUTPUT_H_
//
//#include <stdint.h>
//
///*
// * ws_output.h — 8-pin parallel WS2811, chunked circular DMA
// *
// * Group A — TIM1 (APB2, PSC=3, ARR=55)
// *   CH1 PE9   Strip 0  Madrix universes  1, 2, 3
// *   CH2 PE11  Strip 1  Madrix universes  4, 5, 6
// *   CH3 PE13  Strip 2  Madrix universes  7, 8, 9
// *   CH4 PE14  Strip 3  Madrix universes 10,11,12
// *
// * Group B — TIM3 (APB1, PSC=3, ARR=24)
// *   CH1 PA6   Strip 4  Madrix universes 13,14,15
// *   CH2 PC7   Strip 5  Madrix universes 16,17,18
// *   CH3 PC8   Strip 6  Madrix universes 19,20,21
// *   CH4 PB1   Strip 7  Madrix universes 22,23,24
// */
//
//void WS_Output_Init(void);//
//void WS_Output_Update(void);
//
//#endif /* INC_WS_OUTPUT_H_ */




#ifndef INC_WS_OUTPUT_H_
#define INC_WS_OUTPUT_H_

#include <stdint.h>

/*
 * ws_output.h — 8-pin parallel WS2811, per-channel DMA + double-buffer
 *
 * Group A — TIM1 (APB2, PSC=3, ARR=55  → ~804 kHz)
 *   CH1 PE9   Strip 0  internal universes  0, 1, 2  (Madrix  1, 2, 3)
 *   CH2 PE11  Strip 1  internal universes  3, 4, 5  (Madrix  4, 5, 6)
 *   CH3 PE13  Strip 2  internal universes  6, 7, 8  (Madrix  7, 8, 9)
 *   CH4 PE14  Strip 3  internal universes  9,10,11  (Madrix 10,11,12)
 *
 * Group B — TIM3 (APB1, PSC=3, ARR=24  → 800 kHz)
 *   CH1 PA6   Strip 4  internal universes 12,13,14  (Madrix 13,14,15)
 *   CH2 PC7   Strip 5  internal universes 15,16,17  (Madrix 16,17,18)
 *   CH3 PC8   Strip 6  internal universes 18,19,20  (Madrix 19,20,21)
 *   CH4 PB1   Strip 7  internal universes 21,22,23  (Madrix 22,23,24)
 *
 * Each strip drives 510 LEDs (3 universes × 170 LEDs × 3 ch = 1530 DMX ch).
 * Total: 8 strips × 510 LEDs = 4080 LEDs.
 *
 * DMA requirement (set in stm32f4xx_hal_msp.c):
 *   ALL 8 DMA streams MUST use  DMA_CIRCULAR  mode.
 */

void WS_Output_Init(void);
void WS_Output_Update(void);

#endif /* INC_WS_OUTPUT_H_ */

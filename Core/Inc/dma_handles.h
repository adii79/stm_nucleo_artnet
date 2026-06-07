#ifndef INC_DMA_HANDLES_H_
#define INC_DMA_HANDLES_H_

/*
 * dma_handles.h
 *
 * Compatibility shim — keeps the CubeMX-generated stm32f4xx_hal_msp.c
 * happy while ws_output.c owns the actual DMA handle definitions.
 *
 * Problem
 * -------
 * CubeMX generated stm32f4xx_hal_msp.c uses its own naming scheme:
 *   hdma_tim1_ch1, hdma_tim1_ch4_trig_com,
 *   hdma_tim3_ch1_trig, hdma_tim3_ch4_up,
 *   hdma_tim2_ch2_ch4, hdma_tim2_up_ch3
 *
 * ws_output.c defines the handles it actually uses under shorter names:
 *   hdma_tim1_ch2, hdma_tim1_ch3,
 *   hdma_tim3_ch1, hdma_tim3_ch2, hdma_tim3_ch3, hdma_tim3_ch4,
 *   hdma_tim4_ch1, hdma_tim4_ch2
 *
 * The CubeMX names that have NO equivalent in ws_output.c (because those
 * channels/streams are not used in the 8-output design) must still be
 * declared as real variables so the linker resolves them.  We provide
 * stub definitions here.
 *
 * Usage
 * -----
 * Add  #include "dma_handles.h"  at the TOP of stm32f4xx_hal_msp.c,
 * BEFORE any other includes.  That is the ONLY file that needs it.
 *
 * Do NOT include this file anywhere else.
 */

#include "stm32f4xx_hal.h"

/* ---- Handles actually defined in ws_output.c ---- */
extern DMA_HandleTypeDef hdma_tim3_ch1;   /* replaces hdma_tim3_ch1_trig  */
extern DMA_HandleTypeDef hdma_tim3_ch2;
extern DMA_HandleTypeDef hdma_tim3_ch3;
extern DMA_HandleTypeDef hdma_tim3_ch4;   /* replaces hdma_tim3_ch4_up    */
extern DMA_HandleTypeDef hdma_tim4_ch1;
extern DMA_HandleTypeDef hdma_tim4_ch2;
extern DMA_HandleTypeDef hdma_tim1_ch2;
extern DMA_HandleTypeDef hdma_tim1_ch3;

/* ---- Alias macros: map CubeMX names → ws_output names ---- */
/* TIM3 */
#define hdma_tim3_ch1_trig   hdma_tim3_ch1
#define hdma_tim3_ch4_up     hdma_tim3_ch4

/* ---- Stub variables for CubeMX handles with NO equivalent ----
 *
 * These channels (TIM1 CH1/CH4, TIM2 CH2/CH3/CH4/UP) are initialised
 * by HAL_TIM_PWM_MspInit / HAL_TIM_Base_MspInit in the CubeMX MSP file
 * but are never started by the application.  Declaring them here
 * satisfies the linker; the MSP code will init them harmlessly.
 *
 * If you later need those channels, move the definitions to ws_output.c
 * and remove them from here.
 */
extern DMA_HandleTypeDef hdma_tim1_ch1;
extern DMA_HandleTypeDef hdma_tim1_ch4_trig_com;
extern DMA_HandleTypeDef hdma_tim2_ch2_ch4;
extern DMA_HandleTypeDef hdma_tim2_up_ch3;

#endif /* INC_DMA_HANDLES_H_ */

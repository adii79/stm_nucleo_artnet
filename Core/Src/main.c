/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "lwip.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <string.h>
#include "artnet.h"
#include "dmx_buffer.h"
#include "neo_pixel.h"
// #include "lwip/netif.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
// extern struct netif gnetif;  
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
////#define NUMBER_OF_LEDS  12
//#define NUMBER_OF_LEDS  170
//
////neopixel_led leds1[NUMBER_OF_LEDS + 1];   // Strip 0 → TIM3 CH1 (PC6)
////neopixel_led leds2[NUMBER_OF_LEDS + 1];   // Strip 1 → TIM4 CH1 (PB6)
//
//neopixel_led leds[8][NUMBER_OF_LEDS + 1];
//
////static rgb_color dmx_colors1[NUMBER_OF_LEDS];
////static rgb_color dmx_colors2[NUMBER_OF_LEDS];
//
//static rgb_color dmx_colors[8][NUMBER_OF_LEDS];
//
////static volatile uint8_t dma_busy1 = 0;
////static volatile uint8_t dma_busy2 = 0;
//
//static volatile uint8_t dma_busy[8] = {0};


//#define LEDS_PER_UNIVERSE   170   // 170 × 3 = 510 channels
//#define UNIVERSES_PER_PIN   3
//#define LEDS_PER_PIN        (LEDS_PER_UNIVERSE * UNIVERSES_PER_PIN)  // 510
//
//#define NUM_PINS            6
//
//// 6 strips, each holds 510 LEDs + 1 reset slot
//neopixel_led leds[NUM_PINS][LEDS_PER_PIN + 1];
//
//// Color staging buffers, one per pin (3 universes concatenated)
////static rgb_color dmx_colors[NUM_PINS][LEDS_PER_PIN];
//static rgb_color dmx_colors[NUM_PINS][LEDS_PER_PIN] __attribute__((section(".ccmram")));
//static volatile uint8_t dma_busy[NUM_PINS];


//#define LEDS_PER_UNIVERSE   170
//#define UNIVERSES_PER_PIN   3
//#define LEDS_PER_PIN        (LEDS_PER_UNIVERSE * UNIVERSES_PER_PIN)
//#define NUM_PINS            6

#define LEDS_PER_UNIVERSE   10
#define UNIVERSES_PER_PIN   4
#define LEDS_PER_PIN        (LEDS_PER_UNIVERSE * UNIVERSES_PER_PIN)  // 680
#define NUM_PINS            5

/* DMA reads this directly — MUST stay in main SRAM, never move to CCM */
neopixel_led leds[NUM_PINS][LEDS_PER_PIN + 1];

/* CPU-only buffers — placed in CCM to free up SRAM */
static rgb_color dmx_colors[NUM_PINS][LEDS_PER_PIN] __attribute__((section(".ccmram")));

static volatile uint8_t dma_busy[NUM_PINS];



/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
TIM_HandleTypeDef htim1;
TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim3;
TIM_HandleTypeDef htim4;
TIM_HandleTypeDef htim5;
DMA_HandleTypeDef hdma_tim1_ch1;
DMA_HandleTypeDef hdma_tim1_ch4_trig_com;
DMA_HandleTypeDef hdma_tim1_up;
DMA_HandleTypeDef hdma_tim2_up_ch3;
DMA_HandleTypeDef hdma_tim3_ch1_trig;
DMA_HandleTypeDef hdma_tim3_ch2;
DMA_HandleTypeDef hdma_tim3_ch3;
DMA_HandleTypeDef hdma_tim3_ch4_up;
DMA_HandleTypeDef hdma_tim4_ch1;
DMA_HandleTypeDef hdma_tim4_ch2;
DMA_HandleTypeDef hdma_tim4_up;

UART_HandleTypeDef huart3;

PCD_HandleTypeDef hpcd_USB_OTG_FS;

/* USER CODE BEGIN PV */
//neopixel_led leds[NUMBER_OF_LEDS + 1];
//static rgb_color dmx_colors[NUMBER_OF_LEDS];
//static uint8_t dma_busy1 = 0;                // guard flag
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_USART3_UART_Init(void);
static void MX_USB_OTG_FS_PCD_Init(void);
static void MX_TIM3_Init(void);
static void MX_TIM4_Init(void);
static void MX_TIM5_Init(void);
static void MX_TIM1_Init(void);
static void MX_TIM2_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
//static void build_colors_from_universe(const DMX_Universe_t *uni,
//                                       rgb_color *out,
//                                       uint16_t num_leds)
//{
//    if (uni->valid) {
//        uint8_t master_r = uni->data[0];
//        uint8_t master_g = uni->data[1];
//        uint8_t master_b = uni->data[2];
//
//        for (uint16_t i = 0; i < num_leds; i++) {
//            uint16_t r = (uint16_t)master_r + uni->data[3 + i * 3 + 0];
//            uint16_t g = (uint16_t)master_g + uni->data[3 + i * 3 + 1];
//            uint16_t b = (uint16_t)master_b + uni->data[3 + i * 3 + 2];
//
//            out[i].r = r > 255 ? 255 : (uint8_t)r;
//            out[i].g = g > 255 ? 255 : (uint8_t)g;
//            out[i].b = b > 255 ? 255 : (uint8_t)b;
//        }
//    } else {
//        // No signal — dim red on LED 0 as waiting indicator
//        memset(out, 0, num_leds * sizeof(rgb_color));
//        out[0].r = 32;
//    }
//}

//static void build_colors_from_universe(const DMX_Universe_t *uni,
//                                       rgb_color *out,
//                                       uint16_t num_leds)
//{
//    if (uni->valid) {
//        for (uint16_t i = 0; i < num_leds; i++) {
//        	out[i].r = uni->data[i * 3 + 1];  // Madrix G → LED R wire
//        	out[i].g = uni->data[i * 3 + 0];  // Madrix R → LED G wire
//        	out[i].b = uni->data[i * 3 + 2];
//        }
//    } else {
//        memset(out, 0, num_leds * sizeof(rgb_color));
//        out[0].r = 32;
//    }
//}

//static void build_colors_from_universe(const DMX_Universe_t *uni,
//                                       rgb_color *out,
//                                       uint16_t num_leds)
//{
//    if (uni->valid) {
//        for (uint16_t i = 0; i < num_leds; i++) {
//            out[i].r = uni->data[i * 3 + 1];  // Madrix G → LED R wire
//            out[i].g = uni->data[i * 3 + 0];  // Madrix R → LED G wire
//            out[i].b = uni->data[i * 3 + 2];
//        }
//    } else {
//        memset(out, 0, num_leds * sizeof(rgb_color));
//        out[0].r = 32;  // dim red = waiting indicator
//    }
//}
//
//// Helper macro to start one strip if its DMA is free
//#define PUSH_STRIP(idx, htim_inst, ch)                                  \
//    if (!dma_busy[(idx)]) {                                             \
//        set_pattern_led(leds[(idx)], dmx_colors[(idx)], NUMBER_OF_LEDS);\
//        dma_busy[(idx)] = 1;                                            \
//        HAL_TIM_PWM_Start_DMA(&(htim_inst), (ch),                       \
//            (uint32_t *)leds[(idx)],                                    \
//            NUMBER_OF_LEDS * 24 + 24);                                  \
//    }



//static void build_colors_from_universe(const DMX_Universe_t *uni,
//                                       rgb_color *out,
//                                       uint16_t num_leds)
//{
//    if (uni->valid) {
//        for (uint16_t i = 0; i < num_leds; i++) {
//            out[i].r = uni->data[i * 3 + 1];
//            out[i].g = uni->data[i * 3 + 0];
//            out[i].b = uni->data[i * 3 + 2];
//        }
//    } else {
//        memset(out, 0, num_leds * sizeof(rgb_color));
//        out[0].r = 32;
//    }
//}
//
//static void build_pin_colors(uint8_t pin_idx, uint8_t uni_base)
//{
//    for (uint8_t u = 0; u < UNIVERSES_PER_PIN; u++) {
//        build_colors_from_universe(
//            &dmx_universes[uni_base + u],
//            &dmx_colors[pin_idx][u * LEDS_PER_UNIVERSE],
//            LEDS_PER_UNIVERSE
//        );
//    }
//}

static void build_colors_from_universe(const DMX_Universe_t *uni,
                                       rgb_color *out,
                                       uint16_t num_leds)
{
    if (uni->valid) {
        for (uint16_t i = 0; i < num_leds; i++) {
            /* Straight RGB assignment — struct field order handles wire order */
            out[i].r = uni->data[i * 3 + 0];
            out[i].g = uni->data[i * 3 + 1];
            out[i].b = uni->data[i * 3 + 2];
        }
    } else {
        memset(out, 0, num_leds * sizeof(rgb_color));
        out[0].r = 32;   /* dim red = waiting indicator */
    }
}

static void build_pin_colors(uint8_t pin_idx, uint8_t uni_base)
{
    for (uint8_t u = 0; u < UNIVERSES_PER_PIN; u++) {
        build_colors_from_universe(
            &dmx_universes[uni_base + u],
            &dmx_colors[pin_idx][u * LEDS_PER_UNIVERSE],
            LEDS_PER_UNIVERSE
        );
    }
}

/* Ensure reset slot is zeroed — only needs to happen once at startup */
static void init_leds_reset_slots(void)
{
    for (uint8_t p = 0; p < NUM_PINS; p++)
        memset(&leds[p][LEDS_PER_PIN], 0, sizeof(neopixel_led));
}

#define PUSH_STRIP(pin_idx, htim_inst, ch)                                      \
    if (!dma_busy[(pin_idx)]) {                                                 \
        set_pattern_led(leds[(pin_idx)], dmx_colors[(pin_idx)], LEDS_PER_PIN); \
        dma_busy[(pin_idx)] = 1;                                                \
        HAL_TIM_PWM_Start_DMA(&(htim_inst), (ch),                               \
            (uint32_t *)leds[(pin_idx)],                                        \
            LEDS_PER_PIN * 24 + 24);                                            \
    }

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_USART3_UART_Init();
  MX_USB_OTG_FS_PCD_Init();
  MX_LWIP_Init();
  MX_TIM3_Init();
  MX_TIM4_Init();
  MX_TIM5_Init();
  MX_TIM1_Init();
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */
//    artnet_init();

  memset((uint8_t *)dma_busy, 0, sizeof(dma_busy));
  artnet_init();

    // netif_set_flags(&gnetif, NETIF_FLAG_BROADCAST);

//    rgb_color rainbow[NUMBER_OF_LEDS] = {
//        {255, 0,   0},
//        {255, 127, 0},
//        {255, 255, 0},
//        {0,   255, 0},
//        {0,   0,   255},
//        {75,  0,   130},
//        {148, 0,   211},
//        {255, 0,   0},
//        {255, 127, 0},
//        {255, 255, 0},
//        {0,   255, 0},
//        {0,   0,   255},
//    };
//    uint16_t offset = 0;
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

	  //PD 12 ,PA 6
//	      MX_LWIP_Process();
//
//	      /* --- Timestamp & status LEDs --------------------------------------- */
//	      uint32_t now = HAL_GetTick();
//
//	      static uint32_t last_pkt0 = 0, last_blink0 = 0;
//	      static uint32_t last_pkt1 = 0, last_blink1 = 0;
//
//	      if (dmx_universes[0].packet_count != last_pkt0) {
//	          last_pkt0   = dmx_universes[0].packet_count;
//	          last_blink0 = now;
//	      }
//	      if (dmx_universes[1].packet_count != last_pkt1) {
//	          last_pkt1   = dmx_universes[1].packet_count;
//	          last_blink1 = now;
//	      }
//
//	      // LD1 — ON when universe 0 has any master colour
//	      HAL_GPIO_WritePin(LD1_GPIO_Port, LD1_Pin,
//	          (dmx_universes[0].data[0] | dmx_universes[0].data[1] | dmx_universes[0].data[2])
//	              ? GPIO_PIN_SET : GPIO_PIN_RESET);
//
//	      // LD2 — blinks while universe 0 packets arrive
//	      HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin,
//	          ((now - last_blink0 < 500) && (now % 200 < 100))
//	              ? GPIO_PIN_SET : GPIO_PIN_RESET);
//
//	      // LD3 — blinks while universe 1 packets arrive
//	      HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin,
//	          ((now - last_blink1 < 500) && (now % 200 < 100))
//	              ? GPIO_PIN_SET : GPIO_PIN_RESET);
//
//	      /* --- Build colour buffers from each universe ----------------------- */
//	      build_colors_from_universe(&dmx_universes[0], dmx_colors1, NUMBER_OF_LEDS);
//	      build_colors_from_universe(&dmx_universes[1], dmx_colors2, NUMBER_OF_LEDS);
//
//	      /* --- Push strip 0 via TIM3 CH1 (PC6) ------------------------------ */
//	      if (!dma_busy1) {
//	          set_pattern_led(leds1, dmx_colors1, NUMBER_OF_LEDS);
//	          dma_busy1 = 1;
//	          HAL_TIM_PWM_Start_DMA(&htim3, TIM_CHANNEL_1,
//	                                (uint32_t *)leds1,
//	                                NUMBER_OF_LEDS * 24 + 24);
//	      }
//
//	      /* --- Push strip 1 via TIM4 CH1 (PB6) ------------------------------ */
//	      if (!dma_busy2) {
//	          set_pattern_led(leds2, dmx_colors2, NUMBER_OF_LEDS);
//	          dma_busy2 = 1;
//	          HAL_TIM_PWM_Start_DMA(&htim4, TIM_CHANNEL_1,
//	                                (uint32_t *)leds2,
//	                                NUMBER_OF_LEDS * 24 + 24);
//	      }
//
//	      HAL_Delay(20);



//	  MX_LWIP_Process();
//
//	     uint32_t now = HAL_GetTick();
//
//	     // --- Status LED blink on universe 0 packet activity ---
//	     static uint32_t last_pkt0 = 0, last_blink0 = 0;
//	     if (dmx_universes[0].packet_count != last_pkt0) {
//	         last_pkt0   = dmx_universes[0].packet_count;
//	         last_blink0 = now;
//	     }
//
//	     HAL_GPIO_WritePin(LD1_GPIO_Port, LD1_Pin,
//	         (dmx_universes[0].data[0] | dmx_universes[0].data[1] | dmx_universes[0].data[2])
//	             ? GPIO_PIN_SET : GPIO_PIN_RESET);
//
//	     HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin,
//	         ((now - last_blink0 < 500) && (now % 200 < 100))
//	             ? GPIO_PIN_SET : GPIO_PIN_RESET);
//
//	     // --- Build color buffers for all 8 universes ---
//	     for (int u = 0; u < 8; u++) {
//	         build_colors_from_universe(&dmx_universes[u], dmx_colors[u], NUMBER_OF_LEDS);
//	     }
//
//	     // --- Push all 8 strips ---
//	     // TIM3: universes 0-3 → PC6, PC7, PC8, PC9
//	     PUSH_STRIP(0, htim3, TIM_CHANNEL_1);
//	     PUSH_STRIP(1, htim3, TIM_CHANNEL_2);
//	     PUSH_STRIP(2, htim3, TIM_CHANNEL_3);
//	     PUSH_STRIP(3, htim3, TIM_CHANNEL_4);
//
//	     // TIM4: universes 4-7 → PB6, PB7, PB8, PB9
//	     PUSH_STRIP(4, htim4, TIM_CHANNEL_1);
//	     PUSH_STRIP(5, htim4, TIM_CHANNEL_2);
//	     PUSH_STRIP(6, htim4, TIM_CHANNEL_3);
//	     PUSH_STRIP(7, htim4, TIM_CHANNEL_4);
//
//	     HAL_Delay(20);


//	  MX_LWIP_Process();
//
//	      uint32_t now = HAL_GetTick();
//
//	      // Status LED — blink LD2 on any universe 0 activity
//	      static uint32_t last_pkt0 = 0, last_blink0 = 0;
//	      if (dmx_universes[0].packet_count != last_pkt0) {
//	          last_pkt0   = dmx_universes[0].packet_count;
//	          last_blink0 = now;
//	      }
//	      HAL_GPIO_WritePin(LD1_GPIO_Port, LD1_Pin,
//	          (dmx_universes[0].data[0] | dmx_universes[0].data[1] | dmx_universes[0].data[2])
//	              ? GPIO_PIN_SET : GPIO_PIN_RESET);
//	      HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin,
//	          ((now - last_blink0 < 500) && (now % 200 < 100))
//	              ? GPIO_PIN_SET : GPIO_PIN_RESET);
//
//	      // Build all 6 pin color buffers (each = 3 universes concatenated)
//	      build_pin_colors(0,  0);   // PC6 ← uni 0,1,2
//	      build_pin_colors(1,  3);   // PC7 ← uni 3,4,5
//	      build_pin_colors(2,  6);   // PC8 ← uni 6,7,8
//	      build_pin_colors(3,  9);   // PC9 ← uni 9,10,11
//	      build_pin_colors(4, 12);   // PB6 ← uni 12,13,14
//	      build_pin_colors(5, 15);   // PB7 ← uni 15,16,17
//
//	      // Fire DMA on each pin if idle
//	      PUSH_STRIP(0, htim3, TIM_CHANNEL_1);  // PC6
//	      PUSH_STRIP(1, htim3, TIM_CHANNEL_2);  // PC7
//	      PUSH_STRIP(2, htim3, TIM_CHANNEL_3);  // PC8
//	      PUSH_STRIP(3, htim3, TIM_CHANNEL_4);  // PC9
//	      PUSH_STRIP(4, htim4, TIM_CHANNEL_1);  // PB6
//	      PUSH_STRIP(5, htim4, TIM_CHANNEL_2);  // PB7
//
//	      HAL_Delay(20);

	  MX_LWIP_Process();

	     uint32_t now = HAL_GetTick();

	     // Status LED blink
	     static uint32_t last_pkt0 = 0, last_blink0 = 0;
	     if (dmx_universes[0].packet_count != last_pkt0) {
	         last_pkt0   = dmx_universes[0].packet_count;
	         last_blink0 = now;
	     }
	     HAL_GPIO_WritePin(LD1_GPIO_Port, LD1_Pin,
	         (dmx_universes[0].data[0] | dmx_universes[0].data[1] | dmx_universes[0].data[2])
	             ? GPIO_PIN_SET : GPIO_PIN_RESET);
	     HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin,
	         ((now - last_blink0 < 500) && (now % 200 < 100))
	             ? GPIO_PIN_SET : GPIO_PIN_RESET);

	     // Only build + push a pin if its DMA is idle
	     // This prevents writing dmx_colors[] while set_pattern_led() is reading it
//	     if (!dma_busy[0]) { build_pin_colors(0,  0); PUSH_STRIP(0, htim3, TIM_CHANNEL_1); }
//	     if (!dma_busy[1]) { build_pin_colors(1,  3); PUSH_STRIP(1, htim3, TIM_CHANNEL_2); }
//	     if (!dma_busy[2]) { build_pin_colors(2,  6); PUSH_STRIP(2, htim3, TIM_CHANNEL_3); }
//	     if (!dma_busy[3]) { build_pin_colors(3,  9); PUSH_STRIP(3, htim3, TIM_CHANNEL_4); }
//	     if (!dma_busy[4]) { build_pin_colors(4, 12); PUSH_STRIP(4, htim4, TIM_CHANNEL_1); }
//	     if (!dma_busy[5]) { build_pin_colors(5, 15); PUSH_STRIP(5, htim4, TIM_CHANNEL_2); }

	     if (!dma_busy[0]) { build_pin_colors(0,  0); PUSH_STRIP(0, htim3, TIM_CHANNEL_1); }
	     if (!dma_busy[1]) { build_pin_colors(1,  4); PUSH_STRIP(1, htim3, TIM_CHANNEL_2); }
	     if (!dma_busy[2]) { build_pin_colors(2,  8); PUSH_STRIP(2, htim3, TIM_CHANNEL_3); }
	     if (!dma_busy[3]) { build_pin_colors(3, 12); PUSH_STRIP(3, htim3, TIM_CHANNEL_4); }
	     if (!dma_busy[4]) { build_pin_colors(4, 16); PUSH_STRIP(4, htim4, TIM_CHANNEL_1); }


  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief TIM1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM1_Init(void)
{

  /* USER CODE BEGIN TIM1_Init 0 */

  /* USER CODE END TIM1_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};
  TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig = {0};

  /* USER CODE BEGIN TIM1_Init 1 */

  /* USER CODE END TIM1_Init 1 */
  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 0;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 65535;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
  sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
  if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  sBreakDeadTimeConfig.OffStateRunMode = TIM_OSSR_DISABLE;
  sBreakDeadTimeConfig.OffStateIDLEMode = TIM_OSSI_DISABLE;
  sBreakDeadTimeConfig.LockLevel = TIM_LOCKLEVEL_OFF;
  sBreakDeadTimeConfig.DeadTime = 0;
  sBreakDeadTimeConfig.BreakState = TIM_BREAK_DISABLE;
  sBreakDeadTimeConfig.BreakPolarity = TIM_BREAKPOLARITY_HIGH;
  sBreakDeadTimeConfig.AutomaticOutput = TIM_AUTOMATICOUTPUT_DISABLE;
  if (HAL_TIMEx_ConfigBreakDeadTime(&htim1, &sBreakDeadTimeConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM1_Init 2 */

  /* USER CODE END TIM1_Init 2 */
  HAL_TIM_MspPostInit(&htim1);

}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 0;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 4294967295;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */
  HAL_TIM_MspPostInit(&htim2);

}

/**
  * @brief TIM3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM3_Init(void)
{

  /* USER CODE BEGIN TIM3_Init 0 */

  /* USER CODE END TIM3_Init 0 */

  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM3_Init 1 */

  /* USER CODE END TIM3_Init 1 */
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 3;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 24;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_PWM_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_3) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_4) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM3_Init 2 */

  /* USER CODE END TIM3_Init 2 */
  HAL_TIM_MspPostInit(&htim3);

}

/**
  * @brief TIM4 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM4_Init(void)
{

  /* USER CODE BEGIN TIM4_Init 0 */

  /* USER CODE END TIM4_Init 0 */

  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM4_Init 1 */

  /* USER CODE END TIM4_Init 1 */
  htim4.Instance = TIM4;
  htim4.Init.Prescaler = 3;
  htim4.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim4.Init.Period = 24;
  htim4.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim4.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_PWM_Init(&htim4) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim4, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim4, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_ConfigChannel(&htim4, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_ConfigChannel(&htim4, &sConfigOC, TIM_CHANNEL_3) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_ConfigChannel(&htim4, &sConfigOC, TIM_CHANNEL_4) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM4_Init 2 */

  /* USER CODE END TIM4_Init 2 */
  HAL_TIM_MspPostInit(&htim4);

}

/**
  * @brief TIM5 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM5_Init(void)
{

  /* USER CODE BEGIN TIM5_Init 0 */

  /* USER CODE END TIM5_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM5_Init 1 */

  /* USER CODE END TIM5_Init 1 */
  htim5.Instance = TIM5;
  htim5.Init.Prescaler = 0;
  htim5.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim5.Init.Period = 4294967295;
  htim5.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim5.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim5) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim5, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Init(&htim5) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim5, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim5, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_ConfigChannel(&htim5, &sConfigOC, TIM_CHANNEL_4) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM5_Init 2 */

  /* USER CODE END TIM5_Init 2 */
  HAL_TIM_MspPostInit(&htim5);

}

/**
  * @brief USART3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART3_UART_Init(void)
{

  /* USER CODE BEGIN USART3_Init 0 */

  /* USER CODE END USART3_Init 0 */

  /* USER CODE BEGIN USART3_Init 1 */

  /* USER CODE END USART3_Init 1 */
  huart3.Instance = USART3;
  huart3.Init.BaudRate = 115200;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART3_Init 2 */

  /* USER CODE END USART3_Init 2 */

}

/**
  * @brief USB_OTG_FS Initialization Function
  * @param None
  * @retval None
  */
static void MX_USB_OTG_FS_PCD_Init(void)
{

  /* USER CODE BEGIN USB_OTG_FS_Init 0 */

  /* USER CODE END USB_OTG_FS_Init 0 */

  /* USER CODE BEGIN USB_OTG_FS_Init 1 */

  /* USER CODE END USB_OTG_FS_Init 1 */
  hpcd_USB_OTG_FS.Instance = USB_OTG_FS;
  hpcd_USB_OTG_FS.Init.dev_endpoints = 4;
  hpcd_USB_OTG_FS.Init.speed = PCD_SPEED_FULL;
  hpcd_USB_OTG_FS.Init.dma_enable = DISABLE;
  hpcd_USB_OTG_FS.Init.phy_itface = PCD_PHY_EMBEDDED;
  hpcd_USB_OTG_FS.Init.Sof_enable = ENABLE;
  hpcd_USB_OTG_FS.Init.low_power_enable = DISABLE;
  hpcd_USB_OTG_FS.Init.lpm_enable = DISABLE;
  hpcd_USB_OTG_FS.Init.vbus_sensing_enable = ENABLE;
  hpcd_USB_OTG_FS.Init.use_dedicated_ep1 = DISABLE;
  if (HAL_PCD_Init(&hpcd_USB_OTG_FS) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USB_OTG_FS_Init 2 */

  /* USER CODE END USB_OTG_FS_Init 2 */

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();
  __HAL_RCC_DMA2_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Stream0_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream0_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream0_IRQn);
  /* DMA1_Stream1_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream1_IRQn);
  /* DMA1_Stream2_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream2_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream2_IRQn);
  /* DMA1_Stream3_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream3_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream3_IRQn);
  /* DMA1_Stream4_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream4_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream4_IRQn);
  /* DMA1_Stream5_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream5_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream5_IRQn);
  /* DMA1_Stream6_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream6_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream6_IRQn);
  /* DMA1_Stream7_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream7_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream7_IRQn);
  /* DMA2_Stream1_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA2_Stream1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream1_IRQn);
  /* DMA2_Stream4_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA2_Stream4_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream4_IRQn);
  /* DMA2_Stream5_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA2_Stream5_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream5_IRQn);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* USER CODE BEGIN MX_GPIO_Init_1 */

  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, LD1_Pin|LD3_Pin|LD2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOG, GPIO_PIN_6, GPIO_PIN_RESET);

  /*Configure GPIO pin : USER_Btn_Pin */
  GPIO_InitStruct.Pin = USER_Btn_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(USER_Btn_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : LD1_Pin LD3_Pin LD2_Pin */
  GPIO_InitStruct.Pin = LD1_Pin|LD3_Pin|LD2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : PG6 */
  GPIO_InitStruct.Pin = GPIO_PIN_6;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

  /*Configure GPIO pin : USB_OverCurrent_Pin */
  GPIO_InitStruct.Pin = USB_OverCurrent_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(USB_OverCurrent_GPIO_Port, &GPIO_InitStruct);

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

//void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim)
//{
//    if (htim->Instance == TIM3) {
//        HAL_TIM_PWM_Stop_DMA(htim, TIM_CHANNEL_1);
//        dma_busy1 = 0;
//    }
//    else if (htim->Instance == TIM4) {
//        HAL_TIM_PWM_Stop_DMA(htim, TIM_CHANNEL_1);
//        dma_busy2 = 0;
//    }
//}

//void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim)
//{
//    if (htim->Instance == TIM3) {
//        uint32_t ch = htim->Channel;
//        if (ch == HAL_TIM_ACTIVE_CHANNEL_1) { HAL_TIM_PWM_Stop_DMA(htim, TIM_CHANNEL_1); dma_busy[0] = 0; }
//        if (ch == HAL_TIM_ACTIVE_CHANNEL_2) { HAL_TIM_PWM_Stop_DMA(htim, TIM_CHANNEL_2); dma_busy[1] = 0; }
//        if (ch == HAL_TIM_ACTIVE_CHANNEL_3) { HAL_TIM_PWM_Stop_DMA(htim, TIM_CHANNEL_3); dma_busy[2] = 0; }
//        if (ch == HAL_TIM_ACTIVE_CHANNEL_4) { HAL_TIM_PWM_Stop_DMA(htim, TIM_CHANNEL_4); dma_busy[3] = 0; }
//    }
//    else if (htim->Instance == TIM4) {
//        uint32_t ch = htim->Channel;
//        if (ch == HAL_TIM_ACTIVE_CHANNEL_1) { HAL_TIM_PWM_Stop_DMA(htim, TIM_CHANNEL_1); dma_busy[4] = 0; }
//        if (ch == HAL_TIM_ACTIVE_CHANNEL_2) { HAL_TIM_PWM_Stop_DMA(htim, TIM_CHANNEL_2); dma_busy[5] = 0; }
//        if (ch == HAL_TIM_ACTIVE_CHANNEL_3) { HAL_TIM_PWM_Stop_DMA(htim, TIM_CHANNEL_3); dma_busy[6] = 0; }
//        if (ch == HAL_TIM_ACTIVE_CHANNEL_4) { HAL_TIM_PWM_Stop_DMA(htim, TIM_CHANNEL_4); dma_busy[7] = 0; }
//    }
//}

void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM3) {
        uint32_t ch = htim->Channel;
        if (ch == HAL_TIM_ACTIVE_CHANNEL_1) { HAL_TIM_PWM_Stop_DMA(htim, TIM_CHANNEL_1); dma_busy[0] = 0; }
        if (ch == HAL_TIM_ACTIVE_CHANNEL_2) { HAL_TIM_PWM_Stop_DMA(htim, TIM_CHANNEL_2); dma_busy[1] = 0; }
        if (ch == HAL_TIM_ACTIVE_CHANNEL_3) { HAL_TIM_PWM_Stop_DMA(htim, TIM_CHANNEL_3); dma_busy[2] = 0; }
        if (ch == HAL_TIM_ACTIVE_CHANNEL_4) { HAL_TIM_PWM_Stop_DMA(htim, TIM_CHANNEL_4); dma_busy[3] = 0; }
    }
    else if (htim->Instance == TIM4) {
        uint32_t ch = htim->Channel;
        if (ch == HAL_TIM_ACTIVE_CHANNEL_1) { HAL_TIM_PWM_Stop_DMA(htim, TIM_CHANNEL_1); dma_busy[4] = 0; }
//        if (ch == HAL_TIM_ACTIVE_CHANNEL_2) { HAL_TIM_PWM_Stop_DMA(htim, TIM_CHANNEL_2); dma_busy[5] = 0; }
    }
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

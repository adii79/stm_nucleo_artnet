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
//#define NUMBER_OF_LEDS  12
#define NUMBER_OF_LEDS  50
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
TIM_HandleTypeDef htim3;
TIM_HandleTypeDef htim6;
DMA_HandleTypeDef hdma_tim3_ch1_trig;

UART_HandleTypeDef huart3;

PCD_HandleTypeDef hpcd_USB_OTG_FS;

/* USER CODE BEGIN PV */
neopixel_led leds[NUMBER_OF_LEDS + 1];
static rgb_color dmx_colors[NUMBER_OF_LEDS];
static uint8_t dma_busy1 = 0;                // guard flag
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_TIM6_Init(void);
static void MX_USART3_UART_Init(void);
static void MX_USB_OTG_FS_PCD_Init(void);
static void MX_TIM3_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

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
  MX_TIM6_Init();
  MX_USART3_UART_Init();
  MX_USB_OTG_FS_PCD_Init();
  MX_LWIP_Init();
  MX_TIM3_Init();
  /* USER CODE BEGIN 2 */
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

//	  MX_LWIP_Process();
//
//	 	   HAL_GPIO_WritePin(LD1_GPIO_Port, LD1_Pin,
//	 	       dmx_universes[0].data[0] > 127 ? GPIO_PIN_SET : GPIO_PIN_RESET);
//
//	 	   // LD2 (blue) — blinks only when DMX packets are actively arriving
//	 	   static uint32_t last_packet_count = 0;
//	 	   static uint32_t last_blink_ms = 0;
//	 	   static uint8_t  led_state = 0;
//
//	 	   uint32_t now = HAL_GetTick();
//	 	   if (dmx_universes[0].packet_count != last_packet_count) {
//	 	       last_packet_count = dmx_universes[0].packet_count;
//	 	       last_blink_ms = now;
//	 	   }
//
//	 	   if (now - last_blink_ms < 500) {
//	 	       if (now % 200 < 100)
//	 	           HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET);
//	 	       else
//	 	           HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);
//	 	   } else {
//	 	       HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);
//	 	   }
//	 	  rgb_color rotated[NUMBER_OF_LEDS];
//	 	  	    for (int i = 0; i < NUMBER_OF_LEDS; i++)
//	 	  	        rotated[i] = rainbow[(i + offset) % NUMBER_OF_LEDS];
//
//	 	  	    set_pattern_led(leds, rotated, NUMBER_OF_LEDS);
//	 	  	    HAL_TIM_PWM_Start_DMA(&htim3, TIM_CHANNEL_1, (uint32_t *)leds, NUMBER_OF_LEDS * 24 + 24);
//	 	  	    HAL_Delay(100);
//
//	 	  	    offset = (offset + 1) % NUMBER_OF_LEDS;

//
//	  MX_LWIP_Process();
//
//	    /* --- Status LEDs -------------------------------------------------- */
//	    HAL_GPIO_WritePin(LD1_GPIO_Port, LD1_Pin,
//	        dmx_universes[0].data[0] > 127 ? GPIO_PIN_SET : GPIO_PIN_RESET);
//
//	    static uint32_t last_packet_count = 0;
//	    static uint32_t last_blink_ms     = 0;
//
//	    uint32_t now = HAL_GetTick();
//	    if (dmx_universes[0].packet_count != last_packet_count) {
//	        last_packet_count = dmx_universes[0].packet_count;
//	        last_blink_ms     = now;
//	    }
//	    HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin,
//	        (now - last_blink_ms < 500) && (now % 200 < 100)
//	            ? GPIO_PIN_SET : GPIO_PIN_RESET);
//
//	    /* --- Read active LED count from CH1 (data[0]) --------------------- */
//	    // Sender puts 0–50 directly in CH1
//	    uint8_t active_leds = dmx_universes[0].data[0];
//	    if (active_leds > NUMBER_OF_LEDS) active_leds = NUMBER_OF_LEDS;
//
//	    /* --- Build colours from CH2 onwards (data[1..]) ------------------- */
//	    // CH2,3,4 = LED0 R,G,B  |  CH5,6,7 = LED1 R,G,B  |  ...
//	    // Max channels used: 1 + (50 * 3) = 151 — fits easily in one universe
//	    static rgb_color dmx_colors[NUMBER_OF_LEDS];
//	    memset(dmx_colors, 0, sizeof(dmx_colors));
//
//	    if (dmx_universes[0].valid && active_leds > 0) {
//	        for (uint8_t i = 0; i < active_leds; i++) {
//	            dmx_colors[i].r = dmx_universes[0].data[1 + i * 3 + 0];
//	            dmx_colors[i].g = dmx_universes[0].data[1 + i * 3 + 1];
//	            dmx_colors[i].b = dmx_universes[0].data[1 + i * 3 + 2];
//	        }
//	    } else {
//	        // No signal — dim red on LED 0 as waiting indicator
//	        dmx_colors[0].r = 32;
//	    }
//
//	    /* --- Push to strip ------------------------------------------------ */
//	    set_pattern_led(leds, dmx_colors, NUMBER_OF_LEDS);
//	    HAL_TIM_PWM_Start_DMA(&htim3, TIM_CHANNEL_1,
//	                          (uint32_t *)leds,
//	                          NUMBER_OF_LEDS * 24 + 24);
//	  HAL_Delay(20);



//	  MX_LWIP_Process();
//
//	     /* --- Status LEDs -------------------------------------------------- */
//	     static uint32_t last_packet_count = 0;
//	     static uint32_t last_blink_ms     = 0;
//
//	     uint32_t now = HAL_GetTick();
//	     if (dmx_universes[0].packet_count != last_packet_count) {
//	         last_packet_count = dmx_universes[0].packet_count;
//	         last_blink_ms     = now;
//	     }
//	     // LD1 — ON when any master colour is non-zero
//	     HAL_GPIO_WritePin(LD1_GPIO_Port, LD1_Pin,
//	         (dmx_universes[0].data[0] | dmx_universes[0].data[1] | dmx_universes[0].data[2])
//	             ? GPIO_PIN_SET : GPIO_PIN_RESET);
//
//	     // LD2 — blinks while DMX packets are arriving
//	     HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin,
//	         (now - last_blink_ms < 500) && (now % 200 < 100)
//	             ? GPIO_PIN_SET : GPIO_PIN_RESET);
//
//	     /* --- Read master RGB (CH1, CH2, CH3) ------------------------------ */
//	     uint8_t master_r = dmx_universes[0].data[0];   // CH1
//	     uint8_t master_g = dmx_universes[0].data[1];   // CH2
//	     uint8_t master_b = dmx_universes[0].data[2];   // CH3
//
//	     /* --- Build colours — master + per-LED ----------------------------- */
//	     // Per-LED starts at data[3] (CH4)
//	     // LED i → data[3 + i*3 + 0/1/2]
//	     // Total: 3 + (50*3) = 153 channels — fits in one universe
//	     static rgb_color dmx_colors[NUMBER_OF_LEDS];
//
//	     if (dmx_universes[0].valid) {
//	         for (uint8_t i = 0; i < NUMBER_OF_LEDS; i++) {
//	             uint16_t r = (uint16_t)master_r + dmx_universes[0].data[3 + i * 3 + 0];
//	             uint16_t g = (uint16_t)master_g + dmx_universes[0].data[3 + i * 3 + 1];
//	             uint16_t b = (uint16_t)master_b + dmx_universes[0].data[3 + i * 3 + 2];
//
//	             dmx_colors[i].r = r > 255 ? 255 : (uint8_t)r;
//	             dmx_colors[i].g = g > 255 ? 255 : (uint8_t)g;
//	             dmx_colors[i].b = b > 255 ? 255 : (uint8_t)b;
//	         }
//	     } else {
//	         // No signal — dim red on LED 0 as waiting indicator
//	         memset(dmx_colors, 0, sizeof(dmx_colors));
//	         dmx_colors[0].r = 32;
//	     }
//
//	     /* --- Push to strip ------------------------------------------------ */
//	     set_pattern_led(leds, dmx_colors, NUMBER_OF_LEDS);
//	     HAL_TIM_PWM_Start_DMA(&htim3, TIM_CHANNEL_1,
//	                           (uint32_t *)leds,
//	                           NUMBER_OF_LEDS * 24 + 24);
//	     HAL_Delay(20);


	  MX_LWIP_Process();

	     /* --- Status LEDs -------------------------------------------------- */
	     static uint32_t last_packet_count = 0;
	     static uint32_t last_blink_ms     = 0;

	     uint32_t now = HAL_GetTick();
	     if (dmx_universes[0].packet_count != last_packet_count) {
	         last_packet_count = dmx_universes[0].packet_count;
	         last_blink_ms     = now;
	     }
	     // LD1 — ON when any master colour is non-zero
	     HAL_GPIO_WritePin(LD1_GPIO_Port, LD1_Pin,
	         (dmx_universes[0].data[0] | dmx_universes[0].data[1] | dmx_universes[0].data[2])
	             ? GPIO_PIN_SET : GPIO_PIN_RESET);

	     // LD2 — blinks while DMX packets are arriving
	     HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin,
	         (now - last_blink_ms < 500) && (now % 200 < 100)
	             ? GPIO_PIN_SET : GPIO_PIN_RESET);

	     /* --- Read master RGB (CH1, CH2, CH3) ------------------------------ */
	     uint8_t master_r = dmx_universes[0].data[0];   // CH1
	     uint8_t master_g = dmx_universes[0].data[1];   // CH2
	     uint8_t master_b = dmx_universes[0].data[2];   // CH3

	     /* --- Build colours — master + per-LED ----------------------------- */
	     // Per-LED starts at data[3] (CH4)
	     // LED i → data[3 + i*3 + 0/1/2]
	     // Total: 3 + (50*3) = 153 channels — fits in one universe
	     static rgb_color dmx_colors[NUMBER_OF_LEDS];

	     if (dmx_universes[0].valid) {
	         for (uint8_t i = 0; i < NUMBER_OF_LEDS; i++) {
	             uint16_t r = (uint16_t)master_r + dmx_universes[0].data[3 + i * 3 + 0];
	             uint16_t g = (uint16_t)master_g + dmx_universes[0].data[3 + i * 3 + 1];
	             uint16_t b = (uint16_t)master_b + dmx_universes[0].data[3 + i * 3 + 2];

	             dmx_colors[i].r = r > 255 ? 255 : (uint8_t)r;
	             dmx_colors[i].g = g > 255 ? 255 : (uint8_t)g;
	             dmx_colors[i].b = b > 255 ? 255 : (uint8_t)b;
	         }
	     } else {
	         // No signal — dim red on LED 0 as waiting indicator
	         memset(dmx_colors, 0, sizeof(dmx_colors));
	         dmx_colors[0].r = 32;
	     }

	     /* --- Push to strip ------------------------------------------------ */
	     set_pattern_led(leds, dmx_colors, NUMBER_OF_LEDS);
	     HAL_TIM_PWM_Start_DMA(&htim3, TIM_CHANNEL_1,
	                           (uint32_t *)leds,
	                           NUMBER_OF_LEDS * 24 + 24);
	     HAL_Delay(20);


	  // dmx data on 2 pins(pwm pc7 and )

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
  /* USER CODE BEGIN TIM3_Init 2 */

  /* USER CODE END TIM3_Init 2 */
  HAL_TIM_MspPostInit(&htim3);

}

/**
  * @brief TIM6 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM6_Init(void)
{

  /* USER CODE BEGIN TIM6_Init 0 */

  /* USER CODE END TIM6_Init 0 */

  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM6_Init 1 */

  /* USER CODE END TIM6_Init 1 */
  htim6.Instance = TIM6;
  htim6.Init.Prescaler = 8999;
  htim6.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim6.Init.Period = 2000;
  htim6.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim6) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim6, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM6_Init 2 */

  /* USER CODE END TIM6_Init 2 */

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

  /* DMA interrupt init */
  /* DMA1_Stream4_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream4_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream4_IRQn);

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
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, LD1_Pin|LD3_Pin|LD2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(USB_PowerSwitchOn_GPIO_Port, USB_PowerSwitchOn_Pin, GPIO_PIN_RESET);

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

  /*Configure GPIO pin : USB_PowerSwitchOn_Pin */
  GPIO_InitStruct.Pin = USB_PowerSwitchOn_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(USB_PowerSwitchOn_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : USB_OverCurrent_Pin */
  GPIO_InitStruct.Pin = USB_OverCurrent_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(USB_OverCurrent_GPIO_Port, &GPIO_InitStruct);

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
/* USER CODE BEGIN 4 */
void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM3) {
        HAL_TIM_PWM_Stop_DMA(htim, TIM_CHANNEL_1);
        dma_busy1 = 0;
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

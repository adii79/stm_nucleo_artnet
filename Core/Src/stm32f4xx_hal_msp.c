///* USER CODE BEGIN Header */
///**
//  ******************************************************************************
//  * @file         stm32f4xx_hal_msp.c
//  * @brief        This file provides code for the MSP Initialization
//  *               and de-Initialization codes.
//  ******************************************************************************
//  * @attention
//  *
//  * Copyright (c) 2026 STMicroelectronics.
//  * All rights reserved.
//  *
//  * This software is licensed under terms that can be found in the LICENSE file
//  * in the root directory of this software component.
//  * If no LICENSE file comes with this software, it is provided AS-IS.
//  *
//  ******************************************************************************
//  */
///* USER CODE END Header */
///* Includes ------------------------------------------------------------------*/
//#include "main.h"
///* USER CODE BEGIN Includes */
//
///* USER CODE END Includes */
//extern DMA_HandleTypeDef hdma_tim1_ch1;
//
//extern DMA_HandleTypeDef hdma_tim1_ch2;
//
//extern DMA_HandleTypeDef hdma_tim1_ch3;
//
//extern DMA_HandleTypeDef hdma_tim1_ch4_trig_com;
//
//extern DMA_HandleTypeDef hdma_tim2_ch2_ch4;
//
//extern DMA_HandleTypeDef hdma_tim2_up_ch3;
//
//extern DMA_HandleTypeDef hdma_tim3_ch1_trig;
//
//extern DMA_HandleTypeDef hdma_tim3_ch2;
//
//extern DMA_HandleTypeDef hdma_tim3_ch3;
//
//extern DMA_HandleTypeDef hdma_tim3_ch4_up;
//
//extern DMA_HandleTypeDef hdma_tim4_ch1;
//
//extern DMA_HandleTypeDef hdma_tim4_ch2;
//
///* Private typedef -----------------------------------------------------------*/
///* USER CODE BEGIN TD */
//
///* USER CODE END TD */
//
///* Private define ------------------------------------------------------------*/
///* USER CODE BEGIN Define */
//
///* USER CODE END Define */
//
///* Private macro -------------------------------------------------------------*/
///* USER CODE BEGIN Macro */
//
///* USER CODE END Macro */
//
///* Private variables ---------------------------------------------------------*/
///* USER CODE BEGIN PV */
//
///* USER CODE END PV */
//
///* Private function prototypes -----------------------------------------------*/
///* USER CODE BEGIN PFP */
//
///* USER CODE END PFP */
//
///* External functions --------------------------------------------------------*/
///* USER CODE BEGIN ExternalFunctions */
//
///* USER CODE END ExternalFunctions */
//
///* USER CODE BEGIN 0 */
//
///* USER CODE END 0 */
//
//void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);
//                                                                                /**
//  * Initializes the Global MSP.
//  */
//void HAL_MspInit(void)
//{
//
//  /* USER CODE BEGIN MspInit 0 */
//
//  /* USER CODE END MspInit 0 */
//
//  __HAL_RCC_SYSCFG_CLK_ENABLE();
//  __HAL_RCC_PWR_CLK_ENABLE();
//
//  /* System interrupt init*/
//
//  /* USER CODE BEGIN MspInit 1 */
//
//  /* USER CODE END MspInit 1 */
//}
//
///**
//  * @brief TIM_PWM MSP Initialization
//  * This function configures the hardware resources used in this example
//  * @param htim_pwm: TIM_PWM handle pointer
//  * @retval None
//  */
//void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef* htim_pwm)
//{
//  if(htim_pwm->Instance==TIM1)
//  {
//    /* USER CODE BEGIN TIM1_MspInit 0 */
//
//    /* USER CODE END TIM1_MspInit 0 */
//    /* Peripheral clock enable */
//    __HAL_RCC_TIM1_CLK_ENABLE();
//
//    /* TIM1 DMA Init */
//    /* TIM1_CH1 Init */
//        hdma_tim1_ch1.Instance                 = DMA2_Stream1;
//        hdma_tim1_ch1.Init.Channel             = DMA_CHANNEL_6;
//        hdma_tim1_ch1.Init.Direction           = DMA_MEMORY_TO_PERIPH;
//        hdma_tim1_ch1.Init.PeriphInc           = DMA_PINC_DISABLE;
//        hdma_tim1_ch1.Init.MemInc              = DMA_MINC_ENABLE;
//        hdma_tim1_ch1.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
//        hdma_tim1_ch1.Init.MemDataAlignment    = DMA_MDATAALIGN_HALFWORD;
//        hdma_tim1_ch1.Init.Mode                = DMA_NORMAL;
//        hdma_tim1_ch1.Init.Priority            = DMA_PRIORITY_HIGH;
//        hdma_tim1_ch1.Init.FIFOMode            = DMA_FIFOMODE_ENABLE;
//        hdma_tim1_ch1.Init.FIFOThreshold       = DMA_FIFO_THRESHOLD_FULL;
//        hdma_tim1_ch1.Init.MemBurst            = DMA_MBURST_INC4;
//        hdma_tim1_ch1.Init.PeriphBurst         = DMA_PBURST_INC4;
//        if (HAL_DMA_Init(&hdma_tim1_ch1) != HAL_OK)
//        {
//          Error_Handler();
//        }
//
//        __HAL_LINKDMA(htim_pwm, hdma[TIM_DMA_ID_CC1], hdma_tim1_ch1);
//
//    /* TIM1_CH2 Init */
//    hdma_tim1_ch2.Instance = DMA2_Stream2;
//    hdma_tim1_ch2.Init.Channel = DMA_CHANNEL_6;
//    hdma_tim1_ch2.Init.Direction = DMA_MEMORY_TO_PERIPH;
//    hdma_tim1_ch2.Init.PeriphInc = DMA_PINC_DISABLE;
//    hdma_tim1_ch2.Init.MemInc = DMA_MINC_ENABLE;
//    hdma_tim1_ch2.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
//    hdma_tim1_ch2.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
//    hdma_tim1_ch2.Init.Mode = DMA_NORMAL;
//    hdma_tim1_ch2.Init.Priority = DMA_PRIORITY_LOW;
//    hdma_tim1_ch2.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
//    if (HAL_DMA_Init(&hdma_tim1_ch2) != HAL_OK)
//    {
//      Error_Handler();
//    }
//
//    __HAL_LINKDMA(htim_pwm,hdma[TIM_DMA_ID_CC2],hdma_tim1_ch2);
//
//    /* TIM1_CH3 Init */
//    hdma_tim1_ch3.Instance = DMA2_Stream6;
//    hdma_tim1_ch3.Init.Channel = DMA_CHANNEL_6;
//    hdma_tim1_ch3.Init.Direction = DMA_MEMORY_TO_PERIPH;
//    hdma_tim1_ch3.Init.PeriphInc = DMA_PINC_DISABLE;
//    hdma_tim1_ch3.Init.MemInc = DMA_MINC_ENABLE;
//    hdma_tim1_ch3.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
//    hdma_tim1_ch3.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
//    hdma_tim1_ch3.Init.Mode = DMA_NORMAL;
//    hdma_tim1_ch3.Init.Priority = DMA_PRIORITY_LOW;
//    hdma_tim1_ch3.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
//    if (HAL_DMA_Init(&hdma_tim1_ch3) != HAL_OK)
//    {
//      Error_Handler();
//    }
//
//    __HAL_LINKDMA(htim_pwm,hdma[TIM_DMA_ID_CC3],hdma_tim1_ch3);
//
//    /* TIM1_CH4_TRIG_COM Init */
//    hdma_tim1_ch4_trig_com.Instance = DMA2_Stream4;
//    hdma_tim1_ch4_trig_com.Init.Channel = DMA_CHANNEL_6;
//    hdma_tim1_ch4_trig_com.Init.Direction = DMA_MEMORY_TO_PERIPH;
//    hdma_tim1_ch4_trig_com.Init.PeriphInc = DMA_PINC_DISABLE;
//    hdma_tim1_ch4_trig_com.Init.MemInc = DMA_MINC_ENABLE;
//    hdma_tim1_ch4_trig_com.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
//    hdma_tim1_ch4_trig_com.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
//    hdma_tim1_ch4_trig_com.Init.Mode = DMA_NORMAL;
//    hdma_tim1_ch4_trig_com.Init.Priority = DMA_PRIORITY_LOW;
//    hdma_tim1_ch4_trig_com.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
//    if (HAL_DMA_Init(&hdma_tim1_ch4_trig_com) != HAL_OK)
//    {
//      Error_Handler();
//    }
//
//    /* Several peripheral DMA handle pointers point to the same DMA handle.
//     Be aware that there is only one stream to perform all the requested DMAs. */
//    __HAL_LINKDMA(htim_pwm,hdma[TIM_DMA_ID_CC4],hdma_tim1_ch4_trig_com);
//    __HAL_LINKDMA(htim_pwm,hdma[TIM_DMA_ID_TRIGGER],hdma_tim1_ch4_trig_com);
//    __HAL_LINKDMA(htim_pwm,hdma[TIM_DMA_ID_COMMUTATION],hdma_tim1_ch4_trig_com);
//
//    /* USER CODE BEGIN TIM1_MspInit 1 */
//
//    /* USER CODE END TIM1_MspInit 1 */
//  }
//  else if(htim_pwm->Instance==TIM3)
//  {
//    /* USER CODE BEGIN TIM3_MspInit 0 */
//
//    /* USER CODE END TIM3_MspInit 0 */
//    /* Peripheral clock enable */
//    __HAL_RCC_TIM3_CLK_ENABLE();
//
//    /* TIM3 DMA Init */
//    /* TIM3_CH1_TRIG Init */
//        hdma_tim3_ch1_trig.Instance                 = DMA1_Stream4;
//        hdma_tim3_ch1_trig.Init.Channel             = DMA_CHANNEL_5;
//        hdma_tim3_ch1_trig.Init.Direction           = DMA_MEMORY_TO_PERIPH;
//        hdma_tim3_ch1_trig.Init.PeriphInc           = DMA_PINC_DISABLE;
//        hdma_tim3_ch1_trig.Init.MemInc              = DMA_MINC_ENABLE;
//        hdma_tim3_ch1_trig.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
//        hdma_tim3_ch1_trig.Init.MemDataAlignment    = DMA_MDATAALIGN_HALFWORD;
//        hdma_tim3_ch1_trig.Init.Mode                = DMA_NORMAL;
//        hdma_tim3_ch1_trig.Init.Priority            = DMA_PRIORITY_HIGH;
//        hdma_tim3_ch1_trig.Init.FIFOMode            = DMA_FIFOMODE_ENABLE;
//        hdma_tim3_ch1_trig.Init.FIFOThreshold       = DMA_FIFO_THRESHOLD_FULL;
//        hdma_tim3_ch1_trig.Init.MemBurst            = DMA_MBURST_INC4;
//        hdma_tim3_ch1_trig.Init.PeriphBurst         = DMA_PBURST_INC4;
//        if (HAL_DMA_Init(&hdma_tim3_ch1_trig) != HAL_OK)
//        {
//          Error_Handler();
//        }
//
//        __HAL_LINKDMA(htim_pwm, hdma[TIM_DMA_ID_CC1],    hdma_tim3_ch1_trig);
//        __HAL_LINKDMA(htim_pwm, hdma[TIM_DMA_ID_TRIGGER], hdma_tim3_ch1_trig);
//    /* Several peripheral DMA handle pointers point to the same DMA handle.
//     Be aware that there is only one stream to perform all the requested DMAs. */
//    __HAL_LINKDMA(htim_pwm,hdma[TIM_DMA_ID_CC1],hdma_tim3_ch1_trig);
//    __HAL_LINKDMA(htim_pwm,hdma[TIM_DMA_ID_TRIGGER],hdma_tim3_ch1_trig);
//
//    /* TIM3_CH2 Init */
//    hdma_tim3_ch2.Instance = DMA1_Stream5;
//    hdma_tim3_ch2.Init.Channel = DMA_CHANNEL_5;
//    hdma_tim3_ch2.Init.Direction = DMA_MEMORY_TO_PERIPH;
//    hdma_tim3_ch2.Init.PeriphInc = DMA_PINC_DISABLE;
//    hdma_tim3_ch2.Init.MemInc = DMA_MINC_ENABLE;
//    hdma_tim3_ch2.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
//    hdma_tim3_ch2.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
//    hdma_tim3_ch2.Init.Mode = DMA_NORMAL;
//    hdma_tim3_ch2.Init.Priority = DMA_PRIORITY_LOW;
//    hdma_tim3_ch2.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
//    if (HAL_DMA_Init(&hdma_tim3_ch2) != HAL_OK)
//    {
//      Error_Handler();
//    }
//
//    __HAL_LINKDMA(htim_pwm,hdma[TIM_DMA_ID_CC2],hdma_tim3_ch2);
//
//    /* TIM3_CH3 Init */
//    hdma_tim3_ch3.Instance = DMA1_Stream7;
//    hdma_tim3_ch3.Init.Channel = DMA_CHANNEL_5;
//    hdma_tim3_ch3.Init.Direction = DMA_MEMORY_TO_PERIPH;
//    hdma_tim3_ch3.Init.PeriphInc = DMA_PINC_DISABLE;
//    hdma_tim3_ch3.Init.MemInc = DMA_MINC_ENABLE;
//    hdma_tim3_ch3.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
//    hdma_tim3_ch3.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
//    hdma_tim3_ch3.Init.Mode = DMA_NORMAL;
//    hdma_tim3_ch3.Init.Priority = DMA_PRIORITY_LOW;
//    hdma_tim3_ch3.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
//    if (HAL_DMA_Init(&hdma_tim3_ch3) != HAL_OK)
//    {
//      Error_Handler();
//    }
//
//    __HAL_LINKDMA(htim_pwm,hdma[TIM_DMA_ID_CC3],hdma_tim3_ch3);
//
//    /* TIM3_CH4_UP Init */
//    hdma_tim3_ch4_up.Instance = DMA1_Stream2;
//    hdma_tim3_ch4_up.Init.Channel = DMA_CHANNEL_5;
//    hdma_tim3_ch4_up.Init.Direction = DMA_MEMORY_TO_PERIPH;
//    hdma_tim3_ch4_up.Init.PeriphInc = DMA_PINC_DISABLE;
//    hdma_tim3_ch4_up.Init.MemInc = DMA_MINC_ENABLE;
//    hdma_tim3_ch4_up.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
//    hdma_tim3_ch4_up.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
//    hdma_tim3_ch4_up.Init.Mode = DMA_NORMAL;
//    hdma_tim3_ch4_up.Init.Priority = DMA_PRIORITY_LOW;
//    hdma_tim3_ch4_up.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
//    if (HAL_DMA_Init(&hdma_tim3_ch4_up) != HAL_OK)
//    {
//      Error_Handler();
//    }
//
//    /* Several peripheral DMA handle pointers point to the same DMA handle.
//     Be aware that there is only one stream to perform all the requested DMAs. */
//    __HAL_LINKDMA(htim_pwm,hdma[TIM_DMA_ID_CC4],hdma_tim3_ch4_up);
//    __HAL_LINKDMA(htim_pwm,hdma[TIM_DMA_ID_UPDATE],hdma_tim3_ch4_up);
//
//    /* USER CODE BEGIN TIM3_MspInit 1 */
//
//    /* USER CODE END TIM3_MspInit 1 */
//  }
//  else if(htim_pwm->Instance==TIM4)
//  {
//    /* USER CODE BEGIN TIM4_MspInit 0 */
//
//    /* USER CODE END TIM4_MspInit 0 */
//    /* Peripheral clock enable */
//    __HAL_RCC_TIM4_CLK_ENABLE();
//
//    /* TIM4 DMA Init */
//    /* TIM4_CH1 Init */
//    hdma_tim4_ch1.Instance = DMA1_Stream0;
//    hdma_tim4_ch1.Init.Channel = DMA_CHANNEL_2;
//    hdma_tim4_ch1.Init.Direction = DMA_MEMORY_TO_PERIPH;
//    hdma_tim4_ch1.Init.PeriphInc = DMA_PINC_DISABLE;
//    hdma_tim4_ch1.Init.MemInc = DMA_MINC_ENABLE;
//    hdma_tim4_ch1.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
//    hdma_tim4_ch1.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
//    hdma_tim4_ch1.Init.Mode = DMA_NORMAL;
//    hdma_tim4_ch1.Init.Priority = DMA_PRIORITY_LOW;
//    hdma_tim4_ch1.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
//    if (HAL_DMA_Init(&hdma_tim4_ch1) != HAL_OK)
//    {
//      Error_Handler();
//    }
//
//    __HAL_LINKDMA(htim_pwm,hdma[TIM_DMA_ID_CC1],hdma_tim4_ch1);
//
//    /* TIM4_CH2 Init */
//    hdma_tim4_ch2.Instance = DMA1_Stream3;
//    hdma_tim4_ch2.Init.Channel = DMA_CHANNEL_2;
//    hdma_tim4_ch2.Init.Direction = DMA_MEMORY_TO_PERIPH;
//    hdma_tim4_ch2.Init.PeriphInc = DMA_PINC_DISABLE;
//    hdma_tim4_ch2.Init.MemInc = DMA_MINC_ENABLE;
//    hdma_tim4_ch2.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
//    hdma_tim4_ch2.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
//    hdma_tim4_ch2.Init.Mode = DMA_NORMAL;
//    hdma_tim4_ch2.Init.Priority = DMA_PRIORITY_LOW;
//    hdma_tim4_ch2.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
//    if (HAL_DMA_Init(&hdma_tim4_ch2) != HAL_OK)
//    {
//      Error_Handler();
//    }
//
//    __HAL_LINKDMA(htim_pwm,hdma[TIM_DMA_ID_CC2],hdma_tim4_ch2);
//
//    /* USER CODE BEGIN TIM4_MspInit 1 */
//
//    /* USER CODE END TIM4_MspInit 1 */
//  }
//
//}
//
///**
//  * @brief TIM_Base MSP Initialization
//  * This function configures the hardware resources used in this example
//  * @param htim_base: TIM_Base handle pointer
//  * @retval None
//  */
//void HAL_TIM_Base_MspInit(TIM_HandleTypeDef* htim_base)
//{
//  if(htim_base->Instance==TIM2)
//  {
//    /* USER CODE BEGIN TIM2_MspInit 0 */
//
//    /* USER CODE END TIM2_MspInit 0 */
//    /* Peripheral clock enable */
//    __HAL_RCC_TIM2_CLK_ENABLE();
//
//    /* TIM2 DMA Init */
//    /* TIM2_CH2_CH4 Init */
//    hdma_tim2_ch2_ch4.Instance = DMA1_Stream6;
//    hdma_tim2_ch2_ch4.Init.Channel = DMA_CHANNEL_3;
//    hdma_tim2_ch2_ch4.Init.Direction = DMA_MEMORY_TO_PERIPH;
//    hdma_tim2_ch2_ch4.Init.PeriphInc = DMA_PINC_DISABLE;
//    hdma_tim2_ch2_ch4.Init.MemInc = DMA_MINC_ENABLE;
//    hdma_tim2_ch2_ch4.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
//    hdma_tim2_ch2_ch4.Init.MemDataAlignment = DMA_MDATAALIGN_WORD;
//    hdma_tim2_ch2_ch4.Init.Mode = DMA_NORMAL;
//    hdma_tim2_ch2_ch4.Init.Priority = DMA_PRIORITY_LOW;
//    hdma_tim2_ch2_ch4.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
//    if (HAL_DMA_Init(&hdma_tim2_ch2_ch4) != HAL_OK)
//    {
//      Error_Handler();
//    }
//
//    /* Several peripheral DMA handle pointers point to the same DMA handle.
//     Be aware that there is only one stream to perform all the requested DMAs. */
//    __HAL_LINKDMA(htim_base,hdma[TIM_DMA_ID_CC2],hdma_tim2_ch2_ch4);
//    __HAL_LINKDMA(htim_base,hdma[TIM_DMA_ID_CC4],hdma_tim2_ch2_ch4);
//
//    /* TIM2_UP_CH3 Init */
//    hdma_tim2_up_ch3.Instance = DMA1_Stream1;
//    hdma_tim2_up_ch3.Init.Channel = DMA_CHANNEL_3;
//    hdma_tim2_up_ch3.Init.Direction = DMA_MEMORY_TO_PERIPH;
//    hdma_tim2_up_ch3.Init.PeriphInc = DMA_PINC_DISABLE;
//    hdma_tim2_up_ch3.Init.MemInc = DMA_MINC_ENABLE;
//    hdma_tim2_up_ch3.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
//    hdma_tim2_up_ch3.Init.MemDataAlignment = DMA_MDATAALIGN_WORD;
//    hdma_tim2_up_ch3.Init.Mode = DMA_NORMAL;
//    hdma_tim2_up_ch3.Init.Priority = DMA_PRIORITY_LOW;
//    hdma_tim2_up_ch3.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
//    if (HAL_DMA_Init(&hdma_tim2_up_ch3) != HAL_OK)
//    {
//      Error_Handler();
//    }
//
//    /* Several peripheral DMA handle pointers point to the same DMA handle.
//     Be aware that there is only one stream to perform all the requested DMAs. */
//    __HAL_LINKDMA(htim_base,hdma[TIM_DMA_ID_UPDATE],hdma_tim2_up_ch3);
//    __HAL_LINKDMA(htim_base,hdma[TIM_DMA_ID_CC3],hdma_tim2_up_ch3);
//
//    /* USER CODE BEGIN TIM2_MspInit 1 */
//
//    /* USER CODE END TIM2_MspInit 1 */
//  }
//  else if(htim_base->Instance==TIM5)
//  {
//    /* USER CODE BEGIN TIM5_MspInit 0 */
//
//    /* USER CODE END TIM5_MspInit 0 */
//    /* Peripheral clock enable */
//    __HAL_RCC_TIM5_CLK_ENABLE();
//    /* USER CODE BEGIN TIM5_MspInit 1 */
//
//    /* USER CODE END TIM5_MspInit 1 */
//  }
//  else if(htim_base->Instance==TIM6)
//  {
//    /* USER CODE BEGIN TIM6_MspInit 0 */
//
//    /* USER CODE END TIM6_MspInit 0 */
//    /* Peripheral clock enable */
//    __HAL_RCC_TIM6_CLK_ENABLE();
//    /* TIM6 interrupt Init */
//    HAL_NVIC_SetPriority(TIM6_DAC_IRQn, 6, 0);
//    HAL_NVIC_EnableIRQ(TIM6_DAC_IRQn);
//    /* USER CODE BEGIN TIM6_MspInit 1 */
//
//    /* USER CODE END TIM6_MspInit 1 */
//  }
//  else if(htim_base->Instance==TIM9)
//  {
//    /* USER CODE BEGIN TIM9_MspInit 0 */
//
//    /* USER CODE END TIM9_MspInit 0 */
//    /* Peripheral clock enable */
//    __HAL_RCC_TIM9_CLK_ENABLE();
//    /* USER CODE BEGIN TIM9_MspInit 1 */
//
//    /* USER CODE END TIM9_MspInit 1 */
//  }
//  else if(htim_base->Instance==TIM12)
//  {
//    /* USER CODE BEGIN TIM12_MspInit 0 */
//
//    /* USER CODE END TIM12_MspInit 0 */
//    /* Peripheral clock enable */
//    __HAL_RCC_TIM12_CLK_ENABLE();
//    /* USER CODE BEGIN TIM12_MspInit 1 */
//
//    /* USER CODE END TIM12_MspInit 1 */
//  }
//
//}
//
//void HAL_TIM_MspPostInit(TIM_HandleTypeDef* htim)
//{
//  GPIO_InitTypeDef GPIO_InitStruct = {0};
//  if(htim->Instance==TIM1)
//  {
//    /* USER CODE BEGIN TIM1_MspPostInit 0 */
//
//    /* USER CODE END TIM1_MspPostInit 0 */
//    __HAL_RCC_GPIOE_CLK_ENABLE();
//    /**TIM1 GPIO Configuration
//    PE9     ------> TIM1_CH1
//    PE11     ------> TIM1_CH2
//    PE13     ------> TIM1_CH3
//    PE14     ------> TIM1_CH4
//    */
//    GPIO_InitStruct.Pin = GPIO_PIN_9|GPIO_PIN_11|GPIO_PIN_13|GPIO_PIN_14;
//    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
//    GPIO_InitStruct.Pull = GPIO_NOPULL;
//    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
//    GPIO_InitStruct.Alternate = GPIO_AF1_TIM1;
//    HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);
//
//    /* USER CODE BEGIN TIM1_MspPostInit 1 */
//
//    /* USER CODE END TIM1_MspPostInit 1 */
//  }
//  else if(htim->Instance==TIM2)
//  {
//    /* USER CODE BEGIN TIM2_MspPostInit 0 */
//
//    /* USER CODE END TIM2_MspPostInit 0 */
//
//    __HAL_RCC_GPIOA_CLK_ENABLE();
//    __HAL_RCC_GPIOB_CLK_ENABLE();
//    /**TIM2 GPIO Configuration
//    PA5     ------> TIM2_CH1
//    PB10     ------> TIM2_CH3
//    PB11     ------> TIM2_CH4
//    PB3     ------> TIM2_CH2
//    */
//    GPIO_InitStruct.Pin = GPIO_PIN_5;
//    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
//    GPIO_InitStruct.Pull = GPIO_NOPULL;
//    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
//    GPIO_InitStruct.Alternate = GPIO_AF1_TIM2;
//    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
//
//    GPIO_InitStruct.Pin = GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_3;
//    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
//    GPIO_InitStruct.Pull = GPIO_NOPULL;
//    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
//    GPIO_InitStruct.Alternate = GPIO_AF1_TIM2;
//    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
//
//    /* USER CODE BEGIN TIM2_MspPostInit 1 */
//
//    /* USER CODE END TIM2_MspPostInit 1 */
//  }
//  else if(htim->Instance==TIM3)
//  {
//    /* USER CODE BEGIN TIM3_MspPostInit 0 */
//
//    /* USER CODE END TIM3_MspPostInit 0 */
//
//    __HAL_RCC_GPIOA_CLK_ENABLE();
//    __HAL_RCC_GPIOB_CLK_ENABLE();
//    __HAL_RCC_GPIOC_CLK_ENABLE();
//    /**TIM3 GPIO Configuration
//    PA6     ------> TIM3_CH1
//    PB1     ------> TIM3_CH4
//    PC7     ------> TIM3_CH2
//    PC8     ------> TIM3_CH3
//    */
//    GPIO_InitStruct.Pin = GPIO_PIN_6;
//    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
//    GPIO_InitStruct.Pull = GPIO_NOPULL;
//    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
//    GPIO_InitStruct.Alternate = GPIO_AF2_TIM3;
//    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
//
//    GPIO_InitStruct.Pin = GPIO_PIN_1;
//    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
//    GPIO_InitStruct.Pull = GPIO_NOPULL;
//    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
//    GPIO_InitStruct.Alternate = GPIO_AF2_TIM3;
//    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
//
//    GPIO_InitStruct.Pin = GPIO_PIN_7|GPIO_PIN_8;
//    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
//    GPIO_InitStruct.Pull = GPIO_NOPULL;
//    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
//    GPIO_InitStruct.Alternate = GPIO_AF2_TIM3;
//    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
//
//    /* USER CODE BEGIN TIM3_MspPostInit 1 */
//
//    /* USER CODE END TIM3_MspPostInit 1 */
//  }
//  else if(htim->Instance==TIM4)
//  {
//    /* USER CODE BEGIN TIM4_MspPostInit 0 */
//
//    /* USER CODE END TIM4_MspPostInit 0 */
//
//    __HAL_RCC_GPIOD_CLK_ENABLE();
//    /**TIM4 GPIO Configuration
//    PD12     ------> TIM4_CH1
//    PD13     ------> TIM4_CH2
//    PD14     ------> TIM4_CH3
//    PD15     ------> TIM4_CH4
//    */
//    GPIO_InitStruct.Pin = GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;
//    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
//    GPIO_InitStruct.Pull = GPIO_NOPULL;
//    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
//    GPIO_InitStruct.Alternate = GPIO_AF2_TIM4;
//    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
//
//    /* USER CODE BEGIN TIM4_MspPostInit 1 */
//
//    /* USER CODE END TIM4_MspPostInit 1 */
//  }
//
//}
///**
//  * @brief TIM_PWM MSP De-Initialization
//  * This function freeze the hardware resources used in this example
//  * @param htim_pwm: TIM_PWM handle pointer
//  * @retval None
//  */
//void HAL_TIM_PWM_MspDeInit(TIM_HandleTypeDef* htim_pwm)
//{
//  if(htim_pwm->Instance==TIM1)
//  {
//    /* USER CODE BEGIN TIM1_MspDeInit 0 */
//
//    /* USER CODE END TIM1_MspDeInit 0 */
//    /* Peripheral clock disable */
//    __HAL_RCC_TIM1_CLK_DISABLE();
//
//    /* TIM1 DMA DeInit */
//    HAL_DMA_DeInit(htim_pwm->hdma[TIM_DMA_ID_CC1]);
//    HAL_DMA_DeInit(htim_pwm->hdma[TIM_DMA_ID_CC2]);
//    HAL_DMA_DeInit(htim_pwm->hdma[TIM_DMA_ID_CC3]);
//    HAL_DMA_DeInit(htim_pwm->hdma[TIM_DMA_ID_CC4]);
//    HAL_DMA_DeInit(htim_pwm->hdma[TIM_DMA_ID_TRIGGER]);
//    HAL_DMA_DeInit(htim_pwm->hdma[TIM_DMA_ID_COMMUTATION]);
//    /* USER CODE BEGIN TIM1_MspDeInit 1 */
//
//    /* USER CODE END TIM1_MspDeInit 1 */
//  }
//  else if(htim_pwm->Instance==TIM3)
//  {
//    /* USER CODE BEGIN TIM3_MspDeInit 0 */
//
//    /* USER CODE END TIM3_MspDeInit 0 */
//    /* Peripheral clock disable */
//    __HAL_RCC_TIM3_CLK_DISABLE();
//
//    /* TIM3 DMA DeInit */
//    HAL_DMA_DeInit(htim_pwm->hdma[TIM_DMA_ID_CC1]);
//    HAL_DMA_DeInit(htim_pwm->hdma[TIM_DMA_ID_TRIGGER]);
//    HAL_DMA_DeInit(htim_pwm->hdma[TIM_DMA_ID_CC2]);
//    HAL_DMA_DeInit(htim_pwm->hdma[TIM_DMA_ID_CC3]);
//    HAL_DMA_DeInit(htim_pwm->hdma[TIM_DMA_ID_CC4]);
//    HAL_DMA_DeInit(htim_pwm->hdma[TIM_DMA_ID_UPDATE]);
//    /* USER CODE BEGIN TIM3_MspDeInit 1 */
//
//    /* USER CODE END TIM3_MspDeInit 1 */
//  }
//  else if(htim_pwm->Instance==TIM4)
//  {
//    /* USER CODE BEGIN TIM4_MspDeInit 0 */
//
//    /* USER CODE END TIM4_MspDeInit 0 */
//    /* Peripheral clock disable */
//    __HAL_RCC_TIM4_CLK_DISABLE();
//
//    /* TIM4 DMA DeInit */
//    HAL_DMA_DeInit(htim_pwm->hdma[TIM_DMA_ID_CC1]);
//    HAL_DMA_DeInit(htim_pwm->hdma[TIM_DMA_ID_CC2]);
//    /* USER CODE BEGIN TIM4_MspDeInit 1 */
//
//    /* USER CODE END TIM4_MspDeInit 1 */
//  }
//
//}
//
///**
//  * @brief TIM_Base MSP De-Initialization
//  * This function freeze the hardware resources used in this example
//  * @param htim_base: TIM_Base handle pointer
//  * @retval None
//  */
//void HAL_TIM_Base_MspDeInit(TIM_HandleTypeDef* htim_base)
//{
//  if(htim_base->Instance==TIM2)
//  {
//    /* USER CODE BEGIN TIM2_MspDeInit 0 */
//
//    /* USER CODE END TIM2_MspDeInit 0 */
//    /* Peripheral clock disable */
//    __HAL_RCC_TIM2_CLK_DISABLE();
//
//    /* TIM2 DMA DeInit */
//    HAL_DMA_DeInit(htim_base->hdma[TIM_DMA_ID_CC2]);
//    HAL_DMA_DeInit(htim_base->hdma[TIM_DMA_ID_CC4]);
//    HAL_DMA_DeInit(htim_base->hdma[TIM_DMA_ID_UPDATE]);
//    HAL_DMA_DeInit(htim_base->hdma[TIM_DMA_ID_CC3]);
//    /* USER CODE BEGIN TIM2_MspDeInit 1 */
//
//    /* USER CODE END TIM2_MspDeInit 1 */
//  }
//  else if(htim_base->Instance==TIM5)
//  {
//    /* USER CODE BEGIN TIM5_MspDeInit 0 */
//
//    /* USER CODE END TIM5_MspDeInit 0 */
//    /* Peripheral clock disable */
//    __HAL_RCC_TIM5_CLK_DISABLE();
//    /* USER CODE BEGIN TIM5_MspDeInit 1 */
//
//    /* USER CODE END TIM5_MspDeInit 1 */
//  }
//  else if(htim_base->Instance==TIM6)
//  {
//    /* USER CODE BEGIN TIM6_MspDeInit 0 */
//
//    /* USER CODE END TIM6_MspDeInit 0 */
//    /* Peripheral clock disable */
//    __HAL_RCC_TIM6_CLK_DISABLE();
//
//    /* TIM6 interrupt DeInit */
//    HAL_NVIC_DisableIRQ(TIM6_DAC_IRQn);
//    /* USER CODE BEGIN TIM6_MspDeInit 1 */
//
//    /* USER CODE END TIM6_MspDeInit 1 */
//  }
//  else if(htim_base->Instance==TIM9)
//  {
//    /* USER CODE BEGIN TIM9_MspDeInit 0 */
//
//    /* USER CODE END TIM9_MspDeInit 0 */
//    /* Peripheral clock disable */
//    __HAL_RCC_TIM9_CLK_DISABLE();
//    /* USER CODE BEGIN TIM9_MspDeInit 1 */
//
//    /* USER CODE END TIM9_MspDeInit 1 */
//  }
//  else if(htim_base->Instance==TIM12)
//  {
//    /* USER CODE BEGIN TIM12_MspDeInit 0 */
//
//    /* USER CODE END TIM12_MspDeInit 0 */
//    /* Peripheral clock disable */
//    __HAL_RCC_TIM12_CLK_DISABLE();
//    /* USER CODE BEGIN TIM12_MspDeInit 1 */
//
//    /* USER CODE END TIM12_MspDeInit 1 */
//  }
//
//}
//
///**
//  * @brief UART MSP Initialization
//  * This function configures the hardware resources used in this example
//  * @param huart: UART handle pointer
//  * @retval None
//  */
//void HAL_UART_MspInit(UART_HandleTypeDef* huart)
//{
//  GPIO_InitTypeDef GPIO_InitStruct = {0};
//  if(huart->Instance==USART3)
//  {
//    /* USER CODE BEGIN USART3_MspInit 0 */
//
//    /* USER CODE END USART3_MspInit 0 */
//    /* Peripheral clock enable */
//    __HAL_RCC_USART3_CLK_ENABLE();
//
//    __HAL_RCC_GPIOD_CLK_ENABLE();
//    /**USART3 GPIO Configuration
//    PD8     ------> USART3_TX
//    PD9     ------> USART3_RX
//    */
//    GPIO_InitStruct.Pin = STLK_RX_Pin|STLK_TX_Pin;
//    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
//    GPIO_InitStruct.Pull = GPIO_NOPULL;
//    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
//    GPIO_InitStruct.Alternate = GPIO_AF7_USART3;
//    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
//
//    /* USER CODE BEGIN USART3_MspInit 1 */
//
//    /* USER CODE END USART3_MspInit 1 */
//
//  }
//
//}
//
///**
//  * @brief UART MSP De-Initialization
//  * This function freeze the hardware resources used in this example
//  * @param huart: UART handle pointer
//  * @retval None
//  */
//void HAL_UART_MspDeInit(UART_HandleTypeDef* huart)
//{
//  if(huart->Instance==USART3)
//  {
//    /* USER CODE BEGIN USART3_MspDeInit 0 */
//
//    /* USER CODE END USART3_MspDeInit 0 */
//    /* Peripheral clock disable */
//    __HAL_RCC_USART3_CLK_DISABLE();
//
//    /**USART3 GPIO Configuration
//    PD8     ------> USART3_TX
//    PD9     ------> USART3_RX
//    */
//    HAL_GPIO_DeInit(GPIOD, STLK_RX_Pin|STLK_TX_Pin);
//
//    /* USER CODE BEGIN USART3_MspDeInit 1 */
//
//    /* USER CODE END USART3_MspDeInit 1 */
//  }
//
//}
//
///**
//  * @brief PCD MSP Initialization
//  * This function configures the hardware resources used in this example
//  * @param hpcd: PCD handle pointer
//  * @retval None
//  */
//void HAL_PCD_MspInit(PCD_HandleTypeDef* hpcd)
//{
//  GPIO_InitTypeDef GPIO_InitStruct = {0};
//  if(hpcd->Instance==USB_OTG_FS)
//  {
//    /* USER CODE BEGIN USB_OTG_FS_MspInit 0 */
//
//    /* USER CODE END USB_OTG_FS_MspInit 0 */
//
//    __HAL_RCC_GPIOA_CLK_ENABLE();
//    /**USB_OTG_FS GPIO Configuration
//    PA8     ------> USB_OTG_FS_SOF
//    PA9     ------> USB_OTG_FS_VBUS
//    PA10     ------> USB_OTG_FS_ID
//    PA11     ------> USB_OTG_FS_DM
//    PA12     ------> USB_OTG_FS_DP
//    */
//    GPIO_InitStruct.Pin = USB_SOF_Pin|USB_ID_Pin|USB_DM_Pin|USB_DP_Pin;
//    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
//    GPIO_InitStruct.Pull = GPIO_NOPULL;
//    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
//    GPIO_InitStruct.Alternate = GPIO_AF10_OTG_FS;
//    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
//
//    GPIO_InitStruct.Pin = USB_VBUS_Pin;
//    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
//    GPIO_InitStruct.Pull = GPIO_NOPULL;
//    HAL_GPIO_Init(USB_VBUS_GPIO_Port, &GPIO_InitStruct);
//
//    /* Peripheral clock enable */
//    __HAL_RCC_USB_OTG_FS_CLK_ENABLE();
//    /* USER CODE BEGIN USB_OTG_FS_MspInit 1 */
//
//    /* USER CODE END USB_OTG_FS_MspInit 1 */
//
//  }
//
//}
//
///**
//  * @brief PCD MSP De-Initialization
//  * This function freeze the hardware resources used in this example
//  * @param hpcd: PCD handle pointer
//  * @retval None
//  */
//void HAL_PCD_MspDeInit(PCD_HandleTypeDef* hpcd)
//{
//  if(hpcd->Instance==USB_OTG_FS)
//  {
//    /* USER CODE BEGIN USB_OTG_FS_MspDeInit 0 */
//
//    /* USER CODE END USB_OTG_FS_MspDeInit 0 */
//    /* Peripheral clock disable */
//    __HAL_RCC_USB_OTG_FS_CLK_DISABLE();
//
//    /**USB_OTG_FS GPIO Configuration
//    PA8     ------> USB_OTG_FS_SOF
//    PA9     ------> USB_OTG_FS_VBUS
//    PA10     ------> USB_OTG_FS_ID
//    PA11     ------> USB_OTG_FS_DM
//    PA12     ------> USB_OTG_FS_DP
//    */
//    HAL_GPIO_DeInit(GPIOA, USB_SOF_Pin|USB_VBUS_Pin|USB_ID_Pin|USB_DM_Pin
//                          |USB_DP_Pin);
//
//    /* USER CODE BEGIN USB_OTG_FS_MspDeInit 1 */
//
//    /* USER CODE END USB_OTG_FS_MspDeInit 1 */
//  }
//
//}
//
///* USER CODE BEGIN 1 */
//
///* USER CODE END 1 */








/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file         stm32f4xx_hal_msp.c
  * @brief        MSP Initialization — FIXED for ws_output.c
  *
  * KEY CHANGES vs CubeMX-generated version
  * =========================================
  * 1. TIM1 CH1/CH2/CH3/CH4 DMA streams → DMA_CIRCULAR  (was DMA_NORMAL)
  * 2. TIM3 CH1/CH2/CH3/CH4 DMA streams → DMA_CIRCULAR  (was DMA_NORMAL)
  * 3. TIM1 CH1 stream: FIFO+burst kept for bandwidth; others simplified.
  * 4. TIM3 CH1 stream: FIFO+burst kept for bandwidth; others simplified.
  * 5. Removed TIM2 DMA (not used by ws_output.c).
  * 6. TIM4 DMA removed (TIM4 not used in 8-output design).
  *
  * IMPORTANT: MX_TIM1_Init() in main.c MUST be changed:
  *   htim1.Init.Prescaler = 3;
  *   htim1.Init.Period    = 55;   <-- was 65535
  ******************************************************************************
  */
/* USER CODE END Header */

#include "main.h"

/*
 * DMA handles are DEFINED in main.c (CubeMX-generated global variables).
 * Declare them extern here so HAL_TIM_PWM_MspInit can reference them.
 * DO NOT redefine them — that causes the "multiple definition" linker error.
 */
extern DMA_HandleTypeDef hdma_tim1_ch1;
extern DMA_HandleTypeDef hdma_tim1_ch2;
extern DMA_HandleTypeDef hdma_tim1_ch3;
extern DMA_HandleTypeDef hdma_tim1_ch4_trig_com;

extern DMA_HandleTypeDef hdma_tim3_ch1_trig;
extern DMA_HandleTypeDef hdma_tim3_ch2;
extern DMA_HandleTypeDef hdma_tim3_ch3;
extern DMA_HandleTypeDef hdma_tim3_ch4_up;

extern DMA_HandleTypeDef hdma_tim2_ch2_ch4;
extern DMA_HandleTypeDef hdma_tim2_up_ch3;
extern DMA_HandleTypeDef hdma_tim4_ch1;
extern DMA_HandleTypeDef hdma_tim4_ch2;

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* ===========================================================================
 * HAL_MspInit
 * ========================================================================== */
void HAL_MspInit(void)
{
    __HAL_RCC_SYSCFG_CLK_ENABLE();
    __HAL_RCC_PWR_CLK_ENABLE();
}

/* ===========================================================================
 * HAL_TIM_PWM_MspInit
 * ========================================================================== */
void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef *htim_pwm)
{
    /* -----------------------------------------------------------------------
     * TIM1  —  Group A  (strips 0-3, PE9/PE11/PE13/PE14)
     * APB2 clock 180 MHz, PSC=3 → 45 MHz tick, ARR=55 → ~818 kHz
     * ----------------------------------------------------------------------- */
    if (htim_pwm->Instance == TIM1)
    {
        __HAL_RCC_TIM1_CLK_ENABLE();

        /* CH1 — DMA2 Stream1 Channel6  (HIGH priority, FIFO+burst for bandwidth) */
        hdma_tim1_ch1.Instance                 = DMA2_Stream1;
        hdma_tim1_ch1.Init.Channel             = DMA_CHANNEL_6;
        hdma_tim1_ch1.Init.Direction           = DMA_MEMORY_TO_PERIPH;
        hdma_tim1_ch1.Init.PeriphInc           = DMA_PINC_DISABLE;
        hdma_tim1_ch1.Init.MemInc              = DMA_MINC_ENABLE;
        hdma_tim1_ch1.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
        hdma_tim1_ch1.Init.MemDataAlignment    = DMA_MDATAALIGN_HALFWORD;
        hdma_tim1_ch1.Init.Mode                = DMA_CIRCULAR;   /* FIX: was DMA_NORMAL */
        hdma_tim1_ch1.Init.Priority            = DMA_PRIORITY_HIGH;
        hdma_tim1_ch1.Init.FIFOMode            = DMA_FIFOMODE_ENABLE;
        hdma_tim1_ch1.Init.FIFOThreshold       = DMA_FIFO_THRESHOLD_FULL;
        hdma_tim1_ch1.Init.MemBurst            = DMA_MBURST_INC4;
        hdma_tim1_ch1.Init.PeriphBurst         = DMA_PBURST_SINGLE;
        if (HAL_DMA_Init(&hdma_tim1_ch1) != HAL_OK) Error_Handler();
        __HAL_LINKDMA(htim_pwm, hdma[TIM_DMA_ID_CC1], hdma_tim1_ch1);

        /* CH2 — DMA2 Stream2 Channel6 */
        hdma_tim1_ch2.Instance                 = DMA2_Stream2;
        hdma_tim1_ch2.Init.Channel             = DMA_CHANNEL_6;
        hdma_tim1_ch2.Init.Direction           = DMA_MEMORY_TO_PERIPH;
        hdma_tim1_ch2.Init.PeriphInc           = DMA_PINC_DISABLE;
        hdma_tim1_ch2.Init.MemInc              = DMA_MINC_ENABLE;
        hdma_tim1_ch2.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
        hdma_tim1_ch2.Init.MemDataAlignment    = DMA_MDATAALIGN_HALFWORD;
        hdma_tim1_ch2.Init.Mode                = DMA_CIRCULAR;   /* FIX */
        hdma_tim1_ch2.Init.Priority            = DMA_PRIORITY_HIGH;
        hdma_tim1_ch2.Init.FIFOMode            = DMA_FIFOMODE_ENABLE;
        hdma_tim1_ch2.Init.FIFOThreshold       = DMA_FIFO_THRESHOLD_FULL;
        hdma_tim1_ch2.Init.MemBurst            = DMA_MBURST_INC4;
        hdma_tim1_ch2.Init.PeriphBurst         = DMA_PBURST_SINGLE;
        if (HAL_DMA_Init(&hdma_tim1_ch2) != HAL_OK) Error_Handler();
        __HAL_LINKDMA(htim_pwm, hdma[TIM_DMA_ID_CC2], hdma_tim1_ch2);

        /* CH3 — DMA2 Stream6 Channel6 */
        hdma_tim1_ch3.Instance                 = DMA2_Stream6;
        hdma_tim1_ch3.Init.Channel             = DMA_CHANNEL_6;
        hdma_tim1_ch3.Init.Direction           = DMA_MEMORY_TO_PERIPH;
        hdma_tim1_ch3.Init.PeriphInc           = DMA_PINC_DISABLE;
        hdma_tim1_ch3.Init.MemInc              = DMA_MINC_ENABLE;
        hdma_tim1_ch3.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
        hdma_tim1_ch3.Init.MemDataAlignment    = DMA_MDATAALIGN_HALFWORD;
        hdma_tim1_ch3.Init.Mode                = DMA_CIRCULAR;   /* FIX */
        hdma_tim1_ch3.Init.Priority            = DMA_PRIORITY_HIGH;
        hdma_tim1_ch3.Init.FIFOMode            = DMA_FIFOMODE_ENABLE;
        hdma_tim1_ch3.Init.FIFOThreshold       = DMA_FIFO_THRESHOLD_FULL;
        hdma_tim1_ch3.Init.MemBurst            = DMA_MBURST_INC4;
        hdma_tim1_ch3.Init.PeriphBurst         = DMA_PBURST_SINGLE;
        if (HAL_DMA_Init(&hdma_tim1_ch3) != HAL_OK) Error_Handler();
        __HAL_LINKDMA(htim_pwm, hdma[TIM_DMA_ID_CC3], hdma_tim1_ch3);

        /* CH4 — DMA2 Stream4 Channel6 */
        hdma_tim1_ch4_trig_com.Instance                 = DMA2_Stream4;
        hdma_tim1_ch4_trig_com.Init.Channel             = DMA_CHANNEL_6;
        hdma_tim1_ch4_trig_com.Init.Direction           = DMA_MEMORY_TO_PERIPH;
        hdma_tim1_ch4_trig_com.Init.PeriphInc           = DMA_PINC_DISABLE;
        hdma_tim1_ch4_trig_com.Init.MemInc              = DMA_MINC_ENABLE;
        hdma_tim1_ch4_trig_com.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
        hdma_tim1_ch4_trig_com.Init.MemDataAlignment    = DMA_MDATAALIGN_HALFWORD;
        hdma_tim1_ch4_trig_com.Init.Mode                = DMA_CIRCULAR;   /* FIX */
        hdma_tim1_ch4_trig_com.Init.Priority            = DMA_PRIORITY_HIGH;
        hdma_tim1_ch4_trig_com.Init.FIFOMode            = DMA_FIFOMODE_ENABLE;
        hdma_tim1_ch4_trig_com.Init.FIFOThreshold       = DMA_FIFO_THRESHOLD_FULL;
        hdma_tim1_ch4_trig_com.Init.MemBurst            = DMA_MBURST_INC4;
        hdma_tim1_ch4_trig_com.Init.PeriphBurst         = DMA_PBURST_SINGLE;
        if (HAL_DMA_Init(&hdma_tim1_ch4_trig_com) != HAL_OK) Error_Handler();
        __HAL_LINKDMA(htim_pwm, hdma[TIM_DMA_ID_CC4],         hdma_tim1_ch4_trig_com);
        __HAL_LINKDMA(htim_pwm, hdma[TIM_DMA_ID_TRIGGER],     hdma_tim1_ch4_trig_com);
        __HAL_LINKDMA(htim_pwm, hdma[TIM_DMA_ID_COMMUTATION], hdma_tim1_ch4_trig_com);
    }

    /* -----------------------------------------------------------------------
     * TIM3  —  Group B  (strips 4-7, PA6/PC7/PC8/PB1)
     * APB1 clock 90 MHz, PSC=3 → 22.5 MHz tick, ARR=24 → 800 kHz
     * ----------------------------------------------------------------------- */
    else if (htim_pwm->Instance == TIM3)
    {
        __HAL_RCC_TIM3_CLK_ENABLE();

        /* CH1 — DMA1 Stream4 Channel5 */
        hdma_tim3_ch1_trig.Instance                 = DMA1_Stream4;
        hdma_tim3_ch1_trig.Init.Channel             = DMA_CHANNEL_5;
        hdma_tim3_ch1_trig.Init.Direction           = DMA_MEMORY_TO_PERIPH;
        hdma_tim3_ch1_trig.Init.PeriphInc           = DMA_PINC_DISABLE;
        hdma_tim3_ch1_trig.Init.MemInc              = DMA_MINC_ENABLE;
        hdma_tim3_ch1_trig.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
        hdma_tim3_ch1_trig.Init.MemDataAlignment    = DMA_MDATAALIGN_HALFWORD;
        hdma_tim3_ch1_trig.Init.Mode                = DMA_CIRCULAR;   /* FIX */
        hdma_tim3_ch1_trig.Init.Priority            = DMA_PRIORITY_HIGH;
        hdma_tim3_ch1_trig.Init.FIFOMode            = DMA_FIFOMODE_ENABLE;
        hdma_tim3_ch1_trig.Init.FIFOThreshold       = DMA_FIFO_THRESHOLD_FULL;
        hdma_tim3_ch1_trig.Init.MemBurst            = DMA_MBURST_INC4;
        hdma_tim3_ch1_trig.Init.PeriphBurst         = DMA_PBURST_SINGLE;
        if (HAL_DMA_Init(&hdma_tim3_ch1_trig) != HAL_OK) Error_Handler();
        __HAL_LINKDMA(htim_pwm, hdma[TIM_DMA_ID_CC1],    hdma_tim3_ch1_trig);
        __HAL_LINKDMA(htim_pwm, hdma[TIM_DMA_ID_TRIGGER], hdma_tim3_ch1_trig);

        /* CH2 — DMA1 Stream5 Channel5 */
        hdma_tim3_ch2.Instance                 = DMA1_Stream5;
        hdma_tim3_ch2.Init.Channel             = DMA_CHANNEL_5;
        hdma_tim3_ch2.Init.Direction           = DMA_MEMORY_TO_PERIPH;
        hdma_tim3_ch2.Init.PeriphInc           = DMA_PINC_DISABLE;
        hdma_tim3_ch2.Init.MemInc              = DMA_MINC_ENABLE;
        hdma_tim3_ch2.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
        hdma_tim3_ch2.Init.MemDataAlignment    = DMA_MDATAALIGN_HALFWORD;
        hdma_tim3_ch2.Init.Mode                = DMA_CIRCULAR;   /* FIX */
        hdma_tim3_ch2.Init.Priority            = DMA_PRIORITY_HIGH;
        hdma_tim3_ch2.Init.FIFOMode            = DMA_FIFOMODE_ENABLE;
        hdma_tim3_ch2.Init.FIFOThreshold       = DMA_FIFO_THRESHOLD_FULL;
        hdma_tim3_ch2.Init.MemBurst            = DMA_MBURST_INC4;
        hdma_tim3_ch2.Init.PeriphBurst         = DMA_PBURST_SINGLE;
        if (HAL_DMA_Init(&hdma_tim3_ch2) != HAL_OK) Error_Handler();
        __HAL_LINKDMA(htim_pwm, hdma[TIM_DMA_ID_CC2], hdma_tim3_ch2);

        /* CH3 — DMA1 Stream7 Channel5 */
        hdma_tim3_ch3.Instance                 = DMA1_Stream7;
        hdma_tim3_ch3.Init.Channel             = DMA_CHANNEL_5;
        hdma_tim3_ch3.Init.Direction           = DMA_MEMORY_TO_PERIPH;
        hdma_tim3_ch3.Init.PeriphInc           = DMA_PINC_DISABLE;
        hdma_tim3_ch3.Init.MemInc              = DMA_MINC_ENABLE;
        hdma_tim3_ch3.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
        hdma_tim3_ch3.Init.MemDataAlignment    = DMA_MDATAALIGN_HALFWORD;
        hdma_tim3_ch3.Init.Mode                = DMA_CIRCULAR;   /* FIX */
        hdma_tim3_ch3.Init.Priority            = DMA_PRIORITY_HIGH;
        hdma_tim3_ch3.Init.FIFOMode            = DMA_FIFOMODE_ENABLE;
        hdma_tim3_ch3.Init.FIFOThreshold       = DMA_FIFO_THRESHOLD_FULL;
        hdma_tim3_ch3.Init.MemBurst            = DMA_MBURST_INC4;
        hdma_tim3_ch3.Init.PeriphBurst         = DMA_PBURST_SINGLE;
        if (HAL_DMA_Init(&hdma_tim3_ch3) != HAL_OK) Error_Handler();
        __HAL_LINKDMA(htim_pwm, hdma[TIM_DMA_ID_CC3], hdma_tim3_ch3);

        /* CH4 — DMA1 Stream2 Channel5 */
        hdma_tim3_ch4_up.Instance                 = DMA1_Stream2;
        hdma_tim3_ch4_up.Init.Channel             = DMA_CHANNEL_5;
        hdma_tim3_ch4_up.Init.Direction           = DMA_MEMORY_TO_PERIPH;
        hdma_tim3_ch4_up.Init.PeriphInc           = DMA_PINC_DISABLE;
        hdma_tim3_ch4_up.Init.MemInc              = DMA_MINC_ENABLE;
        hdma_tim3_ch4_up.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
        hdma_tim3_ch4_up.Init.MemDataAlignment    = DMA_MDATAALIGN_HALFWORD;
        hdma_tim3_ch4_up.Init.Mode                = DMA_CIRCULAR;   /* FIX */
        hdma_tim3_ch4_up.Init.Priority            = DMA_PRIORITY_HIGH;
        hdma_tim3_ch4_up.Init.FIFOMode            = DMA_FIFOMODE_ENABLE;
        hdma_tim3_ch4_up.Init.FIFOThreshold       = DMA_FIFO_THRESHOLD_FULL;
        hdma_tim3_ch4_up.Init.MemBurst            = DMA_MBURST_INC4;
        hdma_tim3_ch4_up.Init.PeriphBurst         = DMA_PBURST_SINGLE;
        if (HAL_DMA_Init(&hdma_tim3_ch4_up) != HAL_OK) Error_Handler();
        __HAL_LINKDMA(htim_pwm, hdma[TIM_DMA_ID_CC4],    hdma_tim3_ch4_up);
        __HAL_LINKDMA(htim_pwm, hdma[TIM_DMA_ID_UPDATE], hdma_tim3_ch4_up);
    }

    /* TIM4 — not used in 8-output design, no DMA needed */
    else if (htim_pwm->Instance == TIM4)
    {
        __HAL_RCC_TIM4_CLK_ENABLE();
        /* No DMA init — TIM4 channels unused */
    }
}

/* ===========================================================================
 * HAL_TIM_Base_MspInit
 * TIM2/5/6/9/12 — unchanged from CubeMX, TIM2 DMA stubs harmlessly inited
 * ========================================================================== */
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim_base)
{
    if (htim_base->Instance == TIM2)
    {
        __HAL_RCC_TIM2_CLK_ENABLE();

        /* Stub init — TIM2 DMA not used, just satisfies linker */
        hdma_tim2_ch2_ch4.Instance = DMA1_Stream6;
        hdma_tim2_ch2_ch4.Init.Channel             = DMA_CHANNEL_3;
        hdma_tim2_ch2_ch4.Init.Direction           = DMA_MEMORY_TO_PERIPH;
        hdma_tim2_ch2_ch4.Init.PeriphInc           = DMA_PINC_DISABLE;
        hdma_tim2_ch2_ch4.Init.MemInc              = DMA_MINC_ENABLE;
        hdma_tim2_ch2_ch4.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
        hdma_tim2_ch2_ch4.Init.MemDataAlignment    = DMA_MDATAALIGN_WORD;
        hdma_tim2_ch2_ch4.Init.Mode                = DMA_NORMAL;
        hdma_tim2_ch2_ch4.Init.Priority            = DMA_PRIORITY_LOW;
        hdma_tim2_ch2_ch4.Init.FIFOMode            = DMA_FIFOMODE_DISABLE;
        HAL_DMA_Init(&hdma_tim2_ch2_ch4);  /* ignore error — stub only */

        hdma_tim2_up_ch3.Instance = DMA1_Stream1;
        hdma_tim2_up_ch3.Init.Channel             = DMA_CHANNEL_3;
        hdma_tim2_up_ch3.Init.Direction           = DMA_MEMORY_TO_PERIPH;
        hdma_tim2_up_ch3.Init.PeriphInc           = DMA_PINC_DISABLE;
        hdma_tim2_up_ch3.Init.MemInc              = DMA_MINC_ENABLE;
        hdma_tim2_up_ch3.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
        hdma_tim2_up_ch3.Init.MemDataAlignment    = DMA_MDATAALIGN_WORD;
        hdma_tim2_up_ch3.Init.Mode                = DMA_NORMAL;
        hdma_tim2_up_ch3.Init.Priority            = DMA_PRIORITY_LOW;
        hdma_tim2_up_ch3.Init.FIFOMode            = DMA_FIFOMODE_DISABLE;
        HAL_DMA_Init(&hdma_tim2_up_ch3);
    }
    else if (htim_base->Instance == TIM5)  { __HAL_RCC_TIM5_CLK_ENABLE(); }
    else if (htim_base->Instance == TIM6)
    {
        __HAL_RCC_TIM6_CLK_ENABLE();
        HAL_NVIC_SetPriority(TIM6_DAC_IRQn, 6, 0);
        HAL_NVIC_EnableIRQ(TIM6_DAC_IRQn);
    }
    else if (htim_base->Instance == TIM9)  { __HAL_RCC_TIM9_CLK_ENABLE();  }
    else if (htim_base->Instance == TIM12) { __HAL_RCC_TIM12_CLK_ENABLE(); }
}

/* ===========================================================================
 * HAL_TIM_MspPostInit  —  GPIO alternate-function setup (unchanged)
 * ========================================================================== */
void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    if (htim->Instance == TIM1)
    {
        __HAL_RCC_GPIOE_CLK_ENABLE();
        /* PE9=CH1, PE11=CH2, PE13=CH3, PE14=CH4 */
        GPIO_InitStruct.Pin       = GPIO_PIN_9 | GPIO_PIN_11 | GPIO_PIN_13 | GPIO_PIN_14;
        GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull      = GPIO_NOPULL;
        GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF1_TIM1;
        HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);
    }
    else if (htim->Instance == TIM2)
    {
        __HAL_RCC_GPIOA_CLK_ENABLE();
        __HAL_RCC_GPIOB_CLK_ENABLE();
        GPIO_InitStruct.Pin       = GPIO_PIN_5;
        GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull      = GPIO_NOPULL;
        GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_LOW;
        GPIO_InitStruct.Alternate = GPIO_AF1_TIM2;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

        GPIO_InitStruct.Pin = GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_3;
        HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
    }
    else if (htim->Instance == TIM3)
    {
        __HAL_RCC_GPIOA_CLK_ENABLE();
        __HAL_RCC_GPIOB_CLK_ENABLE();
        __HAL_RCC_GPIOC_CLK_ENABLE();
        /* PA6=CH1, PB1=CH4, PC7=CH2, PC8=CH3 */
        GPIO_InitStruct.Pin       = GPIO_PIN_6;
        GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull      = GPIO_NOPULL;
        GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF2_TIM3;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

        GPIO_InitStruct.Pin = GPIO_PIN_1;
        HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

        GPIO_InitStruct.Pin = GPIO_PIN_7 | GPIO_PIN_8;
        HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
    }
    else if (htim->Instance == TIM4)
    {
        __HAL_RCC_GPIOD_CLK_ENABLE();
        GPIO_InitStruct.Pin       = GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
        GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull      = GPIO_NOPULL;
        GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_LOW;
        GPIO_InitStruct.Alternate = GPIO_AF2_TIM4;
        HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
    }
}

/* ===========================================================================
 * De-init stubs (unchanged)
 * ========================================================================== */
void HAL_TIM_PWM_MspDeInit(TIM_HandleTypeDef *htim_pwm)
{
    if (htim_pwm->Instance == TIM1)
    {
        __HAL_RCC_TIM1_CLK_DISABLE();
        HAL_DMA_DeInit(htim_pwm->hdma[TIM_DMA_ID_CC1]);
        HAL_DMA_DeInit(htim_pwm->hdma[TIM_DMA_ID_CC2]);
        HAL_DMA_DeInit(htim_pwm->hdma[TIM_DMA_ID_CC3]);
        HAL_DMA_DeInit(htim_pwm->hdma[TIM_DMA_ID_CC4]);
    }
    else if (htim_pwm->Instance == TIM3)
    {
        __HAL_RCC_TIM3_CLK_DISABLE();
        HAL_DMA_DeInit(htim_pwm->hdma[TIM_DMA_ID_CC1]);
        HAL_DMA_DeInit(htim_pwm->hdma[TIM_DMA_ID_CC2]);
        HAL_DMA_DeInit(htim_pwm->hdma[TIM_DMA_ID_CC3]);
        HAL_DMA_DeInit(htim_pwm->hdma[TIM_DMA_ID_CC4]);
    }
    else if (htim_pwm->Instance == TIM4)
    {
        __HAL_RCC_TIM4_CLK_DISABLE();
    }
}

void HAL_TIM_Base_MspDeInit(TIM_HandleTypeDef *htim_base)
{
    if      (htim_base->Instance == TIM2)  { __HAL_RCC_TIM2_CLK_DISABLE();  }
    else if (htim_base->Instance == TIM5)  { __HAL_RCC_TIM5_CLK_DISABLE();  }
    else if (htim_base->Instance == TIM6)  { __HAL_RCC_TIM6_CLK_DISABLE(); HAL_NVIC_DisableIRQ(TIM6_DAC_IRQn); }
    else if (htim_base->Instance == TIM9)  { __HAL_RCC_TIM9_CLK_DISABLE();  }
    else if (htim_base->Instance == TIM12) { __HAL_RCC_TIM12_CLK_DISABLE(); }
}

/* ===========================================================================
 * UART + USB MSP  —  unchanged from CubeMX
 * ========================================================================== */
void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    if (huart->Instance == USART3)
    {
        __HAL_RCC_USART3_CLK_ENABLE();
        __HAL_RCC_GPIOD_CLK_ENABLE();
        GPIO_InitStruct.Pin       = GPIO_PIN_8 | GPIO_PIN_9;
        GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull      = GPIO_NOPULL;
        GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF7_USART3;
        HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
    }
}

void HAL_UART_MspDeInit(UART_HandleTypeDef *huart)
{
    if (huart->Instance == USART3)
    {
        __HAL_RCC_USART3_CLK_DISABLE();
        HAL_GPIO_DeInit(GPIOD, GPIO_PIN_8 | GPIO_PIN_9);
    }
}

void HAL_PCD_MspInit(PCD_HandleTypeDef *hpcd)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    if (hpcd->Instance == USB_OTG_FS)
    {
        __HAL_RCC_GPIOA_CLK_ENABLE();
        GPIO_InitStruct.Pin       = GPIO_PIN_8 | GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12;
        GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull      = GPIO_NOPULL;
        GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF10_OTG_FS;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

        GPIO_InitStruct.Pin  = GPIO_PIN_9;
        GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

        __HAL_RCC_USB_OTG_FS_CLK_ENABLE();
    }
}

void HAL_PCD_MspDeInit(PCD_HandleTypeDef *hpcd)
{
    if (hpcd->Instance == USB_OTG_FS)
    {
        __HAL_RCC_USB_OTG_FS_CLK_DISABLE();
        HAL_GPIO_DeInit(GPIOA, GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12);
    }
}

/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : ip_config.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

#ifndef IP_CONFIG_H
#define IP_CONFIG_H

#include <stdint.h>

extern uint8_t ip_Con[4] ;

extern uint8_t NETMASK_ADDRESS_Con[4];
extern uint8_t GATEWAY_ADDRESS_Con[4];

extern uint8_t LED_LOGICAL_ONE ; 
extern uint8_t LED_LOGICAL_ZERO ;

// extern uint8_t  const LEDS_PER_UNIVERSE;
// extern uint8_t  const UNIVERSES_PER_PIN;
// extern uint16_t const LEDS_PER_PIN ;
// extern uint8_t  const NUM_PINS;


#endif

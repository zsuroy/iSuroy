/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : relay.h
  * @author         : @Suroy (https://Suroy.cn)
  * @brief          : Header for led.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) Suroy.cn
  * All rights reserved.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __RELAY_H
#define __RELAY_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"
#include "main.h"

/* Private defines -----------------------------------------------------------*/

/* Exported functions prototypes ---------------------------------------------*/

void RELAY_Toggle(void);
void RELAY_Ctr(uint8_t state);

#ifdef __cplusplus
}
#endif

#endif /* __RELAY_H */

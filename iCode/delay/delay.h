/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : delay.h
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
#ifndef __DELAY_H
#define __DELAY_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"
#include "main.h"

/* Private defines -----------------------------------------------------------*/

/* Exported functions prototypes ---------------------------------------------*/

void Delay_Us(uint32_t us);


#ifdef __cplusplus
}
#endif

#endif /* __DELAY_H */

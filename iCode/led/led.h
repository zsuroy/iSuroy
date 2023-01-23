/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : led.h
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
#ifndef __LED_H
#define __LED_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"
#include "main.h"

/* Private defines -----------------------------------------------------------*/

/* Exported functions prototypes ---------------------------------------------*/

void LED_Toggle(void);
void LED_Ctr(uint8_t state);
void LED_Check(uint8_t mode);

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

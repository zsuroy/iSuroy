/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : beep.h
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
#ifndef __BEEP_H
#define __BEEP_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"
#include "main.h"

/* Private defines -----------------------------------------------------------*/

/* Exported functions prototypes ---------------------------------------------*/

void Beep_Mid1(void);
void Beep_Mid2(void);

#ifdef __cplusplus
}
#endif

#endif /* __BEEP_H */

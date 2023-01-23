/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : key.h
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
#ifndef __KEY_H
#define __KEY_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"
#include "main.h"

/* Private defines -----------------------------------------------------------*/

/* Exported functions prototypes ---------------------------------------------*/

uint8_t KEY_Read(void);
uint8_t KEY_State(void);


#ifdef __cplusplus
}
#endif

#endif /* __KEY_H */

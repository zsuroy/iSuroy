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
#ifndef __ECO_H
#define __ECO_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"
#include "main.h"
/* Private defines -----------------------------------------------------------*/

/* Exported functions prototypes ---------------------------------------------*/

void ECO_Config(uint8_t mode);
void ECO_STANDBY_Check(void);

#ifdef __cplusplus
}
#endif

#endif /* __ECO_H */

/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    crc.c
  * @brief   This file provides code for the configuration
  *          of the CRC instances.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
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
#include "crc.h"

/* USER CODE BEGIN 0 */

#include <stdio.h>
static const uint32_t CRCBUF[4] = {0x02, 0x20, 0x02, 0x03}; // CRC校验数据数组
static uint32_t MCUID[3] = {}; // CRC校验数据数组

/* USER CODE END 0 */

CRC_HandleTypeDef hcrc;

/* CRC init function */
void MX_CRC_Init(void)
{

  /* USER CODE BEGIN CRC_Init 0 */

  /* USER CODE END CRC_Init 0 */

  /* USER CODE BEGIN CRC_Init 1 */

  /* USER CODE END CRC_Init 1 */
  hcrc.Instance = CRC;
  if (HAL_CRC_Init(&hcrc) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN CRC_Init 2 */

  /* USER CODE END CRC_Init 2 */

}

void HAL_CRC_MspInit(CRC_HandleTypeDef* crcHandle)
{

  if(crcHandle->Instance==CRC)
  {
  /* USER CODE BEGIN CRC_MspInit 0 */

  /* USER CODE END CRC_MspInit 0 */
    /* CRC clock enable */
    __HAL_RCC_CRC_CLK_ENABLE();
  /* USER CODE BEGIN CRC_MspInit 1 */

  /* USER CODE END CRC_MspInit 1 */
  }
}

void HAL_CRC_MspDeInit(CRC_HandleTypeDef* crcHandle)
{

  if(crcHandle->Instance==CRC)
  {
  /* USER CODE BEGIN CRC_MspDeInit 0 */

  /* USER CODE END CRC_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_CRC_CLK_DISABLE();
  /* USER CODE BEGIN CRC_MspDeInit 1 */

  /* USER CODE END CRC_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */

/**
 * @brief CRC 校验计算
 * 
 * @param clear 是否清零上次计算结果
 * @return uint32_t 
 */
uint32_t CRC_Cacluate(uint8_t clear)
{
  if(clear)return HAL_CRC_Calculate(&hcrc, (uint32_t *)CRCBUF, 4); //载入CRC并返回计算结果（每次清零BUF）, 4个字节
  else return HAL_CRC_Accumulate(&hcrc, (uint32_t *)CRCBUF, 4); // 计算初始值不会清零：适用于不连续的累加式校验
}

/**
 * @brief CRC 调试程序
 * 
 */
void CRC_Debug(void)
{
  printf("CRC Value: %08X \r\n", (unsigned int)CRC_Cacluate(1));
}

/**
 * @brief 读芯片ID
 * @note 移植需要根据具体芯片手册更改地址
 * @param show 是否显示 
 * @return uint32_t 
 */
void ID_Get(uint8_t show)
{
  MCUID[0] = *(__IO uint32_t *)(0x1FFFF7E8); //读出3个32位芯片ID（高字节）
  MCUID[1] = *(__IO uint32_t *)(0x1FFFF7EC);
  MCUID[2] = *(__IO uint32_t *)(0x1FFFF7F0); //低字节
  if(show)printf("MCU ID: %08X %08X %08X\r\n",(unsigned int)MCUID[0], (unsigned int)MCUID[1],(unsigned int) MCUID[2]); // 串口显示
  return;
}


/* USER CODE END 1 */

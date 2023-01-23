/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    rtc.c
  * @brief   This file provides code for the configuration
  *          of the RTC instances.
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
#include "rtc.h"

/* USER CODE BEGIN 0 */

// 定义存放RTC结构体
RTC_DateTypeDef RtcDate; // 保存日期
RTC_TimeTypeDef RtcTime; // 保存时间

/* USER CODE END 0 */

RTC_HandleTypeDef hrtc;

/* RTC init function */
void MX_RTC_Init(void)
{

  /* USER CODE BEGIN RTC_Init 0 */

  /* USER CODE END RTC_Init 0 */

  RTC_TimeTypeDef sTime = {0};
  RTC_DateTypeDef DateToUpdate = {0};

  /* USER CODE BEGIN RTC_Init 1 */
  
  // 确保掉电走时保留
  __HAL_RCC_PWR_CLK_ENABLE(); // 使能电源时钟PWR-开启BKP寄存器时钟
  HAL_PWR_EnableBkUpAccess(); // 取消备份区域写保护

  /* USER CODE END RTC_Init 1 */

  /** Initialize RTC Only
  */
  hrtc.Instance = RTC;
  hrtc.Init.AsynchPrediv = RTC_AUTO_1_SECOND;
  hrtc.Init.OutPut = RTC_OUTPUTSOURCE_ALARM;
  if (HAL_RTC_Init(&hrtc) != HAL_OK)
  {
    Error_Handler();
  }

  /* USER CODE BEGIN Check_RTC_BKUP */

  if( HAL_RTCEx_BKUPRead(&hrtc, RTC_BKP_DR1) != 0x9527 ) // 判断是否首次上电
  {
    HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR1, 0x9527); // 首次上电则标记数值(写入上电标志)
  
  
  /* USER CODE END Check_RTC_BKUP */

  /** Initialize RTC and set the Time and Date
  */
  sTime.Hours = 0x0;
  sTime.Minutes = 0x0;
  sTime.Seconds = 0x0;

  if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BCD) != HAL_OK)
  {
    Error_Handler();
  }
  DateToUpdate.WeekDay = RTC_WEEKDAY_MONDAY;
  DateToUpdate.Month = RTC_MONTH_JANUARY;
  DateToUpdate.Date = 0x1;
  DateToUpdate.Year = 0x0;

  if (HAL_RTC_SetDate(&hrtc, &DateToUpdate, RTC_FORMAT_BCD) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN RTC_Init 2 */
  
  } //  if( HAL_RTCEx_BKUPRead(&hrtc, RTC_BKP_DR1) != 0x9527 ) // 判断是否首次上电
  
  /* USER CODE END RTC_Init 2 */

}

void HAL_RTC_MspInit(RTC_HandleTypeDef* rtcHandle)
{

  if(rtcHandle->Instance==RTC)
  {
  /* USER CODE BEGIN RTC_MspInit 0 */

  /* USER CODE END RTC_MspInit 0 */
    HAL_PWR_EnableBkUpAccess();
    /* Enable BKP CLK enable for backup registers */
    __HAL_RCC_BKP_CLK_ENABLE();
    /* RTC clock enable */
    __HAL_RCC_RTC_ENABLE();
  /* USER CODE BEGIN RTC_MspInit 1 */

  /* USER CODE END RTC_MspInit 1 */
  }
}

void HAL_RTC_MspDeInit(RTC_HandleTypeDef* rtcHandle)
{

  if(rtcHandle->Instance==RTC)
  {
  /* USER CODE BEGIN RTC_MspDeInit 0 */

  /* USER CODE END RTC_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_RTC_DISABLE();
  /* USER CODE BEGIN RTC_MspDeInit 1 */

  /* USER CODE END RTC_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */

/**
 * @brief 读取日期时间
 * 以 2 位十进制数显示月日时分秒
 */
void RTC_Read(void)
{
  HAL_RTC_GetTime(&hrtc, &RtcTime, RTC_FORMAT_BIN); // 读出时间值到变量
  HAL_RTC_GetDate(&hrtc, &RtcDate, RTC_FORMAT_BIN); // 读出日期到变量 【一定要先读时间再读日期】
    // printf("DateTime: %04d-%02d-%02d %02d:%02d:%02d \r\n", 2000+RtcDate.Year, RtcDate.Month, RtcDate.Date, RtcTime.Hours, RtcTime.Minutes, RtcTime.Seconds); // 显示日期时间
}

/* USER CODE END 1 */

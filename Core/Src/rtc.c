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
#include <stdio.h>
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

/**
 * @brief RTC时钟控制
 * 
 * @param str 指令字符(串)
 * @param opt 指令选项: 0读取; 非0 根据指令字符判断
 * 
 * @note
 * 用串口时记得每次清空串口接收标志:  USART1_RX_STA=0; // 串口接收标志清0
 * 标记清空串口接收缓存: USART1_RX_BUF[0] = '\0';
 */
void RTC_Command(uint8_t str[], uint8_t opt)
{
  if(opt == 0)
  { // 读日期时间及提示
    RTC_Read();
    printf(" SUROY - RTC TEST\r\n");
    printf("DateTime: %04d-%02d-%02d %02d:%02d:%02d \r\n", 2000+RtcDate.Year, RtcDate.Month, RtcDate.Date, RtcTime.Hours, RtcTime.Minutes, RtcTime.Seconds); // 显示日期时间
    printf(" Input Enter to update time, 'C' Initial Clock\r\n");
    printf(" DateTime Format: 20230101120000\r\n");
  }
  else { 
    int i=0;
    while(str[i] != '\0')i++; // 字符串长度

    if(i==1) // 字符串长度为1
    {
      if( (str[0] == 'c') || (str[0] == 'C') )
      {  // 时钟初始化
        MX_RTC_Init(); // 键盘输入c或C，初始化时钟
        printf(" <RTC_Clock init>");
      } else printf(" 指令错误！ \r\n");

    } else if (i == 14)
    { //长度为14即日期时间格式； 年份前两位固定为20
      RtcDate.Year = (str[2]-0x30)*10 + (str[3]-0x30); // 减去0x30(空格)得到0-9十进制数
      RtcDate.Month = (str[4]-0x30)*10 + (str[5]-0x30); // 减去0x30(空格)得到0-9十进制数
      RtcDate.Date = (str[6]-0x30)*10 + (str[7]-0x30); // 减去0x30(空格)得到0-9十进制数
      RtcTime.Hours = (str[8]-0x30)*10 + (str[9]-0x30); // 减去0x30(空格)得到0-9十进制数
      RtcTime.Minutes = (str[10]-0x30)*10 + (str[11]-0x30); // 减去0x30(空格)得到0-9十进制数
      RtcTime.Seconds = (str[12]-0x30)*10 + (str[13]-0x30); // 减去0x30(空格)得到0-9十进制数
      // 时间写入
      if(HAL_RTC_SetTime(&hrtc, &RtcTime, RTC_FORMAT_BIN) != HAL_OK )printf(" 时间写入失败！\r\n");
      else if(HAL_RTC_SetDate(&hrtc, &RtcDate, RTC_FORMAT_BIN) != HAL_OK )printf(" 日期写入失败！\r\n");
      else printf("写入成功！\r\n");
    }
    str[0] = '\0'; //清空字符串长度标志, 用于对传入的 USART1_RX_BUF 进行标记清空
  }

}

/* USER CODE END 1 */

/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    usart.c
  * @brief   This file provides code for the configuration
  *          of the USART instances.
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
#include "usart.h"

/* USER CODE BEGIN 0 */
#include <stdio.h>

uint16_t USART1_RX_STA=0; // 接收状态标记： bit 15 接收完成标注; bit 14 接收到0x0d; bit 13-0 接收到有效字节数目
uint8_t USART1_NewData; // 当前串口1中断接收到1个字节的缓存
uint8_t USART1_RX_BUF[USART1_RX_LEN]; // 接收缓冲区
/* USER CODE END 0 */

UART_HandleTypeDef huart1;

/* USART1 init function */

void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

void HAL_UART_MspInit(UART_HandleTypeDef* uartHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(uartHandle->Instance==USART1)
  {
  /* USER CODE BEGIN USART1_MspInit 0 */

  /* USER CODE END USART1_MspInit 0 */
    /* USART1 clock enable */
    __HAL_RCC_USART1_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**USART1 GPIO Configuration
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_10;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* USART1 interrupt Init */
    HAL_NVIC_SetPriority(USART1_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(USART1_IRQn);
  /* USER CODE BEGIN USART1_MspInit 1 */

  /* USER CODE END USART1_MspInit 1 */
  }
}

void HAL_UART_MspDeInit(UART_HandleTypeDef* uartHandle)
{

  if(uartHandle->Instance==USART1)
  {
  /* USER CODE BEGIN USART1_MspDeInit 0 */

  /* USER CODE END USART1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_USART1_CLK_DISABLE();

    /**USART1 GPIO Configuration
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_9|GPIO_PIN_10);

    /* USART1 interrupt Deinit */
    HAL_NVIC_DisableIRQ(USART1_IRQn);
  /* USER CODE BEGIN USART1_MspDeInit 1 */

  /* USER CODE END USART1_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */

/**
 * @brief 轮询模式读取数据
 * 
 * @param huart 
 */
void UART_Direct(UART_HandleTypeDef *huart)
{
  uint8_t ch;
  // printf("UART Retarget:\r\n");
  if(HAL_UART_Receive(huart,(uint8_t *)&ch, 1, 100) == HAL_OK)
  { //huart1是串口的句柄, 最多等待100ms
    HAL_UART_Transmit(huart, (uint8_t *)&ch, 1, 100); // 发回
  }
}

/**
 * @brief 阻塞式串口收发调试
 * 直接复制本函数内代码放到 main.c 中 while(1) 单独运行
 * 同时 CubeMX 配置串口中断关闭
 * 否则可能会产生异常
 * 
 */
void UART_DEBUG(void)
{
    /*
    printf("Retarget:\r\n");
    printf("-----Suroy.cn\r\n");
    scanf("%c", &RecData);
    if(HAL_UART_Receive(&huart1, &RecData, 1, HAL_MAX_DELAY) == HAL_OK)
      printf("Rec: %c\n", RecData);
    if( RecData == 'y')
    {
      printf("OK: %c\n", RecData);
      Beep_Mid2();
    } else printf("NO!\n");
    */
   return;
}


/**
 * @brief 串口中断接收回调函数
 * 
 * @param huart 
 * @note 
 * 需要在 main.c 中初始化:  
 * HAL_UART_Receive_IT(&huart1, (uint8_t *)&USART1_NewData, 1); //开启串口1接收中断：初始上电默认关闭状态
 * 
 * 
 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  if(huart == &huart1) // 中断来源: 串口1
  {
    // printf("%c", USART1_NewData); // 将接收到的数据发送回电脑

    // bit15\14都为1则表接收到回车键
    if((USART1_RX_STA&0x8000) == 0 )
    { // 接收未完成： USART_RX_STA的bit15表示接收完成标志(接收协议)。USART_RX_STA&0x8000即与运算保留bit15位比较，若为1则接受完成
      
      if(USART1_RX_STA&0x4000)
      { // 收到 bit14表示接收到0X0D(接收协议)  其中，0x0D（asc码是13指的是“回车”\r，是把光标置于本行行首; 0x0A（asc码是10） 指的是“换行”\n，是把光标置于下一行的同一列

        if(USART1_NewData != 0x0a)USART1_RX_STA=0; // 接收错误，重新开始；未收到 \n， 要求最后收到\r\n才是回车确定
        else USART1_RX_STA |= 0x8000; // 给bit15置串口接收完成标志

      } else {
        // 尚未收到 0x0d（\r）
        if(USART1_NewData == 0x0d)USART1_RX_STA |= 0x4000; // 处于没有接收到\r态，应该判断接收值是否满足切换bit14的条件
        else {
          USART1_RX_BUF[USART1_RX_STA&0x3FFF]=USART1_NewData; // 将接收到的数据放入熟组: &0x3FFFF是确定接收到的数据长度的
          USART1_RX_STA++; // 数据长度递增
          if(USART1_RX_STA > (USART1_RX_LEN-1))USART1_RX_STA=0; //接收数据长度超出定义缓冲区，重新开始接收
        }
      }

    }
    HAL_UART_Receive_IT(huart, (uint8_t *)&USART1_NewData, 1); // 再开启接收中断
  }
}


/**
 * @brief 串口中断接收调试程序
 * 收到命令执行对应功能，e.g. (beep)
 */
void USART_IT_DEBUG(void)
{
    if(USART1_RX_STA&0xC000)
    { // 串口1判断接收中断标志位: 有数据传入且含回车(\r\n)作为结束符
      printf("[RX_BUF]: %s\r\n", USART1_RX_BUF); // 输出接收缓冲区内容
      if(USART1_RX_BUF[0]=='1')
      {
        // Beep_Mid2();

      }
      if(USART1_RX_BUF[0]=='0')
      {
        // Beep_Mid1();
      }
      USART1_RX_STA=0; // 串口接收标志清0
      USART1_RX_BUF[0]='\0'; // 置缓冲区为空
    }
}

/* USER CODE END 1 */

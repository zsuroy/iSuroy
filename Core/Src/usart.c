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
#include <string.h>
#include "stm32f1xx_hal.h"
#include "tim.h"

uint16_t USART1_RX_STA=0; // 接收状态标记： bit 15 接收完成标注; bit 14 接收到0x0d; bit 13-0 接收到有效字节数目
uint8_t USART1_NewData; // 当前串口1中断接收到1个字节的缓存
uint8_t USART1_RX_BUF[USART1_RX_LEN]; // 接收缓冲区

uint16_t USART3_RX_STA=0; // 接收状态标记： bit 15 接收完成标注; bit 14 接收到0x0d; bit 13-0 接收到有效字节数目
uint8_t USART3_NewData; // 当前串口1中断接收到1个字节的缓存
uint8_t USART3_RX_BUF[USART3_RX_LEN]; // 接收缓冲区



/* USER CODE END 0 */

UART_HandleTypeDef huart1;
UART_HandleTypeDef huart3;

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
/* USART3 init function */

void MX_USART3_UART_Init(void)
{

  /* USER CODE BEGIN USART3_Init 0 */

  /* USER CODE END USART3_Init 0 */

  /* USER CODE BEGIN USART3_Init 1 */

  /* USER CODE END USART3_Init 1 */
  huart3.Instance = USART3;
  huart3.Init.BaudRate = 115200;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART3_Init 2 */

  /* USER CODE END USART3_Init 2 */

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
    HAL_NVIC_SetPriority(USART1_IRQn, 1, 0);
    HAL_NVIC_EnableIRQ(USART1_IRQn);
  /* USER CODE BEGIN USART1_MspInit 1 */

  /* USER CODE END USART1_MspInit 1 */
  }
  else if(uartHandle->Instance==USART3)
  {
  /* USER CODE BEGIN USART3_MspInit 0 */

  /* USER CODE END USART3_MspInit 0 */
    /* USART3 clock enable */
    __HAL_RCC_USART3_CLK_ENABLE();

    __HAL_RCC_GPIOB_CLK_ENABLE();
    /**USART3 GPIO Configuration
    PB10     ------> USART3_TX
    PB11     ------> USART3_RX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_10;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_11;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* USART3 interrupt Init */
    HAL_NVIC_SetPriority(USART3_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(USART3_IRQn);
  /* USER CODE BEGIN USART3_MspInit 1 */

  /* USER CODE END USART3_MspInit 1 */
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
  else if(uartHandle->Instance==USART3)
  {
  /* USER CODE BEGIN USART3_MspDeInit 0 */

  /* USER CODE END USART3_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_USART3_CLK_DISABLE();

    /**USART3 GPIO Configuration
    PB10     ------> USART3_TX
    PB11     ------> USART3_RX
    */
    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_10|GPIO_PIN_11);

    /* USART3 interrupt Deinit */
    HAL_NVIC_DisableIRQ(USART3_IRQn);
  /* USER CODE BEGIN USART3_MspDeInit 1 */

  /* USER CODE END USART3_MspDeInit 1 */
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
 * HAL_UART_Receive_IT(&huart3, (uint8_t *)&USART3_NewData, 1); //开启串口3接收中断：初始上电默认关闭状态
 * USART3_RX_STA=0; // 清零接收标志位
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


  if(huart ==&huart3)//判断中断来源（串口3：WIFI模块）//接收完的一批数据,还没有被处理,则不再接收其他数据
	{
		if(USART3_RX_STA<USART3_RX_LEN)//还可以接收数据
		{
			__HAL_TIM_SET_COUNTER(&htim2,0); //计数器清空
			if(USART3_RX_STA==0) //使能定时器2的中断
			{
				__HAL_TIM_ENABLE(&htim2); //使能定时器2
			}
			USART3_RX_BUF[USART3_RX_STA++] = USART3_NewData;//最新接收数据放入数组
		}else
		{
			USART3_RX_STA|=0x8000;//强制标记接收完成
		}

		HAL_UART_Receive_IT(&huart3,(uint8_t *)&USART3_NewData,1); //再开启串口3接收中断
	}

  /* V1.0.11 版本，V1.0.12弃用: 由于无法接收不定长数据

  if(huart == &huart3) // 中断来源（串口3:WIFI模块）
  {
    //[原始数据内容]字符: +IPD,1:A    Hex: 0D 0A 2B 49 50 44 2C 31 3A 41 (1为数量，A为内容)
    //【接收原理】当接收到0x0A(即回车\r)时触发接下来的数据采集程序
    //首先清空USART3_RX_BUF[]寄存器，然后将USART3_RX_STA的16位中bit15位置1(01000000 00000000)
    //此时开始采集接下来收到的数据，当收到前6个数据位"+IPD,1:“时，且第7个数据部位0时，表示接收完数据。
    //然后将接收的第7位的一个字节数据内容放入到USART3_RX_STA寄存器低8位，并将16位最高位置1(10000000 00000000)
    //【调用说明】在主函数中用if语句判断(USART3_RX_STA&0x8000)为真时表示成功收到数据
    //然后读取USART3_RX_STA寄存器低14位内容(USART3_RX_STA&0x3FFFF)，即是数据的内容（1个字节）
    //主函数处理完数据后要将USART3_RX_STA清0，才能开启下一次数据接收
    if(USART3_RX_STA&0x4000)
    { // 收到 bit14表示接收到0X0D(接收协议)  其中，0x0D（asc码是13指的是“回车”\r，是把光标置于本行行首; 0x0A（asc码是10） 指的是“换行”\n，是把光标置于下一行的同一列
      
      USART3_RX_BUF[USART3_RX_STA&0x3FFF]=USART3_NewData; // 将接收到的数据放入熟组: &0x3FFFF是确定接收到的数据长度的
      USART3_RX_STA++; // 数据长度递增

      // Packet demo: \n\r+IPD,1:*    *为传入字符
      //              0x0D 0x0A 0x2B xxxxxxx
      if(USART3_RX_BUF[0]=='+' && // 判断返回字符前几位是不是 "+IPD,1:"
        USART3_RX_BUF[1]=='I' &&
        USART3_RX_BUF[2]=='P' &&
        USART3_RX_BUF[3]=='D' &&
        USART3_RX_BUF[4]==',' &&
        USART3_RX_BUF[5]=='1' && //限定只接收1个数量的数据（根据具体项目修改）
        USART3_RX_BUF[6]==':' &&
        USART3_RX_BUF[7]!=0){//同时判断第一个数据是否为0，为0则表未收到数据
          USART3_RX_STA = USART3_RX_BUF[7]+0x8000; // 将数据内容写入寄存器，16位最高位1表接收完成；等同于 ｜ 运算
        }
    }

    // 尚未收到 0x0A（\r回车）
    if(USART3_NewData == 0x0A && !(USART3_RX_STA&0x8000))
    {
      USART3_RX_STA = 0x4000; //将开始采集标志位 置1（bit15）
      for(uint8_t a=0; a<USART3_RX_LEN; a++)USART3_RX_BUF[a]=0; //清空数据寄存器
    }

    HAL_UART_Receive_IT(huart, (uint8_t *)&USART3_NewData, 1); // 再开启串口3接收中断

  }

  */


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

/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
#include "main.h"
#include "adc.h"
#include "crc.h"
#include "dma.h"
#include "rtc.h"
#include "tim.h"
#include "usart.h"
#include "usb_device.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "../../iCode/led/led.h"
#include "../../iCode/key/key.h"
#include "../../iCode/beep/beep.h"
#include "../../iCode/delay/delay.h"
#include "../../iCode/relay/relay.h"
#include "../../iCode/DHT11/dht11.h"
#include "../../USB_DEVICE/App/usbd_cdc_if.h"
#include "../../iCode/eco/eco.h"
#include "../../iCode/WIFI/wifi.h"
#include "stdio.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_NVIC_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_USART1_UART_Init();
  MX_ADC1_Init();
  MX_RTC_Init();
  MX_USB_DEVICE_Init();
  MX_CRC_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  MX_USART3_UART_Init();

  /* Initialize interrupts */
  MX_NVIC_Init();
  /* USER CODE BEGIN 2 */
  HAL_UART_Receive_IT(&huart1, (uint8_t *)&USART1_NewData, 1); //开启串口1接收中断：初始上电默认关闭状态
  HAL_UART_Receive_IT(&huart3, (uint8_t *)&USART3_NewData, 1); //开启WIFI-串口3接收中断：初始上电默认关闭状态
  HAL_ADCEx_Calibration_Start(&hadc1); //ADC采样校准
  // HAL_ADC_Start_DMA(&hadc1, (uint32_t *)&ADC1_Value, 1); // 单通道：启动DMA，采集到数据存入的变量地址，长度为1字节
  HAL_ADC_Start_DMA(&hadc1, (uint32_t *)ADC1_MulChannel_Value, 2); // 双通道：启动DMA，采集到数据存入的变量地址，长度为2字节
  
  HAL_TIM_Base_Start_IT(&htim2); //开启TIM定时器中断: 必须要先开启才能进入中断处理回调函数

  // ECO_STANDBY_Check(); // 节能：待机模式唤醒检测
  
  HAL_Delay(500); // 延时等待MCU稳定
  DHT11_Init();
  HAL_Delay(1500); // 延时等待DHT11稳定


  WIFI_Start(); // WIFI连接到TCP服务器

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  // uint8_t RecData; // 接收输入字符
  printf("Retarget:\r\n");
  printf("-----Suroy.cn\r\n");

  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
    // LED_Toggle();
    // LED_Ctr(1);
    // USART_IT_DEBUG();
    
    /*
    if(USART1_RX_STA&0xC000)
    { // 串口1判断接收中断标志位
      printf("[RX_BUF]: %s\r\n", USART1_RX_BUF); // 输出接收缓冲区内容
      if(USART1_RX_BUF[0]=='1')
      {
        Beep_Mid2();
      }
      if(USART1_RX_BUF[0]=='0')
      {
        Beep_Mid1();
      }
      USART1_RX_STA=0; // 串口接收标志清0
      USART1_RX_BUF[0]='\0'; // 置缓冲区为空
    }
   */

    LED_Check(0);

    // PWM控制LED
    PWM_Value = TIM_PWM_Set();

    // 普通RTC
    // RTC_Command(0, 0); // 时间及提示读取
    // RTC_Command(USART1_RX_BUF, 1); // 时间更改配置
    
    /**优化走时
     * 
    RTC_Opt_Command(0, 0); // 时间及提示读取
    RTC_Opt_Command(USART1_RX_BUF, 1); // 时间更改配置
    */

    // 走时公共代码
    // printf("%s", USART1_RX_BUF);
    // printf("DateTime: %04d-%02d-%02d %02d:%02d:%02d \r\n", 2000+RtcDate.Year, RtcDate.Month, RtcDate.Date, RtcTime.Hours, RtcTime.Minutes, RtcTime.Seconds); // 显示日期时间
    // USART1_RX_STA=0; // 时钟测试：串口接收标志清0
    // USART1_RX_BUF[0]='\0'; // 置清空标志
    // HAL_Delay(200); // 延时200ms以防看不见测试效果

    
    // 节能模式
    // ECO_Config(1); // 睡眠
    // ECO_Config(2); // 停机
    // ECO_Config(3); // 待机

    //WIFI控制调试
    WIFI_CONTROL_Debug();

    // CRC 校验测试
    CRC_Debug();

    //读芯片ID
    ID_Get(1);

    // DHT11
    if(DHT11_ReadData(DHT11_BUF) == HAL_OK)
    {
      printf("DHT11: %02d%c - %02d 'C\r\n", DHT11_BUF[0], '%' , DHT11_BUF[1]); //02:23:24:024 -> DHT11: 81% - 12 'C
      HAL_Delay(1500);
    }

    // USB模拟串口
    USB_Debug();


    // DMA
    // printf("ADC1_DMA_0=%04d  ADC1_DMA_1=%04d\r\n", ADC1_MulChannel_Value[0], ADC1_MulChannel_Value[1]); //读取ADC1数值，4位十进制

    HAL_Delay(500);

  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE|RCC_OSCILLATORTYPE_LSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_RTC|RCC_PERIPHCLK_ADC
                              |RCC_PERIPHCLK_USB;
  PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV8;
  PeriphClkInit.UsbClockSelection = RCC_USBCLKSOURCE_PLL_DIV1_5;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief NVIC Configuration.
  * @retval None
  */
static void MX_NVIC_Init(void)
{
  /* EXTI0_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(EXTI0_IRQn, 1, 0);
  HAL_NVIC_EnableIRQ(EXTI0_IRQn);
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

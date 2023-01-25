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

// 优化走时完善版本的日期时间定义
uint16_t ryear; //4位年
uint8_t rmon, rday, rhour, rmin, rsec, rweek; //2位月日时分秒
//修正月份数据表                                                                     
uint8_t const table_week[12]={0,3,3,6,1,4,6,2,5,0,3,5}; //月修正数据表  
const uint8_t mon_table[12]={31,28,31,30,31,30,31,31,30,31,30,31};//平年的月份日期表


/* USER CODE END 0 */

RTC_HandleTypeDef hrtc;

/* RTC init function */
void MX_RTC_Init(void) // 优化走时版V1.0.4: 初始化
{
  // 确保掉电走时保留
  __HAL_RCC_PWR_CLK_ENABLE(); // 使能电源时钟PWR-开启BKP寄存器时钟
  HAL_PWR_EnableBkUpAccess(); // 取消备份区域写保护

  /** Initialize RTC Only
  */
  hrtc.Instance = RTC;
  hrtc.Init.AsynchPrediv = RTC_AUTO_1_SECOND;
  hrtc.Init.OutPut = RTC_OUTPUTSOURCE_ALARM;
  if (HAL_RTC_Init(&hrtc) != HAL_OK)
  {
    Error_Handler();
  }

  if( HAL_RTCEx_BKUPRead(&hrtc, RTC_BKP_DR1) != 0x9527 ) // 判断是否首次上电
  {
    HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR1, 0x9527); // 首次上电则标记数值(写入上电标志)
    RTC_Set(2023,01,01,10,02,03); // 优化后走时
  } //  if( HAL_RTCEx_BKUPRead(&hrtc, RTC_BKP_DR1) != 0x9527 ) // 判断是否首次上电
  
}

/* RTC init function */
void MX_RTC_Init_Orign(void) // V1.0.3 RTC, 使用时替换原版
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



/**
 * @brief RTC时钟控制
 * @note 优化走时完善版本
 * @param str 指令字符(串)
 * @param opt 指令选项: 0读取; 非0 根据指令字符判断
 * 
 * @note
 * 用串口时记得每次清空串口接收标志:  USART1_RX_STA=0; // 串口接收标志清0
 * 标记清空串口接收缓存: USART1_RX_BUF[0] = '\0';
 */
void RTC_Opt_Command(uint8_t str[], uint8_t opt)
{
  if(opt == 0)
  { // 读日期时间及提示
    RTC_Get();
    printf(" SUROY - RTC OPT TEST\r\n");
    printf("DateTime: %04d-%02d-%02d %02d:%02d:%02d \r\n", 2000+ryear, rmon, rday, rhour, rmin, rsec); // 显示日期时间
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
      ryear = (str[2]-0x30)*10 + (str[3]-0x30); // 减去0x30(空格)得到0-9十进制数
      rmon = (str[4]-0x30)*10 + (str[5]-0x30); // 减去0x30(空格)得到0-9十进制数
      rday = (str[6]-0x30)*10 + (str[7]-0x30); // 减去0x30(空格)得到0-9十进制数
      rhour = (str[8]-0x30)*10 + (str[9]-0x30); // 减去0x30(空格)得到0-9十进制数
      rmin = (str[10]-0x30)*10 + (str[11]-0x30); // 减去0x30(空格)得到0-9十进制数
      rsec = (str[12]-0x30)*10 + (str[13]-0x30); // 减去0x30(空格)得到0-9十进制数

      // 优化走时写入
      if(RTC_Set(ryear, rmon, rday, rhour, rmin, rsec) != HAL_OK) printf(" RTC_Clock_Opt write failure!\r\n");
      else printf("RTC_Clock_Opt write success!\r\n");
    }
    str[0] = '\0'; //清空字符串长度标志, 用于对传入的 USART1_RX_BUF 进行标记清空
  }

}


/**
 * @brief 读出时间
 * @note 优化走时完善版本
 * @return uint8_t 
 */
uint8_t RTC_Get(void){ //读出当前时间值//返回值：0，成功；其他：错误代码.
  static uint16_t daycnt=0;
  uint32_t timecount=0;
  uint32_t temp=0;
  uint16_t temp1=0;

  //【寄存器操作】因为HAL库的不完善，无法直接调用RTC_WriteTimeCounteri函数。此处改用寄存器直接操作
  timecount=RTC->CNTH;//得到计数器中的值（秒钟数）
  timecount<<=16;
  timecount+=RTC->CNTL;
  //【寄存器操作】结束

  temp=timecount/86400;
  //得到天数（秒钟数对应的）
  if(daycnt!=temp){//超过-天了
    daycnt=temp;
    temp1=1970;//从1970年开始
    while(temp>=365){
      if(Is_Leap_Year(temp1)){//是闰年
        if(temp>=366)temp-=366;//闰年的秒钟数
        else {temp1++; break;}
      }
      else temp-=365; //平年
      temp1++;
    }
    ryear=temp1;//得到年份
    temp1=0;
    while(temp>=28){ //超过了一个月
      if( (Is_Leap_Year(ryear)&temp1) == 1 ){//当年是不是闰年/2月份
        if(temp>=29)temp-=29;//闰年的秒钟数
        else break;
      }else{
        if(temp>=mon_table[temp1])temp-=mon_table[temp1];//平年
        else break;
      }
        temp1++; 
    }
    rmon=temp1+1;//得到月份
    rday=temp+1;  //得到日期
  }
	temp=timecount%86400;     //得到秒钟数      
	rhour=temp/3600;     //小时
	rmin=(temp%3600)/60; //分钟     
	rsec=(temp%3600)%60; //秒钟
	rweek=RTC_Get_Week(ryear, rmon, rday);//获取星期  
	return 0;
}


/**
 * @brief 写入时间
 * @note 优化走时完善版本
 * @param syear 
 * @param smon 
 * @param sday 
 * @param hour 
 * @param min 
 * @param sec 
 * @return uint8_t 
 */
uint8_t RTC_Set(uint16_t syear,uint8_t smon,uint8_t sday,uint8_t hour,uint8_t min,uint8_t sec){ //写入当前时间（1970~2099年有效），
	uint16_t t;
	uint32_t seccount=0;
	if(syear<2000||syear>2099)return 1;//syear范围1970-2099，此处设置范围为2000-2099       
	for(t=1970;t<syear;t++){ //把所有年份的秒钟相加
		if(Is_Leap_Year(t))seccount+=31622400;//闰年的秒钟数
		else seccount+=31536000;                    //平年的秒钟数
	}
	smon-=1;
	for(t=0;t<smon;t++){         //把前面月份的秒钟数相加
		seccount+=(uint32_t)mon_table[t]*86400;//月份秒钟数相加
		if(Is_Leap_Year(syear)&&t==1)seccount+=86400;//闰年2月份增加一天的秒钟数        
	}
	seccount+=(uint32_t)(sday-1)*86400;//把前面日期的秒钟数相加
	seccount+=(uint32_t)hour*3600;//小时秒钟数
	seccount+=(uint32_t)min*60;      //分钟秒钟数
	seccount+=sec;//最后的秒钟加上去

  //【寄存器操作】因为HAL库的不完善，无法直接调用RTC_ReadTimeCounteri函数。此处改用寄存器直接操作。
  RTC->CRL|=1<<4;//允许配置
  RTC->CNTL=seccount&0xffff;
  RTC->CNTH=seccount>>16;
  RTC->CRL &= ~(1<<4);//配置更新
  while (!( (RTC->CRL) & (1<<5) )); // 等待RTC寄存器操作完成
  //【寄存器操作】结束
  return 0; //返回值:0,成功;其他:错误代码.    
}

/**
 * @brief 判断是否是闰年函数
 * 优化走时完善版本
 * @note
 * 月份   1  2  3  4  5  6  7  8  9  10 11 12
 * 闰年   31 29 31 30 31 30 31 31 30 31 30 31
 * 非闰年 31 28 31 30 31 30 31 31 30 31 30 31
 * @param year 年份
 * @return uint8_t 该年份是不是闰年.1,是.0,不是
 */
uint8_t Is_Leap_Year(uint16_t year){                    
	if(year%4==0){ //必须能被4整除
		if(year%100==0){		
			if(year%400==0)return 1;//如果以00结尾,还要能被400整除          
			else return 0;  
		}else return 1;  
	}else return 0;
}

/**
 * @brief 按年月日计算星期(只允许1901-2099年)
 * @note 优化走时版本
 * @param year 
 * @param month 
 * @param day 
 * @return uint8_t 
 */
uint8_t RTC_Get_Week(uint16_t year, uint16_t month, uint16_t day)
{ //按年月日计算星期(只允许1901-2099年)//已由RTC_Get调用    
	uint16_t temp2;
	uint8_t yearH, yearL;
	yearH=year/100;     
	yearL=year%100;
	// 如果为21世纪,年份数加100 
	if (yearH>19)yearL+=100;
	// 所过闰年数只算1900年之后的 
	temp2=yearL+yearL/4;
	temp2=temp2%7;
	temp2=temp2+day+table_week[month-1];
	if (yearL%4==0 && month<3)temp2--;

	return temp2%7; //返回星期值
}


/* USER CODE END 1 */

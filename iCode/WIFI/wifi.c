/*********************************************************************************************
 * Author: Suroy
 * Blog: https://suroy.cn
 * 
 * Usage:
 * 
 * @note 
 * 需要在 main.c 中初始化:  
 * HAL_UART_Receive_IT(&huart3, (uint8_t *)&USART3_NewData, 1); //开启串口3接收中断：初始上电默认关闭状态
 * USART3_RX_STA=0; // 清零接收标志位 
 * 
 * @mark:
 * 所有串口回调函数 HAL_UART_RxCpltCallback 均位于 usart.c 文件中

*********************************************************************************************/

#include "wifi.h"
#include "../../Core/Inc/usart.h"
#include <string.h> // 字符串处理的库
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include "../../iCode/beep/beep.h"


/**
 * @brief WIFI 串口专用printf
 * @note 当同时开启2个以上串口时，printf函数只能用于其中之一，其他串口要自创独立的printf函数
 * @param fmt 
 * @param ... 
 */
void WIFI_printf (char *fmt, ...){ 
	char buffer[USART3_RX_LEN+1];  // 存放转换后的数据长度 [长度]
	uint16_t i = 0;	
	va_list arg_ptr;
	va_start(arg_ptr, fmt);  
	vsnprintf(buffer, USART3_RX_LEN+1, fmt, arg_ptr); // 数据转换:向一个字符串缓冲区打印格式化字符串，且可以限定打印的格式化字符串的最大长度。
    i=strlen(buffer); // 获取数据长度
    if(strlen(buffer) > USART3_RX_LEN)i=USART3_RX_LEN; // 若长度大于最大值，则长度等于最大值（多出部分舍去）
    HAL_UART_Transmit(&huart3, (uint8_t *)buffer, i, 0xffff); // 串口发送函数(串口号，内容，数量，溢出时间)
	va_end(arg_ptr);
}



/**
 * @brief WIFI 串口数据发送
 * @note 当同时开启2个以上串口时，printf函数只能用于其中之一，其他串口要自创独立的printf函数
 * @param fmt 
 * @param ... 
 */
void WIFI_TCP_Send (char *fmt, ...){ 
	char buffer[USART3_RX_LEN+1];  // 存放转换后的数据长度 [长度]
	uint16_t i = 0;	
	va_list arg_ptr;
	va_start(arg_ptr, fmt);  
	vsnprintf(buffer, USART3_RX_LEN+1, fmt, arg_ptr); // 数据转换:向一个字符串缓冲区打印格式化字符串，且可以限定打印的格式化字符串的最大长度。
    i=strlen(buffer); // 获取数据长度
    if(strlen(buffer) > USART3_RX_LEN)i=USART3_RX_LEN; // 若长度大于最大值，则长度等于最大值（多出部分舍去）
    WIFI_printf("AT+CIPSEND=%d\r\n",i); // 先发送AT指令和数据长度
    HAL_Delay(100); // 等待WIFI模块返回 ">", 未做是否返回">"判断，不具备高稳定性
    HAL_UART_Transmit(&huart3, (uint8_t *)buffer, i, 0xffff); // 串口发送函数(串口号，内容，数量，溢出时间)
	va_end(arg_ptr);
}


/**
 * @brief WIFI控制调试
 * @note
 * TCP接收数据的处理
 * 只有串口3收到开头为"+IPD,"时才会被识别为接收到的数据，内容转存到了(USART3_RX_STA&0x3FFF)中，且只有一个字节
 * 需要根据具体项目调整 usart.c 中的串口中断回调函数
 */
void WIFI_CONTROL_Debug(void)
{
    if(USART3_RX_STA & 0x8000)
    {
        switch (USART3_RX_STA&0x3FFF) //判断接收数据的内容
        {
            case 'A':
                printf("WIFI Rec: A\r\n");
                Beep_Mid1();
                break;
            case 'B':
                Beep_Mid2(); // 蜂鸣器响
                break;
            
            default:
                printf("WIFI_DEBUG: %c\r\n", (int)(USART3_RX_STA&0x3FFF));
                break;
        }
    }
}


/**
 * @brief WIFI连接TCP
 * @note
 * 初始化时可以调用或者通过按键调
 */
void WIFI_Start(void)
{
    WIFI_printf("AT+CIPSTART=\"TCP\",\"e.iot.suroy.cn\",8888\r\n"); // 连接TCP服务器
    
    // 自建TCP服务器: 需要同步更改发送接收命令
    /*
    WIFI_printf("AT+CIPMUX=1");
    WIFI_printf("AT+CIPSERVERMAXCONN=5"); //最大连接数
    WIFI_printf("AT+CIPSERVER=1,8888");
    WIFI_printf("AT+CIPSTO=600"); //超时10分钟
    WIFI_printf("AT+CIFSR"); //查ip
    WIFI_printf("AT+CIPSEND=0,1"); //发送到设备0，长度1
    printf("WIFI: %s\r\n", USART3_RX_BUF); //输出WIFI配置到串口1
    */
    HAL_Delay(100); //等待
}


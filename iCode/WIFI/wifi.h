#ifndef __WIFI_H
#define __WIFI_H

#include "stm32f1xx_hal.h"
#include "main.h"



extern UART_HandleTypeDef huart3; // 继承USART3点HAL库结构体

/*usart.c 中已经继承
#define USART3_RX_LEN 200 // USART3 最大接收长度
extern uint16_t USART3_RX_STA; // 接收状态标记： bit 15 接收完成标注; bit 14 接收到0x0d; bit 13-0 接收到有效字节数目
extern uint8_t USART3_NewData; // 当前串口3中断接收到1个字节的缓存
extern uint8_t USART3_RX_BUF[USART3_RX_LEN]; // 接收缓冲区
*/

void WIFI_printf (char *fmt, ...);
void WIFI_TCP_Send (char *fmt, ...);
void WIFI_Start(void);
void WIFI_CONTROL_Debug(void);




#endif

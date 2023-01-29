#ifndef __WIFI_H
#define __WIFI_H

#include "stm32f1xx_hal.h"
#include "main.h"



extern UART_HandleTypeDef huart3; // �̳�USART3��HAL��ṹ��

/*usart.c ���Ѿ��̳�
#define USART3_RX_LEN 200 // USART3 �����ճ���
extern uint16_t USART3_RX_STA; // ����״̬��ǣ� bit 15 ������ɱ�ע; bit 14 ���յ�0x0d; bit 13-0 ���յ���Ч�ֽ���Ŀ
extern uint8_t USART3_NewData; // ��ǰ����3�жϽ��յ�1���ֽڵĻ���
extern uint8_t USART3_RX_BUF[USART3_RX_LEN]; // ���ջ�����
*/

void WIFI_printf (char *fmt, ...);
void WIFI_TCP_Send (char *fmt, ...);
void WIFI_Start(void);
void WIFI_CONTROL_Debug(void);




#endif

/*********************************************************************************************
 * Author: Suroy
 * Blog: https://suroy.cn
 * 
 * Usage:
 * 
 * @note 
 * ��Ҫ�� main.c �г�ʼ��:  
 * HAL_UART_Receive_IT(&huart3, (uint8_t *)&USART3_NewData, 1); //��������3�����жϣ���ʼ�ϵ�Ĭ�Ϲر�״̬
 * USART3_RX_STA=0; // ������ձ�־λ 
 * 
 * @mark:
 * ���д��ڻص����� HAL_UART_RxCpltCallback ��λ�� usart.c �ļ���

*********************************************************************************************/

#include "wifi.h"
#include "../../Core/Inc/usart.h"
#include <string.h> // �ַ�������Ŀ�
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include "../../iCode/beep/beep.h"


/**
 * @brief WIFI ����ר��printf
 * @note ��ͬʱ����2�����ϴ���ʱ��printf����ֻ����������֮һ����������Ҫ�Դ�������printf����
 * @param fmt 
 * @param ... 
 */
void WIFI_printf (char *fmt, ...){ 
	char buffer[USART3_RX_LEN+1];  // ���ת��������ݳ��� [����]
	uint16_t i = 0;	
	va_list arg_ptr;
	va_start(arg_ptr, fmt);  
	vsnprintf(buffer, USART3_RX_LEN+1, fmt, arg_ptr); // ����ת��:��һ���ַ�����������ӡ��ʽ���ַ������ҿ����޶���ӡ�ĸ�ʽ���ַ�������󳤶ȡ�
    i=strlen(buffer); // ��ȡ���ݳ���
    if(strlen(buffer) > USART3_RX_LEN)i=USART3_RX_LEN; // �����ȴ������ֵ���򳤶ȵ������ֵ�����������ȥ��
    HAL_UART_Transmit(&huart3, (uint8_t *)buffer, i, 0xffff); // ���ڷ��ͺ���(���ںţ����ݣ����������ʱ��)
	va_end(arg_ptr);
}



/**
 * @brief WIFI �������ݷ���
 * @note ��ͬʱ����2�����ϴ���ʱ��printf����ֻ����������֮һ����������Ҫ�Դ�������printf����
 * @param fmt 
 * @param ... 
 */
void WIFI_TCP_Send (char *fmt, ...){ 
	char buffer[USART3_RX_LEN+1];  // ���ת��������ݳ��� [����]
	uint16_t i = 0;	
	va_list arg_ptr;
	va_start(arg_ptr, fmt);  
	vsnprintf(buffer, USART3_RX_LEN+1, fmt, arg_ptr); // ����ת��:��һ���ַ�����������ӡ��ʽ���ַ������ҿ����޶���ӡ�ĸ�ʽ���ַ�������󳤶ȡ�
    i=strlen(buffer); // ��ȡ���ݳ���
    if(strlen(buffer) > USART3_RX_LEN)i=USART3_RX_LEN; // �����ȴ������ֵ���򳤶ȵ������ֵ�����������ȥ��
    WIFI_printf("AT+CIPSEND=%d\r\n",i); // �ȷ���ATָ������ݳ���
    HAL_Delay(100); // �ȴ�WIFIģ�鷵�� ">", δ���Ƿ񷵻�">"�жϣ����߱����ȶ���
    HAL_UART_Transmit(&huart3, (uint8_t *)buffer, i, 0xffff); // ���ڷ��ͺ���(���ںţ����ݣ����������ʱ��)
	va_end(arg_ptr);
}


/**
 * @brief WIFI���Ƶ���
 * @note
 * TCP�������ݵĴ���
 * ֻ�д���3�յ���ͷΪ"+IPD,"ʱ�Żᱻʶ��Ϊ���յ������ݣ�����ת�浽��(USART3_RX_STA&0x3FFF)�У���ֻ��һ���ֽ�
 * ��Ҫ���ݾ�����Ŀ���� usart.c �еĴ����жϻص�����
 */
void WIFI_CONTROL_Debug(void)
{
    if(USART3_RX_STA & 0x8000)
    {
        switch (USART3_RX_STA&0x3FFF) //�жϽ������ݵ�����
        {
            case 'A':
                printf("WIFI Rec: A\r\n");
                Beep_Mid1();
                break;
            case 'B':
                Beep_Mid2(); // ��������
                break;
            
            default:
                printf("WIFI_DEBUG: %c\r\n", (int)(USART3_RX_STA&0x3FFF));
                break;
        }
    }
}


/**
 * @brief WIFI����TCP
 * @note
 * ��ʼ��ʱ���Ե��û���ͨ��������
 */
void WIFI_Start(void)
{
    WIFI_printf("AT+CIPSTART=\"TCP\",\"e.iot.suroy.cn\",8888\r\n"); // ����TCP������
    
    // �Խ�TCP������: ��Ҫͬ�����ķ��ͽ�������
    /*
    WIFI_printf("AT+CIPMUX=1");
    WIFI_printf("AT+CIPSERVERMAXCONN=5"); //���������
    WIFI_printf("AT+CIPSERVER=1,8888");
    WIFI_printf("AT+CIPSTO=600"); //��ʱ10����
    WIFI_printf("AT+CIFSR"); //��ip
    WIFI_printf("AT+CIPSEND=0,1"); //���͵��豸0������1
    printf("WIFI: %s\r\n", USART3_RX_BUF); //���WIFI���õ�����1
    */
    HAL_Delay(100); //�ȴ�
}


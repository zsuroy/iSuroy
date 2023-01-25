#ifndef __DHT11_H
#define __DHT11_H	 
#include "stm32f1xx_hal.h"
#include "../delay/delay.h"

// main.c 中已经定义
// #define DHT11_Pin GPIO_PIN_6
// #define DHT11_GPIO_Port GPIOA //定义IO接口

void DHT11_IO_OUT (void);
void DHT11_IO_IN (void);
void DHT11_RST (void);
uint8_t Dht11_Check(void); 	   
uint8_t Dht11_ReadBit(void); 
uint8_t Dht11_ReadByte(void); 
uint8_t DHT11_Init (void);
uint8_t DHT11_ReadData(uint8_t *h);

extern uint8_t DHT11_BUF[2];

#endif

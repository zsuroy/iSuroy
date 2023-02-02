#ifndef __OLED_H
#define __OLED_H	 

#include "stm32f1xx_hal.h"
#include "main.h"

#define OLED_ADD	0x78  // OLED的I2C地址（禁止修改）
#define COM				0x00  // OLED 指令（禁止修改）
#define DAT 			0x40  // OLED 数据（禁止修改）

void OLED_Init(void);//初始化
void OLED_DISPLAY_ON (void);//OLED屏开显示
void OLED_DISPLAY_OFF (void);//OLED屏关显示
void OLED_DISPLAY_LIT (uint8_t x);//OLED屏亮度设置（0~255）
void OLED_DISPLAY_CLEAR(void);//清屏操作
void OLED_DISPLAY_8x16(uint8_t x,uint8_t y,uint16_t w);//显示8x16的单个字符 
void OLED_DISPLAY_8x16_BUFFER(uint8_t row,uint8_t *str);//显示8x16的字符串

void OLED_DISPLAY_16x16(uint8_t x,uint8_t y,uint16_t w); //汉字显示
void OLED_DISPLAY_PIC1(void);//图片显示
void OLED_Debug(void);
void OLED_Printf (uint8_t row, char *fmt, ...);


		 				    
#endif

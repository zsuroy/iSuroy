/*********************************************************************************************
 * Author: Suroy
 * Blog: https://suroy.cn
 * 
 * Usage:
 * I2C 硬件驱动 OLED
 *   PB6     ------> I2C1_SCL
 *   PB7     ------> I2C1_SDA
*********************************************************************************************/

#include "oled.h"
#include "ASCII.h" //引入字码
#include "i2c.h"
#include <string.h> // 字符串处理的库
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>

void OLED_Init (void){//OLED屏开显示初始化
	OLED_DISPLAY_OFF(); //OLED关显示
	OLED_DISPLAY_CLEAR(); //清空屏幕内容
	OLED_DISPLAY_ON(); //OLED屏初始值设置并开显示
}
void OLED_DISPLAY_ON (void){//OLED屏初始值设置并开显示
	uint8_t buf[28]={
	0xae,//0xae:关显示，0xaf:开显示
    0x00,0x10,//开始地址（双字节）       
	0xd5,0x80,//显示时钟频率？
	0xa8,0x3f,//复用率？
	0xd3,0x00,//显示偏移？
	0XB0,//写入页位置（0xB0~7）
	0x40,//显示开始线
	0x8d,0x14,//VCC电源
	0xa1,//设置段重新映射？
	0xc8,//COM输出方式？
	0xda,0x12,//COM输出方式？
	0x81,0xff,//对比度，指令：0x81，数据：0~255（255最高）
	0xd9,0xf1,//充电周期？
	0xdb,0x30,//VCC电压输出
	0x20,0x00,//水平寻址设置
	0xa4,//0xa4:正常显示，0xa5:整体点亮
	0xa6,//0xa6:正常显示，0xa7:反色显示
	0xaf//0xae:关显示，0xaf:开显示
	}; 
	HAL_I2C_Mem_Write(&hi2c1, OLED_ADD, COM, I2C_MEMADD_SIZE_8BIT, buf, 28, 1000);
}
void OLED_DISPLAY_OFF (void){//OLED屏关显示
	uint8_t buf[3]={
		0xae,//0xae:关显示，0xaf:开显示
		0x8d,0x10,//VCC电源
	}; 
	HAL_I2C_Mem_Write(&hi2c1, OLED_ADD, COM, I2C_MEMADD_SIZE_8BIT, buf, 3, 1000);
}

void OLED_DISPLAY_LIT (uint8_t x){//OLED屏亮度设置（0~255）
	uint8_t buf=0x81;
	HAL_I2C_Mem_Write(&hi2c1, OLED_ADD, COM, I2C_MEMADD_SIZE_8BIT, &buf, 1, 1000);
	HAL_I2C_Mem_Write(&hi2c1, OLED_ADD, COM, I2C_MEMADD_SIZE_8BIT, &x, 1, 1000); //亮度值
}

void OLED_DISPLAY_CLEAR(void){//清屏操作
	uint8_t j,t;
	uint8_t buf[2]={0x10, 0x00};
	for(t=0xB0;t<0xB8;t++){	//设置起始页地址为0xB0
		HAL_I2C_Mem_Write(&hi2c1, OLED_ADD, COM, I2C_MEMADD_SIZE_8BIT, &t, 1, 1000); //页地址（从0xB0到0xB7）
		HAL_I2C_Mem_Write(&hi2c1, OLED_ADD, COM, I2C_MEMADD_SIZE_8BIT, &buf[0], 1, 1000); //起始列地址的高4位
		HAL_I2C_Mem_Write(&hi2c1, OLED_ADD, COM, I2C_MEMADD_SIZE_8BIT, &buf[1], 1, 1000); //起始列地址的低4位
		for(j=0;j<132;j++){	//整页内容填充
			HAL_I2C_Mem_Write(&hi2c1, OLED_ADD, DAT, I2C_MEMADD_SIZE_8BIT, &buf[1], 1, 1000); //起始列地址的低4位
 		}
	}
}

//显示英文与数字8*16的ASCII码
//取模大小为8*16，取模方式为“从左到右从上到下”“纵向8点下高位”
void OLED_DISPLAY_8x16(uint8_t x, //显示汉字的页坐标（从0到7）（此处不可修改）
						uint8_t y, //显示汉字的列坐标（从0到128）
						uint16_t w){ //要显示汉字的编号
	uint8_t j,t,c=0;
	uint8_t buf[4];
	y=y+2; //因OLED屏的内置驱动芯片是从0x02列作为屏上最左一列，所以要加上偏移量
	for(t=0;t<2;t++){
		buf[0]=0xb0+x; buf[1]=y/16+0x10; buf[2]=y%16;
		HAL_I2C_Mem_Write(&hi2c1, OLED_ADD, COM, I2C_MEMADD_SIZE_8BIT, &buf[0], 1, 1000); //页地址（从0xB0到0xB7）
		HAL_I2C_Mem_Write(&hi2c1, OLED_ADD, COM, I2C_MEMADD_SIZE_8BIT, &buf[1], 1, 1000); //起始列地址的高4位
		HAL_I2C_Mem_Write(&hi2c1, OLED_ADD, COM, I2C_MEMADD_SIZE_8BIT, &buf[2], 1, 1000); //起始列地址的低4位

		for(j=0;j<8;j++){ //整页内容填充
			buf[3]=ASCII_8x16[(w*16)+c-512];
			HAL_I2C_Mem_Write(&hi2c1, OLED_ADD, DAT, I2C_MEMADD_SIZE_8BIT, &buf[3], 1, 1000); //为了和ASII表对应要减512
			c++;
		}
		x++; //页地址加1
	}
}
//向LCM发送一个字符串,长度64字符之内。
//应用：OLED_DISPLAY_8_16_BUFFER(0," Suroy's Studio"); 
void OLED_DISPLAY_8x16_BUFFER(uint8_t row,uint8_t *str){
	uint8_t r=0;
	while(*str != '\0'){
		OLED_DISPLAY_8x16(row,r*8,*str++);
		r++;
    }	
}


//----- 用于汉字显示的程序 ------//
//显示汉字16*16
//取模大小为16*16，取模方式为“从左到右从上到下”“纵向8点下高位”
void OLED_DISPLAY_16x16(uint8_t x, //显示汉字的页坐标（从0xB0到0xB7）
			uint8_t y, //显示汉字的列坐标（从0到63）
			uint16_t w){ //要显示汉字的编号
	uint8_t j,t,c=0;
	uint8_t buf[4];
	for(t=0;t<2;t++){
		buf[0]=0xb0+x; buf[1]=y/16+0x10; buf[2]=y%16; 
		HAL_I2C_Mem_Write(&hi2c1, OLED_ADD, COM, I2C_MEMADD_SIZE_8BIT, &buf[0], 1, 1000); //页地址（从0xB0到0xB7）
		HAL_I2C_Mem_Write(&hi2c1, OLED_ADD, COM, I2C_MEMADD_SIZE_8BIT, &buf[1], 1, 1000); //起始列地址的高4位
		HAL_I2C_Mem_Write(&hi2c1, OLED_ADD, COM, I2C_MEMADD_SIZE_8BIT, &buf[2], 1, 1000); //起始列地址的低4位
		for(j=0;j<16;j++){ //整页内容填充
			buf[3]=GB_16[(w*32)+c];
			HAL_I2C_Mem_Write(&hi2c1, OLED_ADD, DAT, I2C_MEMADD_SIZE_8BIT, &buf[3], 1, 1000); 
			c++;}x++; //页地址加1
	}
	buf[3]=0xAF; // 重新给缓存少一个变量 
	HAL_I2C_Mem_Write(&hi2c1, OLED_ADD, COM, I2C_MEMADD_SIZE_8BIT, &buf[3], 1, 1000); //开显示
}

void OLED_DISPLAY_PIC1(void){ //显示全屏图片
	uint8_t m,i;
	uint8_t buf[4];
	for(m=0;m<8;m++){
		buf[0]=0xb0+m; buf[1]=0x10; buf[2]=0x02; 
		HAL_I2C_Mem_Write(&hi2c1, OLED_ADD, COM, I2C_MEMADD_SIZE_8BIT, &buf[0], 1, 1000); //页地址（从0xB0到0xB7）
		HAL_I2C_Mem_Write(&hi2c1, OLED_ADD, COM, I2C_MEMADD_SIZE_8BIT, &buf[1], 1, 1000); //起始列地址的高4位
		HAL_I2C_Mem_Write(&hi2c1, OLED_ADD, COM, I2C_MEMADD_SIZE_8BIT, &buf[2], 1, 1000); //起始列地址的低4位
		for(i=0;i<128;i++){//送入128次图片显示内容
			buf[3]=PIC1[i+m*128];
			HAL_I2C_Mem_Write(&hi2c1, OLED_ADD, DAT, I2C_MEMADD_SIZE_8BIT, &buf[3], 1, 1000); 
		}
	}
}



/**
 * @brief OLED_Printf
 * @note
 * 输出字符(字母/数字)到屏幕; 不支持汉字
 * @param row 行号 0+
 * @param fmt 
 * @param ... 
 * 
 * @usage
 * OLED_Printf(0, "%s", "Welcome!")
 */
void OLED_Printf (uint8_t row, char *fmt, ...){ 
	char buffer[16];  // 存放转换后的数据长度 [长度]
	// uint16_t i = 0;	
	va_list arg_ptr;
	va_start(arg_ptr, fmt);  
	vsnprintf(buffer, 16, fmt, arg_ptr); // 数据转换:向一个字符串缓冲区打印格式化字符串，且可以限定打印的格式化字符串的最大长度。
    // if(strlen(buffer) > 15)i=15; // 若长度大于最大值，则长度等于最大值（多出部分舍去）
	OLED_DISPLAY_8x16_BUFFER(row, (uint8_t *)buffer); // 发送
	va_end(arg_ptr);
}



/**
 * @brief OLED 显示调试
 * 
 */
void OLED_Debug(void)
{
	// I2C_Configuration();//I2C初始化
	OLED_Init(); //OLED初始化
	OLED_DISPLAY_LIT(100);//亮度设置

	OLED_DISPLAY_PIC1();//显示全屏图片
	HAL_Delay(1000); //延时
	OLED_DISPLAY_CLEAR();
	OLED_DISPLAY_8x16_BUFFER(0,(uint8_t *)"   SuroyDev. "); //显示字符串
	// OLED_Printf(0,"%s","   SuroyDev. "); //显示字符串
	OLED_Printf(5,"%s","http://suroy.cn"); //显示字符串

	OLED_DISPLAY_16x16(2,2*16,0);//汉字显示	 及时行乐
	OLED_DISPLAY_16x16(2,3*16,1);
	OLED_DISPLAY_16x16(2,4*16,2);
	OLED_DISPLAY_16x16(2,5*16,3);
	
}



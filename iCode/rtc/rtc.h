#ifndef INC_RTC_H_
#define INC_RTC_H_

#include "stm32f1xx_hal.h" //HAL库文件声明
#include "main.h" //IO定义与初始化函数在main.c文件中，必须引用

/*
//时间读写与设置说明//
1，在mani.c文件中主循环之前放入RTC_Init();可使能RTC时钟。
	RTC_Init函数自带判断首次上电功能
2，使用RTC_Get();读出时间。读出的数据存放在：
年 ryear	（16位）
月 rmon	（以下都是8位）
日 rday
时 rhour
分 rmin
秒 rsec
周 rweek
3，使用RTC_Set(4位年,2位月,2位日,2位时,2位分,2位秒); 写入时间。
例如：RTC_Set(2022,8,6,21,34,0);

其他函数都是帮助如上3个函数的，不需要调用。
注意要使用RTC_Get和RTC_Set的返回值，为0时表示读写正确。
*/

extern RTC_HandleTypeDef hrtc;

//声明rtc.c文件中定义的全局变量（注意：这里不能给变量赋值）
extern uint16_t ryear;
extern uint8_t rmon,rday,rhour,rmin,rsec,rweek;

void RTC_Init(void); //用户自建的带有上电BPK判断的RTC初始化【在主循环前调用】
uint8_t Is_Leap_Year(uint16_t year);//判断是否是闰年函数
uint8_t RTC_Get(void);//读出当前时间值【主函数中需要读RTC时调用】
uint8_t RTC_Set(uint16_t syear,uint8_t smon,uint8_t sday,uint8_t hour,uint8_t min,uint8_t sec);//写入当前时间【主函数中需要写入RTC时调用】
uint8_t RTC_Get_Week(uint16_t year,uint8_t month,uint8_t day);//按年月日计算星期

#endif



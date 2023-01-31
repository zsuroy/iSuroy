/*********************************************************************************************
 * Author: Suroy
 * Blog: https://suroy.cn
 * 
 * Usage: 
 * 中文字符需要串口编码为GB2312方可显示
 * 
*********************************************************************************************/

#include "esp8266.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

uint8_t esp8266_send_cmd(char *cmd, char *ack, uint16_t waittime) //ESP8266发送指令（底层函数）
{
	uint8_t res = 0;
	USART3_RX_STA = 0;
	memset(USART3_RX_BUF,0,USART3_RX_LEN); //将串口3的缓存空间清0
	WIFI_printf("%s\r\n", cmd); //调用WIFI模块专用的发送函数
	if(waittime) //需要等待应答
	{
		while(--waittime) //等待倒计时
		{
			HAL_Delay(10);//HAL库延时函数
			if(USART3_RX_STA&0x8000) //接收到期待的应答结果
			{
				if(esp8266_check_cmd((uint8_t *)ack))
				{
					printf("<ESP> Replay: %s\r\n",(uint8_t *)ack);//反馈应答信息
					break; //得到有效数据
				}
				USART3_RX_STA=0; //串口3标志位清0
			} 
		}
		if(waittime==0)res=1;
	}
	return res;
}

uint8_t* esp8266_check_cmd(uint8_t *str) //ESP8266检查指令（底层函数）
{
	char *strx=0;
	if(USART3_RX_STA&0X8000) //接收到一次数据了
	{
		USART3_RX_BUF[USART3_RX_STA&0X7FFF] = 0; //添加结束符
		printf("%s\r\n",(char*)USART3_RX_BUF);
		strx=strstr((const char*)USART3_RX_BUF,(const char*)str);
	} 
	return (uint8_t*)strx;
}

uint8_t esp8266_quit_trans(void) //ESP8266退出判断（底层函数）
{
	while((USART3->SR&0X40)==0); //等待发送空
	USART3->DR='+';      
	HAL_Delay(15); //大于串口组帧时间(10ms)
	while((USART3->SR&0X40)==0); //等待发送空
	USART3->DR='+';      
	HAL_Delay(15); //大于串口组帧时间(10ms)
	while((USART3->SR&0X40)==0); //等待发送空
	USART3->DR='+';      
	HAL_Delay(500); //等待500ms
	return esp8266_send_cmd("AT","OK",20); //退出透传判断
}

uint8_t esp8266_Connect_IOTServer(void) //ESP8266连接到物联网平台服务器
{
//状态检测
	printf("准备配置模块\r\n");
	HAL_Delay(100);
	esp8266_send_cmd("AT","OK",50);
	printf("准备退出透传模式\n");
	if(esp8266_quit_trans())
	{
		printf("退出透传模式失败，准备重启\r\n");
		return 6;
	}else printf("退出透传模式成功\r\n");
	
	printf("准备关闭回显\r\n");
	if(esp8266_send_cmd("ATE0","OK",50))
	{
		printf("关闭回显失败准备重启\r\n");
		return 1;
	}else printf("关闭回显成功\r\n");
	
	printf("查询模块是否在线\r\n");
	if(esp8266_send_cmd("AT","OK",50))
	{
		printf("模块不在线准备重启\r\n");
		return 1;
	}else printf("设置查询在线成功\r\n");
	
//设置
	printf("准备设置STA模式\r\n");
	if(esp8266_send_cmd("AT+CWMODE=1","OK",50))
	{
		printf("设置STA模式失败准备重启\r\n");
		return 1;
	}else printf("设置STA模式成功\r\n");
	
	printf("准备重启\r\n");
	if(esp8266_send_cmd("AT+RST","OK",50))
	{
		printf("重启失败，准备重启\r\n");
		return 2;
	}else printf("重启成功，等待三秒\r\n");
	
	HAL_Delay(3000);//延时等待WIFI模块就绪

	printf("准备取消自动连接\r\n");
	if(esp8266_send_cmd("AT+CWAUTOCONN=0","OK",50))
	{
		printf("取消自动连接失败，准备重启\r\n");
		return 3;
	}else printf("取消自动连接成功\r\n");
	
	printf("准备链接路由器\r\n");
	if(esp8266_Connect_AP())
	{
		printf("连接路由器失败，等待重启\r\n");
		return 4;
	}else printf("连接路由器成功\r\n");
	
	HAL_Delay(4000);//延时等待WIFI模块就绪

	printf("准备开启DHCP\r\n");
	if(esp8266_send_cmd("AT+CWDHCP=1,1","OK",100))
	{
		printf("开启DHCP失败，准备重启\r\n");
		return 7;
	}else printf("设置DHCP成功\r\n");

//TCP连接
	printf("设置为关闭多路连接\r\n");
	if(esp8266_send_cmd("AT+CIPMUX=0","OK",100))
	{
		printf("关闭多路连接失败，准备重启\r\n");
		return 7;
	}else printf("设置关闭多路连接成功\r\n");

	printf("准备链接服务器\r\n");
	if(esp8266_Connect_Server())
	{
		printf("连接服务器失败，等待重启\r\n");
		return 8;
	}else printf("连接服务器成功\r\n");
	
	printf("准备退出透传模式\n");
	if(esp8266_quit_trans())
	{
		printf("退出透传模式失败，准备重启\r\n");
		return 6;
	}else printf("退出透传模式成功\r\n");
	
	printf("设置为透传模式\r\n");
	if(esp8266_send_cmd("AT+CIPMODE=1","OK",50))
	{
		printf("设置透传失败，准备重启\r\n");
		return 6;
	}else printf("设置透传成功\r\n");

//发送数据
	printf("设置开启透传模式\r\n");
	if(esp8266_send_cmd("AT+CIPSEND","OK",1000))
	{
		printf("开启透传失败，准备重启\r\n");
		return 9;
	}else printf("开启透传成功\r\n");
	
	USART3_RX_STA = 0;
	memset(USART3_RX_BUF,0,USART3_RX_LEN);
	
	return 0; //一切顺利返回0
}

uint8_t esp8266_Connect_AP() //ESP8266连接AP设备（无线路由器）
{
	uint8_t i=10;
	char *p = (char*)malloc(50);//分配存储空间的指针

	sprintf((char*)p,"AT+CWJAP=\"%s\",\"%s\"",SSID,PASS);//发送连接AT指令
	while(esp8266_send_cmd(p,"WIFI GOT IP",1000) && i)//循环判断等待连接AP的结果
	{
		printf("链接AP失败，尝试重新连接\r\n"); //连接失败的反馈信息
		i--;
	}
	free(p);//释放分配的空间和指针
	if(i) return 0;//执行成功返回0
	else return 1;//执行失败返回1
}

uint8_t esp8266_Connect_Server() //ESP8266连接到服务器
{
	uint8_t i=10;
	// char *p = (char*)malloc(50);//分配存储空间的指针
	char *p = (char*)malloc(70);//分配存储空间的指针
	sprintf((char*)p,"AT+CIPSTART=\"TCP\",\"%s\",\%s",IOT_DOMAIN_NAME,IOT_PORTNUM);
	while(esp8266_send_cmd(p,"CONNECT",1000) && i)
	{
		printf("链接服务器失败，尝试重新连接\r\n");
		i--;
	}
	free(p);//释放分配的空间和指针
	if(i)return 0;//执行成功返回0
	else return 1;//执行失败返回1
}


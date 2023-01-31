/*********************************************************************************************
 * Author: Suroy
 * Blog: https://suroy.cn
 * 
 * Usage: None
 * 
*********************************************************************************************/

#include "iot.h"
#include "string.h"
#include "stdlib.h"
#include <stdio.h>
#include <stdarg.h>
#include "../hmac/utils_hmac.h"
#include "../esp8266/esp8266.h"
#include "../../beep/beep.h"
#include "../../key/key.h"



uint16_t PING_Counter=0xFFF0; //MQTT心跳计数: 初始让其先溢出
uint16_t REBOOT_Counter=0; //WIFI模块异常重启计数
int t; //MQTT接收函数返回状态临时变量
int qos; //服务质量等级（0最多分发一次，1至少分发一次，2只分发一次）
int payloadinlen; //消息长度
unsigned char dup; //重发标志位（0首发，1早前报文的重发）
unsigned char retained; //保留位参数
unsigned short msgid; // 消息ID
unsigned char* payloadin; // 消息内容
MQTTString receiveTopic; // 订阅主题


uint16_t buflen=200;
unsigned char buf[200];

char ClientID[128];
uint8_t ClientID_len;

char Username[128];
uint8_t Username_len;

char Password[128];
uint8_t Password_len;

uint8_t IOT_connect()
{
	uint16_t a;
	uint32_t len;
	char temp[128];
	printf("Start to connect Server\r\n");
	MQTTPacket_connectData data = MQTTPacket_connectData_initializer;//配置部分可变头部的值
	buflen = sizeof(buf);
	memset(buf,0,buflen);
	memset(ClientID,0,128);//客户端ID的缓冲区全部清零
	sprintf(ClientID,"%s|securemode=3,signmethod=hmacsha1|",DEVICENAME);//构建客户端ID，并存入缓冲区
	memset(Username,0,128);//用户名的缓冲区全部清零
	sprintf(Username,"%s&%s",DEVICENAME,PRODUCTKEY);//构建用户名，并存入缓冲区

	Username_len = strlen(Username);

	memset(temp,0,128);//临时缓冲区全部清零
	sprintf(temp,"clientId%sdeviceName%sproductKey%s",DEVICENAME,DEVICENAME,PRODUCTKEY);//构建加密时的明文
	utils_hmac_sha1(temp,strlen(temp),Password,DEVICESECRE,DEVICESECRE_LEN);//以DeviceSecret为秘钥对temp中的明文，进行hmacsha1加密，结果就是密码，并保存到缓冲区中
	Password_len = strlen(Password);//计算用户名的长度

	printf("ClientId:%s\r\n",ClientID);
	printf("Username:%s\r\n",Username);
	printf("Password:%s\r\n",Password);

	//【重要参数设置】可修改版本号、保活时间
	data.MQTTVersion = MQTTVERSION; //MQTT协议版本号
	data.clientID.cstring = ClientID; //客户端标识，用于区分每个客户端xxx为自定义，后面为固定格式
	data.keepAliveInterval = KEEPALIVEINTERVAL; //保活计时器，定义了服务器收到客户端消息的最大时间间隔,单位是秒
	data.cleansession = 1; //该标志置1服务器必须丢弃之前保持的客户端的信息，将该连接视为“不存在”
	data.username.cstring = Username; //用户名 DeviceName&ProductKey
	data.password.cstring = Password; //密码，工具生成
	
	len = MQTTSerialize_connect(buf, buflen, &data);//构造连接的报文
	transport_sendPacketBuffer(0,buf, len);//发送连接请求

	unsigned char sessionPresent, connack_rc;
	a=0;
	while(MQTTPacket_read(buf, buflen, transport_getdata) != CONNACK || a>1000)//等待胳回复
	{
		HAL_Delay(10);//必要的延时等待
		a++;//超时计数加1
	}
	if(a>1000)NVIC_SystemReset();//当计数超时，则复位单片机

	while(MQTTDeserialize_connack(&sessionPresent, &connack_rc, buf, buflen) != 1 || connack_rc != 0);
	if(connack_rc != 0)
	{
		printf("Reply: %uc\r\n",connack_rc);
	}
	printf("Connected!\r\n");
	return 0;//执行成功返回0
}

void IOT_ping(void)//发送心跳包PING（保持与云服务器的连接）
{
	uint32_t len;
	len = MQTTSerialize_pingreq(buf, buflen); //计算数据长度
	transport_sendPacketBuffer(0, buf, len); //发送数据
	HAL_Delay(200);//必要的延时等待
	printf("Ping... ");
}

uint8_t IOT_subscribe(void)//subscribe主题订阅（订阅成功后才能接收订阅消息）
{
	uint32_t len;
	int req_qos = TOPIC_QOS;
	MQTTString topicString = MQTTString_initializer;//定义Topic结构体并初始化
	topicString.cstring = TOPIC_SUBSCRIBE;
	len = MQTTSerialize_subscribe(buf, buflen, 0, MSGID, 1, &topicString, &req_qos);//订阅发送数据编码
	transport_sendPacketBuffer(0, buf, len);
	HAL_Delay(100);//必要的延时等待
	if(MQTTPacket_read(buf, buflen, transport_getdata) == SUBACK) //等待订阅回复
	{
		unsigned short submsgid;
		int subcount;
		int granted_qos;
		MQTTDeserialize_suback(&submsgid, 1, &subcount, &granted_qos, buf, buflen);//回复的订阅确认数据解码
		if(granted_qos != 0) //qos不为0表示订阅成功
		{
			printf("Succeed GrantedQoS=%d\r\n", granted_qos);
			return 0; //订阅成功
		}
	}
	printf("Fail to Subscription\r\n");
	return 1; //订阅失败
}

uint8_t IOT_publish(char* payload)//publish主题发布（参数是发布信息内容，用双引号包含）
{
	uint32_t len;
	MQTTString topicString = MQTTString_initializer;//定义Topic结构体并初始化
	topicString.cstring = TOPIC_PUBLISH;
	int payloadlen = strlen(payload);//用函数计算发布信息内容的长度
	printf("Publish: %.*s\r\n", payloadlen, payload);
	//将要发送的信息payload通过MQTTSerialize_publish编码后用transport_sendPacketBuffer发送给云服务器
	len = MQTTSerialize_publish(buf, buflen, 0, 0, 0, 0, topicString,
								(unsigned char*)payload, payloadlen);//发布数据编码
	transport_sendPacketBuffer(0, buf, len); //发送编码好的最终数据
	HAL_Delay(100);//必要的延时等待
	return 1;
}


void IOT_publish_printf (char *fmt, ...)
{
	char buff[200];  //用于存放转换后的数据
	va_list arg_ptr;
	va_start(arg_ptr, fmt);
	vsnprintf(buff, 201, fmt, arg_ptr);//数据转换
	IOT_publish(buff);//将字符处理后publish主题发布
    va_end(arg_ptr);
}





/**
 * @brief 云平台处理主程序
 * @author Suroy
 * @note
 * 初始化
	HAL_UART_Receive_IT(&huart3,(uint8_t *)&USART3_NewData,1); //再开启串口3接收中断
	HAL_TIM_Base_Start_IT(&htim2);//开启定时器中断（必须开启才能进入中断处理回调函数）

	//  while(esp8266_Connect_AP());//连接AP无线路由器热点（热点参数在esp8266.h。WIFI模块已保存热点时可屏蔽）
	while(esp8266_Connect_IOTServer());//AT指令连接TCP连接云服务器（IP和端口参数在esp8266.h文件内修改设置）
	while(IOT_connect());//用MQTT协议+三元组信息连接阿里云物联网平台（三元组参数在iot.h文件内修改设置）
	printf("订阅云服务器\r\n");
	HAL_Delay(100);//等待
	IOT_subscribe();//主题订阅（订阅成功后才能接收订阅消息）
	PING_Counter=0xFFF0; //强制发送心跳包的计数溢出，立即重发心跳包
 *
 *
 */
void IOT_Work(void)
{
  while (1)
  {
	  //循环发送心跳包，以保持设备在线
	  HAL_Delay(10);//主循环的间隔延时（防止刷新过快）
	  PING_Counter++;//计算加1
	  if(PING_Counter>1000){ //每1000*10毫秒延时发送一次Ping心跳包（保保持与云服务器的连接）
			PING_Counter=0;//计算标志清0
			IOT_ping();//发送Ping心跳包
			if(MQTTPacket_read(buf, buflen, transport_getdata)==PINGRESP){//判断心跳包是不回复确认
				printf("Pong\r\n"); //回复0xD0，0x00时表示心跳成功的回复
				// 发布消息
				//IOT_publish_printf("W=%d,U=%02d,Temp=%02d,Hum=%02d,WARNING=%d",WORK_STA,USER_TEMP_STA,DHT11_BUF[1],DHT11_BUF[0],WARNING_STA);//publish主题发布（发送到云平台）

			}else {
				printf("<Ping> Error\r\n");//无回复表示失败
				Beep_Mid2();//蜂鸣器输出单音（提示心跳失败）
				PING_Counter=0xFFF0; //强制发送心跳包的计数溢出，立即重发心跳包
				REBOOT_Counter++;//重启计数加1
				if(REBOOT_Counter>20) //如果快速发送心跳包20次后无回复，则复位WIFI模块重新连接
				{
					while(esp8266_Connect_IOTServer());//AT指令连接TCP连接云服务器（IP和端口参数在esp8266.h文件内修改设置）
					while(IOT_connect());//用MQTT协议+三元组信息连接阿里云物联网平台（三元组参数在iot.h文件内修改设置）
					PING_Counter=0;REBOOT_Counter=0;//计算标志清0
				}
			}
			USART3_RX_STA = 0;//串口3接收标志位清0
	  }
	  
	  //接收云端的订阅消息
	  if(USART3_RX_STA&0x8000) //判断云服务器发布的消息
	  {
		  switch (USART3_RX_BUF[0]/16){//判断接收到的报文类型
			case PUBLISH:
				Beep_Mid1();//蜂鸣器输出单音
				t = MQTTDeserialize_publish( //对接收的MQTT原始数据进行解码（返回1表示成功，其他值表示错误）
					  &dup, //【得出】重发标志位（0首发，1早前报文的重发）
					  &qos, //【得出】服务质量等级（0最多分发一次，1至少分发一次，2只分发一次）
					  &retained, //【得出】保留位参数
					  &msgid, //【得出】消息ID
					  &receiveTopic, //【得出】订阅主题名
					  &payloadin, //【得出】消息内容
					  &payloadinlen, //【得出】消息长度
					  USART3_RX_BUF, USART3_RX_STA&0x7FFF); //【输入】原始数据缓存（数组+数量）
				if(t){//如果数据正确
				  printf("Topic:%.*s  ", receiveTopic.lenstring.len, receiveTopic.lenstring.data);//显示接收主题
				  printf("Message:%.*s  ", payloadinlen, payloadin);//显示消息内容的字符串
				  printf("QoS:%d\r\n", qos);//显示接收QoS
				  USART3_RX_STA = 0;//标志位清0
				  //数据控制开发板的程序
				  if(strstr((const char*)payloadin,(const char*)"LED1 ON"))//比对信息内容是不是LED1 ON
				  {
					//   LED_1(1);
					  IOT_publish("LED1 ON OK!");//publish主题发布（发送到云平台）
				  }else if(strstr((const char*)payloadin,(const char*)"LED1 OFF"))//同上
				  {
					//   LED_1(0);
					  IOT_publish("LED1 OFF OK!");//publish主题发布（发送到云平台）
				  }
				}else{
				  printf("Fail to receive message\r\n");//接收错误时的显示
				}
				break;
			case CONNACK: //连接报文确认
				//插入您的处理程序（也可空置）
				break;
			case SUBACK: //订阅请求报文确认
				//插入您的处理程序（也可空置）
				break;
			case UNSUBACK: //取消订阅报文确认
				//插入您的处理程序（也可空置）
				break;
			default:
				//冗余语句
				break;
		  }
		  USART3_RX_STA = 0;//串口3接收标志位清0
	  }

	  //按键操作
	  if(KEY_2())//按下KEY1判断【连接云服务器并订阅主题】
	  {
		  Beep_Mid2();//提示音
		//   while(esp8266_Connect_AP());//连接AP无线路由器热点（热点参数在esp8266.h。WIFI模块已保存热点时可屏蔽）
		  while(esp8266_Connect_IOTServer());//连接TCP连接云服务器（IP和端口参数在esp8266.h文件内修改设置）
		  while(IOT_connect());//用MQTT协议+三元组信息连接阿里云物联网平台（三元组参数在iot.h文件内修改设置）
		  printf("Book to Server\r\n");
		  IOT_subscribe();//主题订阅（订阅成功后才能接收订阅消息）
		  HAL_Delay(100);//等待
	  }

	  if(KEY_2())//按下KEY2判断【向服务器发布信息】
	  {
		  Beep_Mid2();//提示音
		  IOT_publish("TEST Suroy.cn");//publish主题发布（参数是发布信息内容，用双引号包含）
		  HAL_Delay(100);//等待
	  }

  }

}


#ifndef IOT_IOT_H
#define IOT_IOT_H

#include "stm32f1xx_hal.h"
#include "../../../Core/Inc/usart.h"
#include "../mqtt/MQTTPacket.h"
#include "../mqtt/transport.h"

/*********************************************************************************************
 * Author: Suroy
 * Blog: https://suroy.cn
 * 
 * Usage: None
 * 
*********************************************************************************************/

//【三元组信息】在下方修改设置您的物联网云平台提供的三元组信息（手动复制正确信息到双引号内）

#define  PRODUCTKEY           "SUROY" //产品ID（ProductKey）【必须按您的实际情况修改】
#define  PRODUCTKEY_LEN       strlen(PRODUCTKEY) //产品ID长度
#define  DEVICENAME			 "i001" //设备名（DeviceName）【必须按您的实际情况修改】
#define  DEVICENAME_LEN       strlen(DEVICENAME) //设备名长度
#define  DEVICESECRE          "1234" //设备秘钥（DeviceSecret）【必须按您的实际情况修改】
#define  DEVICESECRE_LEN      strlen(DEVICESECRE) //设备秘钥长度

#define  TOPIC_SUBSCRIBE		"/SUROY/i001/user/get" //订阅权限的地址【必须按您的实际情况修改】
#define  TOPIC_QOS				0  //QoS服务质量数值（0/1）
#define  MSGID					1  //信息识别ID
 
#define  TOPIC_PUBLISH			"/SUROY/i001/user/update/error" //发布权限的地址【必须按您的实际情况修改】

#define  MQTTVERSION			4 //MQTT协议版本号（3表示V3.1，4表示V3.1.1）
#define  KEEPALIVEINTERVAL		120 //保活计时器，服务器收到客户端消息（含心跳包）的最大间隔（单位是秒）

extern uint16_t buflen; //临时缓存数量
extern unsigned char buf[200]; //临时缓存数组

uint8_t IOT_connect(void); //IOT物联网平台连接
void IOT_ping(void); //IOT物联网平台PING（心跳包）
uint8_t IOT_subscribe(void);//subscribe主题订阅（订阅成功后才能接收订阅消息）
uint8_t IOT_publish(char* payload);//publish主题发布（参数是发布信息内容，用双引号包含）
void IOT_publish_printf (char *fmt, ...); // 用于主题发布的printf
void IOT_Work(void); // 初始化


#endif


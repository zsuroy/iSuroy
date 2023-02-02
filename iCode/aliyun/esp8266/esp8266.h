#ifndef ESP8266_ESP8266_H
#define ESP8266_ESP8266_H

#include "stm32f1xx_hal.h"
#include "../../wifi/wifi.h"
#include "../../../Core/Inc/usart.h"

//【网络连接信息】在下方修改设置您的路由器热点和物联网平台IP地址+端口号信息（手动复制正确信息到双引号内）

#define SSID "SUROY" //无线路由器热点名称【必须按您的实际情况修改】
#define PASS "8888"   //无线路由器热点密码【必须按您的实际情况修改】

#define IOT_DOMAIN_NAME "iot-06z00iylhpfrg6v.mqtt.iothub.aliyuncs.com" //云服务器IP地址【必须按您的实际情况修改】
#define IOT_PORTNUM 	"1883" //云服务器端口号

uint8_t esp8266_send_cmd(char *cmd, char *ack, uint16_t waittime);
uint8_t* esp8266_check_cmd(uint8_t *str);
uint8_t esp8266_Connect_IOTServer(void); //连接物联网云服务器IP
uint8_t esp8266_Connect_AP(void); //连接AP路由器
uint8_t esp8266_Connect_Server(void); //连接服务器
uint8_t esp8266_quit_trans(void); //判断指令退出

#endif


/*********************************************************************************************
 * Author: Suroy
 * Blog: https://suroy.cn
 * 
 * Usage:
 * 1. 初始化调用 
 * HAL_Delay(500); // 延时等待MCU稳定
 * DHT11_Init();
 * HAL_Delay(1500); // 延时等待DHT11稳定
 * 2. 读数据调用 
 * DHT11_ReadData(DHT11_BUF);
 * printf("DHT11: %02d%c - %02d 'C\r\n", DHT11_BUF[0], '%' , DHT11_BUF[1]); //02:23:24:024 -> DHT11: 81% - 12 'C
 * HAL_Delay(1500); // 等待下一次

*********************************************************************************************/

#include "dht11.h"
#include "main.h"

uint8_t DHT11_BUF[2] = {0}; // 存放DHT11数据

void DHT11_Pin_OUT (void){ //端口变为输出
    __HAL_RCC_GPIOA_CLK_ENABLE();
	GPIO_InitTypeDef GPIO_InitStructure = {0}; 	
    GPIO_InitStructure.Pin = DHT11_Pin;
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStructure.Pull = GPIO_NOPULL;
    GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);
}

void DHT11_Pin_IN (void){ //端口变为输入
	GPIO_InitTypeDef GPIO_InitStructure = {0}; 	
    GPIO_InitStructure.Pin = DHT11_Pin; //选择端口号（0~15或all）                        
    GPIO_InitStructure.Mode = GPIO_MODE_INPUT; //选择IO接口工作方式  
    GPIO_InitStructure.Pull = GPIO_PULLUP;
    GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH; 
	HAL_GPIO_Init(DHT11_GPIO_Port, &GPIO_InitStructure);
}

void DHT11_RST (void){ //DHT11端口复位，发出起始信号（IO发送）
	DHT11_Pin_OUT();
	HAL_GPIO_WritePin(DHT11_GPIO_Port, DHT11_Pin, GPIO_PIN_RESET); //	low
	HAL_Delay(20); //拉低至少18ms						
	HAL_GPIO_WritePin(DHT11_GPIO_Port, DHT11_Pin, GPIO_PIN_SET); //	
	Delay_Us(30); //主机拉高20~40us
}

uint8_t Dht11_Check(void){ //等待DHT11回应，返回1:未检测到DHT11，返回0:成功（IO接收）	   
    uint8_t retry=0;
    DHT11_Pin_IN();//IO到输入状态	 
    while (HAL_GPIO_ReadPin(DHT11_GPIO_Port,DHT11_Pin)&&retry<100){//DHT11会拉低40~80us
        retry++;
        Delay_Us(1);
    }	 
    if(retry>=100)return 1; else retry=0;
    while (!HAL_GPIO_ReadPin(DHT11_GPIO_Port,DHT11_Pin)&&retry<100){//DHT11拉低后会再次拉高40~80us
        retry++;
        Delay_Us(1);
    }
    if(retry>=100)return 1;	    
    return 0;
}

uint8_t Dht11_ReadBit(void){ //从DHT11读取一个位 返回值：1/0
    uint8_t retry=0;
    while(HAL_GPIO_ReadPin(DHT11_GPIO_Port,DHT11_Pin)&&retry<100){//等待变为低电平
        retry++;
        Delay_Us(1);
    }
    retry=0;
    while(!HAL_GPIO_ReadPin(DHT11_GPIO_Port,DHT11_Pin)&&retry<100){//等待变高电平
        retry++;
        Delay_Us(1);
    }
    Delay_Us(40);//等待40us	//用于判断高低电平，即数据1或0
    if(HAL_GPIO_ReadPin(DHT11_GPIO_Port,DHT11_Pin))return 1; else return 0;		   
}

uint8_t Dht11_ReadByte(void){  //从DHT11读取一个字节  返回值：读到的数据
    uint8_t i,dat;
    dat=0;
    for (i=0;i<8;i++){ 
        dat<<=1; 
        dat|=Dht11_ReadBit();
    }						    
    return dat;
}

/**
 * @brief DHT11初始化
 * 
 * @return uint8_t 
 */
uint8_t DHT11_Init(void){
    //APB2外设时钟使能已在 main.c 执行      
	DHT11_RST();//DHT11端口复位，发出起始信号
	return Dht11_Check(); //等待DHT11回应
}

/**
 * @brief 读取一次温湿度数据值
 * @note 
 * 湿度值(十进制，范围:20%~90%) 
 * 温度值(十进制，范围:0~50°)
 * @param h 存储地址
 * @return uint8_t 0,正常;1,失败
 */
uint8_t DHT11_ReadData(uint8_t *h){ 
    uint8_t buf[5];
    uint8_t i;
    DHT11_RST();//DHT11端口复位，发出起始信号
    if(Dht11_Check()==0){ //等待DHT11回应
        for(i=0;i<5;i++){//读取5位数据
            buf[i]=Dht11_ReadByte(); //读出数据
        }
        if((buf[0]+buf[1]+buf[2]+buf[3])==buf[4]){	//数据校验
            *h=buf[0]; //将湿度值放入指针1
			h++;
            *h=buf[2]; //将温度值放入指针2
        }
    }else return 1;
    return 0;	    
}
 

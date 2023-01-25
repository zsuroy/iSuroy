/*********************************************************************************************
 * Author: Suroy
 * Blog: https://suroy.cn
 * 
 * Usage:
 * 1. ��ʼ������ 
 * HAL_Delay(500); // ��ʱ�ȴ�MCU�ȶ�
 * DHT11_Init();
 * HAL_Delay(1500); // ��ʱ�ȴ�DHT11�ȶ�
 * 2. �����ݵ��� 
 * DHT11_ReadData(DHT11_BUF);
 * printf("DHT11: %02d%c - %02d 'C\r\n", DHT11_BUF[0], '%' , DHT11_BUF[1]); //02:23:24:024 -> DHT11: 81% - 12 'C
 * HAL_Delay(1500); // �ȴ���һ��

*********************************************************************************************/

#include "dht11.h"
#include "main.h"

uint8_t DHT11_BUF[2] = {0}; // ���DHT11����

void DHT11_Pin_OUT (void){ //�˿ڱ�Ϊ���
    __HAL_RCC_GPIOA_CLK_ENABLE();
	GPIO_InitTypeDef GPIO_InitStructure = {0}; 	
    GPIO_InitStructure.Pin = DHT11_Pin;
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStructure.Pull = GPIO_NOPULL;
    GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);
}

void DHT11_Pin_IN (void){ //�˿ڱ�Ϊ����
	GPIO_InitTypeDef GPIO_InitStructure = {0}; 	
    GPIO_InitStructure.Pin = DHT11_Pin; //ѡ��˿ںţ�0~15��all��                        
    GPIO_InitStructure.Mode = GPIO_MODE_INPUT; //ѡ��IO�ӿڹ�����ʽ  
    GPIO_InitStructure.Pull = GPIO_PULLUP;
    GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH; 
	HAL_GPIO_Init(DHT11_GPIO_Port, &GPIO_InitStructure);
}

void DHT11_RST (void){ //DHT11�˿ڸ�λ��������ʼ�źţ�IO���ͣ�
	DHT11_Pin_OUT();
	HAL_GPIO_WritePin(DHT11_GPIO_Port, DHT11_Pin, GPIO_PIN_RESET); //	low
	HAL_Delay(20); //��������18ms						
	HAL_GPIO_WritePin(DHT11_GPIO_Port, DHT11_Pin, GPIO_PIN_SET); //	
	Delay_Us(30); //��������20~40us
}

uint8_t Dht11_Check(void){ //�ȴ�DHT11��Ӧ������1:δ��⵽DHT11������0:�ɹ���IO���գ�	   
    uint8_t retry=0;
    DHT11_Pin_IN();//IO������״̬	 
    while (HAL_GPIO_ReadPin(DHT11_GPIO_Port,DHT11_Pin)&&retry<100){//DHT11������40~80us
        retry++;
        Delay_Us(1);
    }	 
    if(retry>=100)return 1; else retry=0;
    while (!HAL_GPIO_ReadPin(DHT11_GPIO_Port,DHT11_Pin)&&retry<100){//DHT11���ͺ���ٴ�����40~80us
        retry++;
        Delay_Us(1);
    }
    if(retry>=100)return 1;	    
    return 0;
}

uint8_t Dht11_ReadBit(void){ //��DHT11��ȡһ��λ ����ֵ��1/0
    uint8_t retry=0;
    while(HAL_GPIO_ReadPin(DHT11_GPIO_Port,DHT11_Pin)&&retry<100){//�ȴ���Ϊ�͵�ƽ
        retry++;
        Delay_Us(1);
    }
    retry=0;
    while(!HAL_GPIO_ReadPin(DHT11_GPIO_Port,DHT11_Pin)&&retry<100){//�ȴ���ߵ�ƽ
        retry++;
        Delay_Us(1);
    }
    Delay_Us(40);//�ȴ�40us	//�����жϸߵ͵�ƽ��������1��0
    if(HAL_GPIO_ReadPin(DHT11_GPIO_Port,DHT11_Pin))return 1; else return 0;		   
}

uint8_t Dht11_ReadByte(void){  //��DHT11��ȡһ���ֽ�  ����ֵ������������
    uint8_t i,dat;
    dat=0;
    for (i=0;i<8;i++){ 
        dat<<=1; 
        dat|=Dht11_ReadBit();
    }						    
    return dat;
}

/**
 * @brief DHT11��ʼ��
 * 
 * @return uint8_t 
 */
uint8_t DHT11_Init(void){
    //APB2����ʱ��ʹ������ main.c ִ��      
	DHT11_RST();//DHT11�˿ڸ�λ��������ʼ�ź�
	return Dht11_Check(); //�ȴ�DHT11��Ӧ
}

/**
 * @brief ��ȡһ����ʪ������ֵ
 * @note 
 * ʪ��ֵ(ʮ���ƣ���Χ:20%~90%) 
 * �¶�ֵ(ʮ���ƣ���Χ:0~50��)
 * @param h �洢��ַ
 * @return uint8_t 0,����;1,ʧ��
 */
uint8_t DHT11_ReadData(uint8_t *h){ 
    uint8_t buf[5];
    uint8_t i;
    DHT11_RST();//DHT11�˿ڸ�λ��������ʼ�ź�
    if(Dht11_Check()==0){ //�ȴ�DHT11��Ӧ
        for(i=0;i<5;i++){//��ȡ5λ����
            buf[i]=Dht11_ReadByte(); //��������
        }
        if((buf[0]+buf[1]+buf[2]+buf[3])==buf[4]){	//����У��
            *h=buf[0]; //��ʪ��ֵ����ָ��1
			h++;
            *h=buf[2]; //���¶�ֵ����ָ��2
        }
    }else return 1;
    return 0;	    
}
 

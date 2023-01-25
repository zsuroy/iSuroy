/*********************************************************************************************
 * Author: Suroy
 * Blog: https://suroy.cn
 * 
 * Note:
 * 此文件为 RTC 走时优化版本(即 V1.0.4) 分离出来的文件
 * 【仅备份使用，编译时请忽略此文件夹】

*********************************************************************************************/


#include "rtc.h"

//以下2行全局变量，用于RTC时间的读取与读入
uint16_t ryear; //4位年
uint8_t rmon,rday,rhour,rmin,rsec,rweek;//2位月日时分秒周

void RTC_Init(void) //用户自建的带有上电BPK判断的RTC初始化
{
  hrtc.Instance = RTC;
  hrtc.Init.AsynchPrediv = RTC_AUTO_1_SECOND;
  hrtc.Init.OutPut = RTC_OUTPUTSOURCE_NONE;
  if (HAL_RTC_Init(&hrtc) != HAL_OK)
  {
    Error_Handler();
  }
  if(HAL_RTCEx_BKUPRead(&hrtc,RTC_BKP_DR1)!=0X5050){ //判断是否首次上电
	   HAL_RTCEx_BKUPWrite(&hrtc,RTC_BKP_DR1,0X5050); //标记数值 下次不执行“首次上电”的部分
	   RTC_Set(2022,1,1,0,0,0);//写入RTC时间的操作RTC_Set(4位年,2位月,2位日,2位时,2位分,2位秒)
  }
}

//判断是否是闰年函数
//月份   1  2  3  4  5  6  7  8  9  10 11 12
//闰年   31 29 31 30 31 30 31 31 30 31 30 31
//非闰年 31 28 31 30 31 30 31 31 30 31 30 31
//输入:年份
//输出:该年份是不是闰年.1,是.0,不是
uint8_t Is_Leap_Year(uint16_t year){
	if(year%4==0){ //必须能被4整除
		if(year%100==0){
			if(year%400==0)return 1;//如果以00结尾,还要能被400整除
			else return 0;
		}else return 1;
	}else return 0;
}
//设置时钟
//把输入的时钟转换为秒钟
//以1970年1月1日为基准
//1970~2099年为合法年份

//月份数据表
uint8_t const table_week[12]={0,3,3,6,1,4,6,2,5,0,3,5}; //月修正数据表
const uint8_t mon_table[12]={31,28,31,30,31,30,31,31,30,31,30,31};//平年的月份日期表

//写入时间
uint8_t RTC_Set(uint16_t syear,uint8_t smon,uint8_t sday,uint8_t hour,uint8_t min,uint8_t sec){ //写入当前时间（1970~2099年有效），
	uint16_t t;
	uint32_t seccount=0;
	if(syear<2000||syear>2099)return 1;//syear范围1970-2099，此处设置范围为2000-2099
	for(t=1970;t<syear;t++){ //把所有年份的秒钟相加
		if(Is_Leap_Year(t))seccount+=31622400;//闰年的秒钟数
		else seccount+=31536000;                    //平年的秒钟数
	}
	smon-=1;
	for(t=0;t<smon;t++){         //把前面月份的秒钟数相加
		seccount+=(uint32_t)mon_table[t]*86400;//月份秒钟数相加
		if(Is_Leap_Year(syear)&&t==1)seccount+=86400;//闰年2月份增加一天的秒钟数
	}
	seccount+=(uint32_t)(sday-1)*86400;//把前面日期的秒钟数相加
	seccount+=(uint32_t)hour*3600;//小时秒钟数
	seccount+=(uint32_t)min*60;      //分钟秒钟数
	seccount+=sec;//最后的秒钟加上去

	//【寄存器操作】因为HAL库的不完善，无法直接调用RTC_ReadTimeCounter函数。此处改用寄存器直接操作。
	RTC->CRL|=1<<4;   //允许配置
	RTC->CNTL=seccount&0xffff;
	RTC->CNTH=seccount>>16;
	RTC->CRL&=~(1<<4);//配置更新
	while(!(RTC->CRL&(1<<5)));//等待RTC寄存器操作完成
	//【寄存器操作】结束

	return 0; //返回值:0,成功;其他:错误代码.
}

//读出时间
uint8_t RTC_Get(void){//读出当前时间值 //返回值:0,成功;其他:错误代码.
	static uint16_t daycnt=0;
	uint32_t timecount=0;
	uint32_t temp=0;
	uint16_t temp1=0;

	//【寄存器操作】因为HAL库的不完善，无法直接调用RTC_WriteTimeCounter函数。此处改用寄存器直接操作。
 	timecount=RTC->CNTH;//得到计数器中的值(秒钟数)
	timecount<<=16;
	timecount+=RTC->CNTL;
	//【寄存器操作】结束

	temp=timecount/86400;   //得到天数(秒钟数对应的)
	if(daycnt!=temp){//超过一天了
		daycnt=temp;
		temp1=1970;  //从1970年开始
		while(temp>=365){
		     if(Is_Leap_Year(temp1)){//是闰年
			     if(temp>=366)temp-=366;//闰年的秒钟数
			     else {temp1++;break;}
		     }
		     else temp-=365;       //平年
		     temp1++;
		}
		ryear=temp1;//得到年份
		temp1=0;
		while(temp>=28){//超过了一个月
			if(Is_Leap_Year(ryear)&&temp1==1){//当年是不是闰年/2月份
				if(temp>=29)temp-=29;//闰年的秒钟数
				else break;
			}else{
	            if(temp>=mon_table[temp1])temp-=mon_table[temp1];//平年
	            else break;
			}
			temp1++;
		}
		rmon=temp1+1;//得到月份
		rday=temp+1;  //得到日期
	}
	temp=timecount%86400;     //得到秒钟数
	rhour=temp/3600;     //小时
	rmin=(temp%3600)/60; //分钟
	rsec=(temp%3600)%60; //秒钟
	rweek=RTC_Get_Week(ryear,rmon,rday);//获取星期
	return 0;
}

uint8_t RTC_Get_Week(uint16_t year,uint8_t month,uint8_t day){ //按年月日计算星期(只允许1901-2099年)//已由RTC_Get调用
	uint16_t temp2;
	uint8_t yearH,yearL;
	yearH=year/100;
	yearL=year%100;
	// 如果为21世纪,年份数加100
	if (yearH>19)yearL+=100;
	// 所过闰年数只算1900年之后的
	temp2=yearL+yearL/4;
	temp2=temp2%7;
	temp2=temp2+day+table_week[month-1];
	if (yearL%4==0&&month<3)temp2--;
	return(temp2%7); //返回星期值
}


/*于 main.c 中调用demo

RTC_Init();//自创走时完善的RTC时钟初始化函数

while (1)
  {
	if(USART1_RX_STA&0xC000){ //如果标志位是0xC000表示收到数据串完成，可以处理。
	   if((USART1_RX_STA&0x3FFF)==0){ //单独的回车键再显示一次欢迎词
		   RTC_Get();//读出当前RTC日期与时间，放入全局变量
		   printf(" SUROY_RTC_DEBUG   \r\n");
		   printf(" 实时时间：%04d-%02d-%02d  %02d:%02d:%02d  \r\n",
				   ryear, rmon, rday, rhour, rmin, rsec);//显示日期时间
		   printf(" 单按回车键更新时间，输入字母C初始化时钟 \r\n");
		   printf(" 请输入设置时间，格式20170806120000，按回车键确定！ \r\n");
	   }else if((USART1_RX_STA&0x3FFF)==1){  //判断数据是不是1个
		   if(USART1_RX_BUF[0]=='c' ||  USART1_RX_BUF[0]=='C'){
			   MX_RTC_Init(); //键盘输入c或C，初始化时钟（调用HAL库自带的初始化函数）
			   printf("初始化成功！       \r\n");//显示初始化成功
		   }else{
			   printf("指令错误！           \r\n"); //显示指令错误！
		   }
	   }else if((USART1_RX_STA&0x3FFF)==14){ //判断数据是不是14个
		   //将超级终端发过来的数据换算并写入RTC
		   ryear = (USART1_RX_BUF[0]-0x30)*1000 + (USART1_RX_BUF[1]-0x30)*100 +
				   (USART1_RX_BUF[2]-0x30)*10 + (USART1_RX_BUF[3]-0x30);//减0x30得到十进制0~9的数据
		   rmon =  (USART1_RX_BUF[4]-0x30)*10 + (USART1_RX_BUF[5]-0x30);
		   rday =  (USART1_RX_BUF[6]-0x30)*10 + (USART1_RX_BUF[7]-0x30);
		   rhour = (USART1_RX_BUF[8]-0x30)*10 + (USART1_RX_BUF[9]-0x30);
		   rmin =  (USART1_RX_BUF[10]-0x30)*10 + (USART1_RX_BUF[11]-0x30);
		   rsec =  (USART1_RX_BUF[12]-0x30)*10 + (USART1_RX_BUF[13]-0x30);
		   if (RTC_Set(ryear,rmon,rday,rhour,rmin,rsec) != HAL_OK)//将数据写入RTC程序
		   {
			   printf("写入时间失败！        \r\n"); //显示写入失败
		   }else printf("写入成功！       \r\n");//显示写入成功
	   }else{ //如果以上都不是，即是错误的指令。
		   printf("指令错误！          \r\n");  //如果不是以上正确的操作，显示指令错误！
	   }
	   USART1_RX_STA=0; //将串口数据标志位清0
	}
*/


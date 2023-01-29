<!--suppress HtmlDeprecatedAttribute -->

<div align="center">
  <p>
      <img alt="logo" src="https://raw.githubusercontent.com/zsuroy/iSuroy/master/logo.png"/>
  </p>

  <h1>STM32 项目驱动应用库</h1>
  <p> 基于 STM32F103C8T6 的 HAL 库应用实践模版  </p>
  <p>开发环境: Vscode + Embeded IDE + GCC + Cortex-debug (MacOS)  </p>
  <p>作者: 👨🏻‍💻 Suroy (https://suroy.cn) </p>

  <p>
    <a href="https://suroy.cn"><img alt="SUROY(BLOG)" src="https://img.shields.io/website?down_message=FLOWER&label=SUROY&up_color=ff69b4&up_message=DREAM&logo=micro:bit&url=https%3A%2F%2Fsuroy.cn"></a>
    <a href="https://github.com/zsuroy"><img alt="Suroy" src="https://img.shields.io/github/languages/top/zsuroy/iSuroy?style=flat-square"/></a>
    <a href="https://github.com/zsuroy"><img alt="Suroy" src="https://img.shields.io/github/languages/count/zsuroy/iSuroy?style=flat"/></a>
    <a href="https://github.com/zsuroy"><img alt="GitHub last commit" src="https://img.shields.io/github/last-commit/zsuroy/iSuroy"></a>
    <img alt="GitHub" src="https://img.shields.io/github/license/zsuroy/iSuroy">
  </p>
</div>


 

# 环境搭建

[基于 VsCode + GCC + STM32 环境下的串口输入输出重定向](https://suroy.cn/embeded/serial-port-inputoutput-redirection-based-on-vscode-gcc-stm32-environment.html)

[ARM 嵌入式基于 STM32CUBEMX 开发指南](https://suroy.cn/embeded/arm-embedded-development-guide-based-on-stm32cubemx.html)

# 版本说明

## V1.0.0

> 2022.1.23 

已有功能如下：

1. 按键驱动
2. LED闪烁
3. 无源蜂鸣器
4. 继电器控制
5. USART 串口收发及输入输出重定向
6. ADC + DMA 采样
   + 直接 ADC 采样
   + DMA 单通道模式采样 


## V1.0.1

> 优化 DMA 功能  
> 2022.1.23
> 

新增功能:

1. ADC + DMA 双通道采样

   + 调整原因：ADC2 无法使用 DMA 功能，故将 ADC2 的 IN5(通道五) 调整到 ADC1 的通道五，同时启用 DMA 的循环接收功能

   + ADC1 启用 IN4、IN5 双通道进行 ADC 采样


## V1.0.2

> 新增 RTC 时钟功能  
> 2022.1.24  
> 

采用BKP寄存器写入上电标志以确保掉电之后时间保留。
即当断电之后，下次上电走时接续上次继续；
若存在备用电源可以确保时间正常运作。


## V1.0.3

> RTC 时钟功能新增配置时间及初始化  
> 2022.1.24

注意:

使用时需要及时清空串口接收标志，及标记串口接收缓存区。
详细请查看 `RTC_Command(uint8_t str[], uint8_t opt)` 函数

```c
USART1_RX_STA=0; // 串口接收标志清0
USART1_RX_BUF[0] = '\0'; //标记清空串口接收缓存
```

当前版本的 RTC 时钟断电复位后，重新上电后仅仅会保留时间，而日期会被清零复位，若需要创建完善版本的 RTC 功能见 `[V1.0.4])`


## V1.0.4

> RTC 时钟走时优化: 断电之后日期时间均可正常保留  
> 2022.1.25

注意： 无备用电池，未得到有效测试结果，如需使用建议先调试。

使用说明：

1. 禁用 CubeMX 自动生成 RTC 函数初始化  
2. 采用修改的 MX_RTC_INIT() 函数进行初始化并在 main.c 调用

或可参阅分离出来的程序 `iCode/rtc/rtc.c`


## V1.0.5

> 新增 DHT11 温湿度传感器驱动  
> 2022.1.26

调用说明:


1. 初始化调用 

```c
HAL_Delay(500); // 延时等待MCU稳定
DHT11_Init();
HAL_Delay(1500); // 延时等待DHT11稳定
```

2. 读数据调用 

```c
DHT11_ReadData(DHT11_BUF);
printf("DHT11: %02d%c - %02d 'C\r\n", DHT11_BUF[0], '%' , DHT11_BUF[1]); //02:23:24:024 -> DHT11: 81% - 12 'C
HAL_Delay(1500); // 等待下一次
```

⚠️ 注意：此版本忽略了 V1.0.4 走时优化，还原 V1.0.3 的 RTC 功能

## V1.0.6

> 新增 USB_Slave 驱动，作虚拟串口通讯使用  
> 2022.1.26

使用说明:

1. CubeMX 生成代码时需要分配堆栈空间以防编译/运行出错  
   + Minimun XXXX Size: 原来 -> 目标大小
   + Minimun Heap Size: 0x200 -> 0x1000
   + Minimun Stack Size: 0x400 -> 0x1000

2. 添加项目资源
   + `/USB_DEVICE/` USB驱动文件夹
   + `/Middlewares/` 中间件驱动文件夹
   + PS: **最后添加对应项目头文件包含目录**

3. 修改对应功能函数
   * `/USB_DEVICE/App/usbd_cdc_if.c` 虚拟串口通讯文件
      + `CDC_Receive_FS` USB接收函数
      + `CDC_Transmit_FS` USB发送函数
      + `USB_printf` 自定义封装的模拟串口发送函数
   * `/USB_DEVICE/App/usbd_cdc_if.h` 虚拟串口通讯头文件


⚠️ 注意事项：

初始化时需要: `HAL_CAN_MspDeInit(&hcan); // 关闭CAN功能，使USB功能可以被电脑识别` 

由于USB与CAN共用一个RAM空间，不能**同时**使用，若需要使用 CAN 时可以重新初始化。


PS: 构思一下，物理外挂那不就来了🐶



## V1.0.7

> 新增节能模式：睡眠模式、停机模式、待机模式  
> 2022.1.26


使用说明:  
> `/iCode/eco/eco.c` 文件

1. 在 main.h 中导出时钟函数
```c
extern void SystemClock_Config(void); // 导出时钟函数以便于节能模式调用
```

2. 停机模式中配置
   > 此例中使用 key2 按下触发外部中断来唤醒

   + 需要配置1个GPIO作为外部触发中断(GPIO_EXTI)以唤醒: e.g. PA0 -> GPIO_EXTI0
   + 参数如下: GPIO_Mode -> 下降沿触发; GPIO_Pull-up 上拉
   + NVIC 中允许 EXTI0

3. 待机模式配置
   > 此例中使用 SYS_WKUP 进行触发唤醒

   + 为了能够正常使用 key2 的按键功能，仍旧使用 CubeMX 生产 PA0 作为普通的 GPIO 输入
   + 参数如下: GPIO_Mode -> INPUT; GPIO_Pull-up/Down 上拉
   + 而待机模式中采用自定义函数功能的方式将其设置为 SYS_WKUP 功能
   + 使用说明
     * 长按 Key2 按键启动待机模式；按RST键从待机模式中唤醒 🤡 
     * 调试建议将对应函数直接复制到 main.c 中测试
   + 已知问题： 若直接将 PA0 键设置为 SYS_WKUP 有时候会导致编译报错，可能是 `eco.c` 文件到问题



## V1.0.8

> CRC 校验功能  
> 2023.1.27

使用说明:

1. CubeMX 生产CRC代码
   + Computing -> CRC: Activated

2. 编写 CRC 程序
   + `Core/Src/crc.c` 修改CRC程序
   + `CRCBUF` 即存储需要校验数据缓冲区

3. 芯片ID读取
   + `void ID_Get(uint8_t show);`


## V1.0.9

> 外部中断实验  
> 2023.1.27

按键中断实验: `/iCode/key/key.c`
> 使用外部触发中断以确保按键不受主程序中耗时影响，达到实时响应效果。  
   + 需要配置1个GPIO作为外部触发中断(GPIO_EXTI): e.g. PA0 -> GPIO_EXTI0
   + 参数如下: GPIO_Mode -> 下降沿触发; GPIO_Pull-up 上拉   （按键引脚初始为高电平，按下低电平，此时产生一个下降沿信号）
   + NVIC 中允许 EXTI0
   + 😋Tips: 若多个GPIO共用一个外部中断，可以在中断回调函数中判断各GPIO电平以确定具体是哪一个产生了中断。
   + 😅外部中断回调函数中均不能使用 HAL 库自带的 `HAL_Delay() // 延时函数`；可以使用自写的延时 `Delay_Us()`
   + 调用逻辑分析:
   + `stm32f1xx_it.c: EXTI0_IRQHandler() -> stm32f1xx_hal_gpio.c: HAL_GPIO_EXTI_IRQHandler() -> HAL_GPIO_EXTI_Callback()`

🙄异常说明：
> 开启PA0点外部中断(NVIC)，编译可能会提示链接报错 `undefined reference to 'HAL_UART_Transmit'`  
> 原因在于CubeMX生成的默认中断优先级均为0会产生冲突

解决如下： NVIC -> 调整 EXTI0 的外部中断优先级编号为 1



## V1.0.10

> 新增定时器、PWM功能  
> 2023.1.27

1. TIMx 介绍
通用 TIMx (TIM2 、 TIM3 、 TIM4 和 TIM5) 定时器功能包括：
• 16 位向上、向下、向上 / 向下自动装载计数器  
• 16 位可编程 ( 可以实时修改 ) 预分频器，计数器时钟频率的分频系数为 1 ～ 65536 之间的任意数值  
• 4 个独立通道： ─ 输入捕获 ─ 输出比较 ─ PWM 生成 ( 边缘或中间对齐模式 ) ─ 单脉冲模式输出  
• 使用外部信号控制定时器和定时器互连的同步电路  
• 如下事件发生时产生中断 /DMA ： ─ 更新：计数器向上溢出 / 向下溢出，计数器初始化 ( 通过软件或者内部 / 外部触发 ) ─ 触发事件 ( 计数器启动、停止、初始化或者由内部 / 外部触发计数 ) ─ 输入捕获 ─ 输出比较  
• 支持针对定位的增量 ( 正交 ) 编码器和霍尔传感器电路  
• 触发输入作为外部时钟或者按周期的电流管理


2. 普通定时
   > 通常选 TIM1 之外的定时器使用 `/Core/Src/tim.c`  
   + 参数说明
      + Tout  = (ARR+1) * (PCS+1) / Tclk
      + 定时时间(us) = (计数周期+1) * (分频系数+1) / 输入时钟频率(MHz)
      + Tclk: CubeMX 时钟树 -> APB1 Timer clocks(MHz)
      + APR/PCS: CubeMX 定时器窗口中的属性参数

   + 配置说明
      > e.g. TIM2 定时 5s = 5000000us = (7199+1) * (49999+1) / 72
      + CubeMX: Timers -> TIM2 -> Clock Source [Internal Clock] -> Parameter Settings（下面） -> NVIC 允许TIM2中断 -> 设置中断优先级为2防止冲突
      + Parameter Settings:  
        + 分频系数 PSC [4999]
        + 计数模式 Up
        + 计数周期 Couter Period [7199]
        + 自动重载初值 auto-reload preload [enable] 
          > 无论允许或禁止，中断处理程序均会定时器自动重载初值；开启时，重载初值不会立马生效而是在下一个更新事件出现时由影子寄存器传入；禁止时，写入自动重载寄存器立马生效  

   + 代码编写
     + 初始化 `MX_TIM2_Init();`
     + 开启定时器中断 `HAL_TIM_Base_Start_IT(&htim2);` 必须要先开启才能进入中断处理回调函数
     + 重新编写中断回调函数 `HAL_TIM_PeriodElapsedCallback()`
     + 👻注意：TIM中断开启后，若回调函数中不清空中断标志位可能会导致不进 while(1) 主程序
     + 解决: 中断回调函数中调用 `__HAL_TIM_CLEAR_FLAG(&htim2, TIM_FLAG_UPDATE); //清空定时器中断标志位`


3. PWM脉宽调制
   > 通过控制占空比以实现对LED亮度、步进电机等的控制
   > 无电路，该PWM未经过实际测试
   + 配置说明
     + e.g. 选PB0作为PWM调光LED
     + CubeMX
       + GPIO -> PB0 -> TIM3_CH3 (定时器3通道3)；
       + Timer -> TIM3 -> 勾选 Internal Clock -> Channel 3 [PWM Generation CH3]
       + Parameter Settings
         + Counter Setting: PSC [71]; CounterMode [Up]; CounterPeriod [499]; auto-reload Preload [enable];
         + PWM Generation Ch3: Mode [PWM mode 1]; Pulse [0]; Output compare preload [输出占空比, enable]; FastMode [disable]; CH Polarity [通道极性, High]
         + Tips: 通道极性即占空比中“占”的输出
         + NVIC 使能 TIM3  
         + GPIO: GPIO mode [交替推挽, Alternate Function Push Pull]; speed [High]; Label [LED]
   + 使用说明
      + 初始化: `MX_TIM2_Init();//TIM3初始化` ， `HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_3); // 开启定时器PWM输出`
      + 占空比设置: `__HAL_TIM_SetCompare(&htim3, TIM_CHANNEL_3, PWM_Value); // 设置占空比函数`
      + 自写函数: `uint32_t TIM_PWM_Set(void); //获取PWM值`



## V1.0.11

> WIFI模块通讯  
> 2023.1.29


1. 调试技巧

> 若无其他工具协助调试时，可以直接采用串口电平转发的形式实现将A串口数据转发到 ESP8266 所用B串口

```c
// 使用前需要先在程序中删除 USART1、USART3相关代码；采用GPIO端口电平互传，相当于导线连接
while(1)
{
   HAL_GPIO_Write(GPIOB, GPIO_PIN_10, HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_10)); // 将PA10(USART1-RX) 到电平状态转发到PB11(USART3-TX)

   HAL_GPIO_Write(GPIOB, GPIO_PIN_9, HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_11)); // 将PA10(USART1-TX) 到电平状态转发到PB11(USART3-RX)
}
```

2. 常用AT指令集

```text

// 复位
AT+RST

// 设置Sta模式
AT+CWMODE=1

// 查询周围WIFI
AT+CWLAP

// 查询FLASH中配置的AP信息
AT+CWJAP_DEF?

// 配置AP
AT+CWJAP_DEF="SUROY_iNet","Suroy921-"

// 查询IP地址
AT+CIFSR

// [本机作客户端] 设置设备单连接模式(0,单连接；1,多连接)
AT+CIPMUX=0

// 建立TCP连接
AT+CIPSTART="TCP","192.168.123.1",8888

// 发送数据(5个字节)
AT+CIPSEND=5

// 断开连接
AT+CIPCLOSE

// 断开WIFI
AT+CWQAP

```

3. 基础知识回顾

\n ： 换行符（newline），另起一行，对应ASCII值10，16进制0x0D（缩写：LF）。  
\r ： 回车符（return），回到一行的开头，对应ASCII值13，16进制0x0A（缩写：CR）。  
前者使光标到行首，后者使光标下移一格。通常用的Enter是两个加起来。要想通用的话，最好用\r\n换行。
Windows系统里面，每行结尾是 回车+换行（CR+LF），即“\r\n”；
Unix系统里，每行结尾只有 换行CR，即“\n”；
Mac系统里，每行结尾是 回车CR 即‘\r’；
我们平时编写文件的回车符应该确切来说叫做回车换行符；


注意：实际项目应用时需要变更 wifi.c 中程序逻辑，当前仅接收1个字节数据。



🌏 TodoList  

- [ ] RS485 通讯驱动
- [ ] CAN 通讯驱动
- [ ] SPI 闪存芯片驱动
- [ ] 蓝牙驱动(模块坏了🌚)


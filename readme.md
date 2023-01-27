<!--suppress HtmlDeprecatedAttribute -->

<div align="center">
  <p>
      <img alt="logo" src="https://raw.githubusercontent.com/zsuroy/iSuroy/master/logo.png"/>
  </p>

  <h1>STM32 项目驱动应用库</h1>
  <p> 基于 STM32F103C8T6 的 HAL 库应用实践模版  </p>
  <p>开发环境: Vscode + Embeded IDE + GCC + Cortex-debug (MacOS)  </p>
  <p>作者: Suroy (https://suroy.cn) </p>

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

按键中断实验:
> 使用外部触发中断以确保按键不受主程序中耗时影响，达到实时响应效果。  
   + 需要配置1个GPIO作为外部触发中断(GPIO_EXTI): e.g. PA0 -> GPIO_EXTI0
   + 参数如下: GPIO_Mode -> 下降沿触发; GPIO_Pull-up 上拉   （按键引脚初始为高电平，按下低电平，此时产生一个下降沿信号）
   + NVIC 中允许 EXTI0
   + 😋Tips: 若多个GPIO共用一个外部中断，可以在中断回调函数中判断各GPIO电平以确定具体是哪一个产生了中断。
   + 😅外部中断回调函数中均不能使用 HAL 库自带的 `HAL_Delay() // 延时函数`；可以使用自写的延时 `Delay_Us()`
   + 调用逻辑分析:
   + `stm32f1xx_it.c: EXTI0_IRQHandler() -> stm32f1xx_hal_gpio.c: HAL_GPIO_EXTI_IRQHandler() -> HAL_GPIO_EXTI_Callback()`

🙄玄学说明：
> 开启PA0点外部中断(NVIC)，编译可能会提示链接报错 `undefined reference to 'HAL_UART_Transmit'`，目前原因未知

解决：原因在于CubeMX生成的默认中断优先级均为0会产生冲突
NVIC -> 调整 EXTI0 的外部中断优先级编号为 1
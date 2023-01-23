# STM32 项目驱动应用库
> 基于 STM32F103C8T6 的 HAL 库应用实践模版
> 开发环境: Vscode + Embeded IDE + GCC + Cortex-debug (MacOS)
> 作者: Suroy (https://suroy.cn)

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
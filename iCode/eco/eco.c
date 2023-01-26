/*********************************************************************************************
 * Author: Suroy
 * Blog: https://suroy.cn
 * 
 * Note:
 * 此文件为 Energy Saving Mode 节能模式
*********************************************************************************************/


#include "eco.h"
#include <stdio.h>

/**
 * @brief 节能模式配置
 * @note
 * 建议直接将本函数内程序代码复制到 main.c 中 while(1) 处执行
 * 
 * 模式名    关闭功能   唤醒
 * 睡眠模式: ARM内核(CPU)    所有内部/外部功能的中断/事件
 * 
 * 停机模式：ARM内核/内部所有功能/PLL分频器/HSE   外部中断输入接口EXT1(16个IO之一)/RTC闹钟到时/USB唤醒信号/电源电压监控中断PVD
 *          从停止模式唤醒，并重新开启时钟后，还可以从上次停止处继续执行代码。
 *          SRAM和寄存器内容被保留下来，在停止模式下，所有的I/O引脚都保持它们在运行模式时的状态。
 * 
 * 待机模式： ARM内核/内部所有功能/PLL分频器/HSE/SRAM内容消失   独立看门狗WDG复位/NRST接口的外部复位信号/专用唤醒WKUP引脚/RTC闹钟到时
 *          待机模式唤醒测试有BUG，可以按复位键替代～
 * 
 *  * @param mode 1睡眠模式；2停机模式； 3待机模式
 */
void ECO_Config(uint8_t mode)
{
    if(mode == 1)
    { // 睡眠模式: 效果并不明显，因为系统几乎始终会有中断唤醒
        printf("Enter SLEEP Mode, NVIC wake-up\r\n");
        // 参数：PWR_LOWPOWERREGULATOR_ON 低功耗电源开启；  PWR_SLEEPENTRY_WFE 中断事件唤醒
        HAL_PWR_EnterSLEEPMode(PWR_MAINREGULATOR_ON, PWR_SLEEPENTRY_WFI); // 主电源开启，进入睡眠模式（任意中断可唤醒）
    }
    else if(mode == 2)
    { // 停机模式: 唤醒后接到上次停止处执行
       printf("Enter STOP Mode, Press key2 wake-up\r\n");
       HAL_PWR_EnterSTOPMode(PWR_LOWPOWERREGULATOR_ON, PWR_STOPENTRY_WFI); //低功耗电源开启，进入停机模式
       // 此处进入停机状态！

       // 接下来的程序在唤醒后执行
       SystemClock_Config(); // 唤醒后重新初始化时钟 
       printf("Exit STOP Mode\r\n");

    }
    else if(mode == 3)
    { // 待机模式：效果约等同于复位
      if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) == GPIO_PIN_RESET ) //PA0
      { // KEY2按键按下 -> 进入待机模式
        HAL_Delay(50); //去抖动
        if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) == GPIO_PIN_RESET)
        {
            // 进入待机模式
            printf("Enter STANDBY Mode, Press WKUP to wake-up\r\n");
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_RESET); // PA0端口变为低电平，准备唤醒键初始电平
            __HAL_PWR_CLEAR_FLAG(PWR_FLAG_WU); // 清除 WKUP唤醒键 状态位，为接收上升沿信号作准备
            HAL_PWR_EnableWakeUpPin(PWR_WAKEUP_PIN1); // 使能WKUP唤醒键的唤醒功能(使能PA0)
            HAL_PWR_EnterSTANDBYMode(); //进入待机模式
            // 待机模式下无法继续运行，等同于复位操作，回到主函数从新运行，故无后续恢复程序，判断待机模式恢复应写入到主函数初始化里面
        }
      }
    }
    else return;
}


/**
 * @brief 待机模式: 唤醒检测
 * @note
 * 在 main.c 初始化处(即 while(1) 之前) 调用 
 */
void ECO_STANDBY_Check(void)
{
    if(__HAL_PWR_GET_FLAG(PWR_FLAG_SB) != RESET)
    { //从待机模式中唤醒
    printf("Wake-up from STANDBY Mode\r\n");
    HAL_PWR_DisableWakeUpPin(PWR_WAKEUP_PIN1); // 禁止WKUP引脚唤醒的功能
    __HAL_PWR_CLEAR_FLAG(PWR_FLAG_SB); // 清除 WKUP唤醒键 标志位
    //于此插入相应唤醒程序

    }
    else return;
}


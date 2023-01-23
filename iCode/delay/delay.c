#include "delay.h"


/**
 * @brief 微妙级延时
 * @author Suroy
 * 
 * 利用 CPU 循环空转实现的非精确应用的微妙延时函数
 * 易受中断事件会导致误差： 执行中断处理函数会耗时
 * 不同型号CPU、编译器均会产生误差
 * 
 * @param us 
 */
void Delay_Us(uint32_t us)
{
    uint32_t delay = (HAL_RCC_GetHCLKFreq() / 8000000 * us); // 1us时间内CPU循环的次数 * 需要的延时(us)
    while(delay--); // 循环delay次，达到1us延时
}


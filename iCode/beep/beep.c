#include "beep.h"
#include "../delay/delay.h"

/**
 * @brief 报警音
 * 实用 HAL 库毫秒级延时实现
 */
void Beep_Mid1(void)
{
    uint16_t timer = 50; //单音时长
    uint16_t hz = 1; //单音音调
    for (uint16_t i = 0; i < timer; i++)
    { //循环次数决定单音持续时长
        // 产生一个 hz 方波发音
        HAL_GPIO_WritePin(BEEP_GPIO_Port, BEEP_Pin, GPIO_PIN_SET); //拉高
        HAL_Delay(hz); //延时
        HAL_GPIO_WritePin(BEEP_GPIO_Port, BEEP_Pin, GPIO_PIN_RESET);
        HAL_Delay(hz);
    }    
}


/**
 * @brief 报警音
 * 采用 CPU 空转延时实习
 * 
 */
void Beep_Mid2(void)
{
    uint16_t timer = 200; //单音时长
    uint16_t hz = 500; //单音音调
    for (uint16_t i = 0; i < timer; i++)
    { //循环次数决定单音持续时长
        // 产生一个 hz 方波发音
        HAL_GPIO_WritePin(BEEP_GPIO_Port, BEEP_Pin, GPIO_PIN_SET); //拉高
        Delay_Us(hz); //延时
        HAL_GPIO_WritePin(BEEP_GPIO_Port, BEEP_Pin, GPIO_PIN_RESET);
        Delay_Us(hz);
    }    
}

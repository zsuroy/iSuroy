
#include "led.h"
#include "../key/key.h" // 引用按键声明
#include "../beep/beep.h"
#include "../relay/relay.h"

/**
 * @brief 控制LED亮灭
 * 
 * @param state 1开；0关
 */
void LED_Ctr(uint8_t state)
{
    if(state) HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_RESET);
    else HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_SET);
}

/**
 * @brief LED延时反转
 * 
 */
void LED_Toggle(void)
{
    Beep_Mid1(); // 蜂鸣器
    RELAY_Toggle(); // 继电器
    HAL_GPIO_TogglePin(LED2_GPIO_Port, LED2_Pin);
    // HAL_Delay(1000); //为了在按键外部中断回调函数中使用屏蔽此句延时
}


/**
 * @brief LED心跳检测
 * 
 * @param mode 0高电平时闪烁LED灯; 1按键控制LED灯
 */
void LED_Check(uint8_t mode)
{
    if(!mode && KEY_State())LED_Toggle(); //闪烁模式
    else LED_Ctr(KEY_State()); //按键控制LED灯
}
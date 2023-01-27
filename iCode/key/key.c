#include "key.h"
#include "../delay/delay.h"//中断回调调用
#include "../beep/beep.h"
#include "../led/led.h"
#include <stdio.h>

/**
 * @brief 读取按键状态
 * 
 * @return uint8_t 
 */
uint8_t KEY_Read(void)
{
    return (uint8_t)HAL_GPIO_ReadPin(KEY_GPIO_Port, KEY_Pin);
}

/**
 * @brief 按键开关状态读取
 * 
 * @return uint8_t 1开; 0关
 */
uint8_t KEY_State(void)
{
    if(HAL_GPIO_ReadPin(KEY_GPIO_Port, KEY_Pin) == GPIO_PIN_SET)
    {
        HAL_Delay(20); //去抖动
        if(HAL_GPIO_ReadPin(KEY_GPIO_Port, KEY_Pin) == GPIO_PIN_SET)
        {
            while(HAL_GPIO_ReadPin(KEY_GPIO_Port, KEY_Pin) == GPIO_PIN_SET); // 等待按键松开
            return 0x01; //开
        }
    }

    if(HAL_GPIO_ReadPin(KEY_GPIO_Port, KEY_Pin) == GPIO_PIN_RESET)
    {
        HAL_Delay(20); //去抖动
        if(HAL_GPIO_ReadPin(KEY_GPIO_Port, KEY_Pin) == GPIO_PIN_RESET)
        {
            while(HAL_GPIO_ReadPin(KEY_GPIO_Port, KEY_Pin) == GPIO_PIN_RESET); // 等待按键松开
            return 0x00;
        }    
    }
    return 0x00;
}



/**
 * @brief KEY2 按下检测
 * 
 * @return uint8_t 
 */
uint8_t KEY_2(void)
{
    uint8_t state=0;
    if(HAL_GPIO_ReadPin(KEY_GPIO_Port, KEY2_Pin) == GPIO_PIN_RESET)
    {
        // HAL_Delay(20); //去抖动（外部中断回调函数中不能使用系统自带的延时函数）
        Delay_Us(20000); //去抖
        if(HAL_GPIO_ReadPin(KEY_GPIO_Port, KEY2_Pin) == GPIO_PIN_RESET)
            state=1;
    }
    while(HAL_GPIO_ReadPin(KEY_GPIO_Port, KEY2_Pin) == GPIO_PIN_RESET); // 等待按键松开
    Delay_Us(20000); //去抖
    return state;
}


/**
 * @brief GPIO外部中断回调函数
 * @note
 * 此处设置为检测 KEY2 按下
 * @param GPIO_Pin 
 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if(GPIO_Pin == KEY2_Pin)
    {
        if(KEY_2())
        {
            printf("Enter EXTI0 \r\n");
            Beep_Mid2();
            HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_SET); //close
            // HAL_GPIO_TogglePin(LED2_GPIO_Port, LED2_Pin); //LED翻转
            Delay_Us(500000); //去抖
            HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_RESET); //close

        }
    }
}


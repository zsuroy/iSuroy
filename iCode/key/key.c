#include "key.h"

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
        HAL_Delay(50); //去抖动
        if(HAL_GPIO_ReadPin(KEY_GPIO_Port, KEY_Pin) == GPIO_PIN_SET)
            return 0x01; //开
    }

    if(HAL_GPIO_ReadPin(KEY_GPIO_Port, KEY_Pin) == GPIO_PIN_RESET)
    {
        HAL_Delay(50); //去抖动
        if(HAL_GPIO_ReadPin(KEY_GPIO_Port, KEY_Pin) == GPIO_PIN_RESET)
            return 0x00;
    }
    return 0x00;
}


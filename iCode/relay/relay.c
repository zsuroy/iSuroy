
#include "relay.h"

/**
 * @brief 控制继电器
 * 
 * @param state 1开；0关
 */
void RELAY_Ctr(uint8_t state)
{
    if(state) HAL_GPIO_WritePin(RELAY1_GPIO_Port, RELAY1_Pin, GPIO_PIN_RESET);
    else HAL_GPIO_WritePin(RELAY1_GPIO_Port, RELAY1_Pin, GPIO_PIN_SET);
}

/**
 * @brief RELAY延时反转
 * @todo  增加Relay2
 */
void RELAY_Toggle(void)
{
    HAL_GPIO_TogglePin(RELAY1_GPIO_Port, RELAY1_Pin);
    HAL_Delay(1);
}


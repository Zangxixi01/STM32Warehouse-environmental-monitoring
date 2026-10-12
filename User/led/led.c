
#include "sys.h"
void LED_GPIO_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    RCC_APB2PeriphClockCmd(LED_GPIO_CLK, ENABLE);
    GPIO_InitStruct.GPIO_Pin = LED_GPIO_PIN1;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;

    GPIO_Init(LED_GPIO_PORT, &GPIO_InitStruct);
}

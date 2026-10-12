
#include "sys.h"

void Beep_Init(void)
{
    /* 定义io结构体 */
    GPIO_InitTypeDef GPIO_InitStructure;
    /* 端口模式配置 */
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

    RCC_APB2PeriphClockCmd(BEEP_GPIO_CLK, ENABLE);

    /* 相关端口引脚初始化 */
    GPIO_InitStructure.GPIO_Pin = BEEP_GPIO_PIN;
    GPIO_Init(BEEP_GPIO_PORT, &GPIO_InitStructure);

    /* 默认输出高电平 */
    GPIO_SetBits(BEEP_GPIO_PORT, BEEP_GPIO_PIN);
    BEEP_OFF; // 蜂鸣器关
}

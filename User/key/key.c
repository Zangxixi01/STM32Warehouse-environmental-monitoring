
#include "sys.h"

u8 isKey1 = 0;
u8 isKey2 = 0;
u8 isKey3 = 0;
u8 isKey4 = 0;

//
/*
******按键初始化函数*****************************************
*	功能:	按键初始化函数
*	入口参数:	无
*	返回参数:	无
*	说明:		KEY1-PB12		KEY2-PB13   KEY3-PB14    KEY4-PB15
************************************************************
*/
void KEY_Init(void) // IO初始化
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(KEY1_GPIO_CLK, ENABLE);

    GPIO_InitStructure.GPIO_Pin = KEY1_GPIO_PIN | KEY2_GPIO_PIN | KEY3_GPIO_PIN | KEY4_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; // 设置成上拉输入

    GPIO_Init(KEY1_GPIO_PORT, &GPIO_InitStructure); //
    GPIO_Init(KEY2_GPIO_PORT, &GPIO_InitStructure); //
    GPIO_Init(KEY3_GPIO_PORT, &GPIO_InitStructure); //
    GPIO_Init(KEY4_GPIO_PORT, &GPIO_InitStructure); //

}

void Key1Press(void)
{
    if (isKey1 == 0)
        isKey1 = 1;
}

void Key2Press(void)
{
    if (isKey2 == 0)
        isKey2 = 1;
}

void Key3Press(void)
{
    if (isKey3 == 0)
        isKey3 = 1;
}

void Key4Press(void)
{
    if (isKey4 == 0)
        isKey4 = 1;
}



/*
******按键扫描函数*****************************************
*	功能:	按键扫描函数
*	入口参数:	无
*	返回参数:	无
*	说明:		KEY1-PB12		KEY2-PB13   KEY3-PB14    KEY4-PB15
************************************************************
*/
void KeyScan(void)
{
    static int keyCount = 0;
    static int keyState = 0;
    if (KEY1 == 0 && keyState == 0) // 按键按下
    {
        keyCount++;
        if (keyCount > 2 && KEY1 == 0 && keyState == 0) // 加两次类似延迟10ms，不好解释
        {
            /*ToDo:按键按下执行的操作*/
            Key1Press();
            keyState = 1;
        }
    }
    else if (KEY2 == 0 && keyState == 0)
    {
        keyCount++;
        if (keyCount > 2 && KEY2 == 0 && keyState == 0)
        {
            /*ToDo:按键按下执行的操作*/
            Key2Press();
            keyState = 1;
        }
    }
    else if (KEY3 == 0 && keyState == 0)
    {
        keyCount++;
        if (keyCount > 2 && KEY3 == 0 && keyState == 0)
        {
            /*ToDo:按键按下执行的操作*/
            Key3Press();
            keyState = 1;
        }
    }
    else if (KEY4 == 0 && keyState == 0)
    {
        keyCount++;
        if (keyCount > 2 && KEY4 == 0 && keyState == 0)
        {
            /*ToDo:按键按下执行的操作*/
            Key4Press();
            keyState = 1;
        }
    }

    else if (KEY1 == 1 && KEY2 == 1 && KEY3 == 1 && KEY4 == 1 && keyState == 1) // 当所有按键都处于抬起状态，状态刷新
    {
        keyCount = 0;
        keyState = 0;
    }
}

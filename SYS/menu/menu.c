#include "sys.h"

// 模式切换标志位
uint8_t mode_selection = 0;
// 页面显示的标志位
uint8_t displayFlag = 0;
// 阈值设置标志位
uint8_t thresholdFlag = 0;
// 手动模式标志位
uint8_t manualFlag = 0;

/*****************贯穿整个系统的函数 在主函数只需要调用这一个函数*/

void Mode_selection(void)
{
    // 调用执行设备函数 此函数不能删除
    Action();

    // 按键1 按下模式切换
    if (isKey1) {
        isKey1 =  0;
        mode_selection++;

        if (mode_selection == 2) {
            System.Switch1 = System.Switch2 = System.Switch3 = System.Switch4 = System.Switch5 = 0;//进入手动界面关闭所有开关
        }

        oled_Clear();
    }

    // 标志位限位
    if (mode_selection >= 3) {
        mode_selection = 0;
    }

    // 根据标志位判断进入哪一个模式  手动模式下不调用自动控制函数其余可以调用
    switch (mode_selection) {
    case 0:
        Display();//传感器数据显示
        Automatic();//自动控制
        break;

    case 1:
        Threshold_Settings();//阈值设置
        Automatic();//自动控制
        break;

    case 2:
        Manual();//手动模式
        break;
    }
}

/****
自动控制逻辑判断
****/
uint8_t flag = 0;
void Automatic(void)
{
    // 温度高于设定的上限时，继电器1闭合打开风扇 进行降温
    if (SensorData.TempVal > Threshold.TempThreshold) {
        System.Switch1 = 1;//继电器-降温开关标志位置1
    } else {
        System.Switch1 = 0;//继电器-降温开关标志位置0
    }

    // 湿度低于设定的下限时，继电器2闭合打开加湿片 进行加湿

    if (SensorData.HumiVal < Threshold.HumiThreshold) {
        System.Switch2 = 1;//继电器-加湿开关标志位置1
    } else {
        System.Switch2 = 0;//继电器-加湿开关标志位置0
    }

    // 二氧化碳浓度高于设定的上限时，继电器3闭合打开风扇 进行排风
    if (SensorData.CO2Val > Threshold.CO2Threshold) {
        System.Switch3 = 1;//继电器-排风开关标志位置1
    } else {
        System.Switch3 = 0;//继电器-排风开关标志位置0
    }

    //烟雾高于设定的阈值时，蜂鸣器打开 进行报警
    if (SensorData.SmogVal > Threshold.SmogThreshold) {
        System.Switch4 = 1;//报警标志位为1
    } else {
        System.Switch4 = 0;//报警标志位为0
    }

    // 光照低于设置的阈值时，打开灯光进行补光，高于阈值时
    if (SensorData.LightVal < Threshold.LightThreshold) {
        System.Switch5 = 1;//灯光标志位为1
    } else {
        System.Switch5 = 0;//灯光标志位为0
    }
}

/****
根据执行设备的标志位来开关执行设备
****/
void Action(void)
{
    if (System.Switch1 == 1) { // 降温
        RELAY1_ON;//打开继电器-降温
    } else {
        RELAY1_OFF;//关闭继电器-降温
    }

    if (System.Switch2 == 1) { // 加湿
        RELAY2_ON;//打开继电器-加湿
    } else {
        RELAY2_OFF;//关闭继电器-加湿
    }

    if (System.Switch3 == 1) { // 排风
        RELAY3_ON;//打开继电器-排风
    } else {
        RELAY3_OFF;//关闭继电器-排风
    }

    if (System.Switch4 == 1) { // 报警
        BEEP_ON;//打开蜂鸣器
    } else {
        BEEP_OFF;//关闭蜂鸣器
    }

    if (System.Switch5 == 1) { // 灯光
        LED1_ON;//打开灯光
    } else {
        LED1_OFF;//关闭灯光
    }
}
/****
传感器数据显示
****/

void Display(void)
{
    //显示界面显示翻页
    if (isKey2) {
        isKey2 = 0;
        oled_Clear();
        displayFlag = !displayFlag;//显示界面标志位取反
    }

    if (displayFlag == 0) {
         
        oled_ShowCHinese(16 * 1, 2 * 0, 0);
        oled_ShowCHinese(16 * 2, 2 * 0, 1);
        oled_ShowCHinese(16 * 3, 2 * 0, 2);
        oled_ShowCHinese(16 * 4, 2 * 0, 3);
        oled_ShowCHinese(16 * 5, 2 * 0, 37);
        oled_ShowCHinese(16 * 6, 2 * 0, 38);
        // 温度
        oled_ShowCHinese(16 * 0, 2 * 1, 4);
        oled_ShowCHinese(16 * 1, 2 * 1, 22);
        oled_ShowString(16 * 3, 2 * 1, ":", 16);
        oled_ShowNum(16 * 4, 2 * 1, SensorData.TempVal, 2, 16);//显示温度数据
        oled_ShowCHinese(16 * 5, 2 * 1, 39);
        // 湿度
        oled_ShowCHinese(16 * 0, 2 * 2, 5);
        oled_ShowCHinese(16 * 1, 2 * 2, 22);
        oled_ShowString(16 * 3, 2 * 2, ":", 16);
        oled_ShowNum(16 * 4, 2 * 2, SensorData.HumiVal, 2, 16);//显示湿度数据
        oled_ShowString(16 * 5, 2 * 2, "%RH", 16);
        //烟雾
        oled_ShowCHinese(16 * 0, 2 * 3, 40);
        oled_ShowCHinese(16 * 1, 2 * 3, 41);
        oled_ShowString(16 * 3, 2 * 3, ":", 16);
        oled_ShowNum(16 * 4, 2 * 3, SensorData.SmogVal, 4, 16);//显示烟雾浓度数据
        oled_ShowString(16 * 6, 2 * 3, "PPM", 16);
    }

    if (displayFlag == 1) {
        // 光照
        oled_ShowCHinese(16 * 0, 2 * 0, 6);
        oled_ShowCHinese(16 * 1, 2 * 0, 7);
        oled_ShowString(16 * 2, 2 * 0, ":", 16);
        oled_ShowNum(16 * 3, 2 * 0, SensorData.LightVal, 4, 16);//显示光照强度数据
        oled_ShowString(16 * 6, 2 * 0, "Lux", 16);
        // 二氧化碳
        oled_ShowString(16 * 0, 2 * 1, "CO2", 16);
        oled_ShowString(16 * 2, 2 * 1, ":", 16);
        oled_ShowNum(16 * 3, 2 * 1, SensorData.CO2Val, 4, 16);//显示二氧化碳浓度数据
        oled_ShowString(16 * 6, 2 * 1, "PPM", 16);
    }
}

/****
阈值设置
****/

void Threshold_Settings(void)
{
    // 按键2按下选中设置阈值
    if (isKey2) {
        isKey2 = 0;
        thresholdFlag++;

        // 限位标志   总数-1
        if (thresholdFlag >= 5) {
            thresholdFlag = 0;
        }

        // 翻页时清屏
        if (thresholdFlag == 3 || thresholdFlag == 0) {
            oled_Clear();
        }
    }

    // 调用光标显示函数
    Asterisk(thresholdFlag);

    // 按键3按下阈值加
    if (isKey3) {
        isKey3 = 0;

        switch (thresholdFlag) {
        case 0:
            Threshold.TempThreshold++;//温度上限+1
            break;

        case 1:
            Threshold.HumiThreshold++;//湿度上限+1
            break;

        case 2:
            Threshold.SmogThreshold += 100; //烟雾浓度上限+20
            break;

        case 3:
            Threshold.LightThreshold += 10;//光照强度上限+10
            break;

        case 4:
            Threshold.CO2Threshold += 10; //二氧化碳浓度上限+10
            break;
        }
    }

    // 按键4按下阈值减
    if (isKey4) {
        isKey4 = 0;

        switch (thresholdFlag) {
        case 0:
            Threshold.TempThreshold--;//温度上限-1
            break;

        case 1:
            Threshold.HumiThreshold--;//湿度上限-1
            break;

        case 2:
            Threshold.SmogThreshold -= 100; //烟雾浓度上限-20
            break;

        case 3:
            Threshold.LightThreshold -= 10;//光照强度上限-10
            break;

        case 4:
            Threshold.CO2Threshold -= 10; //二氧化碳浓度上限-10
            break;
        }
    }

    // 按键4按下阈值减

    // 如果标志位小于4 显示第一页
    if (thresholdFlag < 3) {
        // 阈值设置
        oled_ShowCHinese(16 * 2, 2 * 0, 26);
        oled_ShowCHinese(16 * 3, 2 * 0, 27);
        oled_ShowCHinese(16 * 4, 2 * 0, 33);
        oled_ShowCHinese(16 * 5, 2 * 0, 34);
        // 温度阈值
        oled_ShowCHinese(16 * 0, 2 * 1, 4);
        oled_ShowCHinese(16 * 1, 2 * 1, 22);
        oled_ShowCHinese(16 * 2, 2 * 1, 26);
        oled_ShowCHinese(16 * 3, 2 * 1, 27);
        oled_ShowString(16 * 4, 2 * 1, ":", 16);
        oled_ShowNum(16 * 5, 2 * 1, Threshold.TempThreshold, 2, 16);//显示温度上限数据
        // 湿度阈值
        oled_ShowCHinese(16 * 0, 2 * 2, 5);
        oled_ShowCHinese(16 * 1, 2 * 2, 22);
        oled_ShowCHinese(16 * 2, 2 * 2, 26);
        oled_ShowCHinese(16 * 3, 2 * 2, 27);
        oled_ShowString(16 * 4, 2 * 2, ":", 16);
        oled_ShowNum(16 * 5, 2 * 2, Threshold.HumiThreshold, 2, 16);//显示湿度下限数据
        // 烟雾阈值
        oled_ShowCHinese(16 * 0, 2 * 3, 40);
        oled_ShowCHinese(16 * 1, 2 * 3, 41);
        oled_ShowCHinese(16 * 2, 2 * 3, 26);
        oled_ShowCHinese(16 * 3, 2 * 3, 27);
        oled_ShowString(16 * 4, 2 * 3, ":", 16);
        oled_ShowNum(16 * 5, 2 * 3, Threshold.SmogThreshold, 4, 16);//显示烟雾浓度上限数据
    }
    // 如果大于等于3显示第二个页面
    else {
        // 光照阈值
        oled_ShowCHinese(16 * 0, 2 * 0, 6);
        oled_ShowCHinese(16 * 1, 2 * 0, 7);
        oled_ShowCHinese(16 * 2, 2 * 0, 26);
        oled_ShowCHinese(16 * 3, 2 * 0, 27);
        oled_ShowString(16 * 4, 2 * 0, ":", 16);
        oled_ShowNum(16 * 5, 2 * 0, Threshold.LightThreshold, 3, 16);//显示光照下限数据
        // 二氧化碳阈值
        oled_ShowString(16 * 0, 2 * 1, "CO2", 16);
        oled_ShowCHinese(16 * 2, 2 * 1, 26);
        oled_ShowCHinese(16 * 3, 2 * 1, 27);
        oled_ShowString(16 * 4, 2 * 1, ":", 16);
        oled_ShowNum(16 * 5, 2 * 1, Threshold.CO2Threshold, 4, 16);//显示二氧化碳浓度上限数据
    }

    // 阈值限幅
    if (Threshold.TempThreshold >= 99) {
        Threshold.TempThreshold = 99;
    }

    if (Threshold.TempThreshold < 1) {
        Threshold.TempThreshold = 1;
    }

    if (Threshold.HumiThreshold < 1) {
        Threshold.HumiThreshold = 1;
    }

    if (Threshold.HumiThreshold >= 99) {
        Threshold.HumiThreshold = 99;
    }

    if (Threshold.CO2Threshold >= 2500) {
        Threshold.CO2Threshold = 2500;
    }

    if (Threshold.CO2Threshold <  1) {
        Threshold.CO2Threshold = 1;
    }

    if (Threshold.LightThreshold <  1) {
        Threshold.LightThreshold = 1;
    }

    if (Threshold.LightThreshold >= 1000) {
        Threshold.LightThreshold = 1000;
    }

    if (Threshold.SmogThreshold >= 2500) {
        Threshold.SmogThreshold = 2500;
    }

    if (Threshold.SmogThreshold <  1) {
        Threshold.SmogThreshold = 1;
    }
}

/****
手动模式
****/

void Manual(void)
{
    // 按下按键2选中
    if (isKey2) {
        isKey2 = 0;
        manualFlag++;

        // 限位
        if (manualFlag >= 5) {
            manualFlag = 0;
        }

        if (manualFlag == 3 || manualFlag == 0) {
            oled_Clear();
        }
    }

    // 调用光标显示函数
    Asterisk(manualFlag);
    // 按下按键3状态取反

    if (isKey3) {
        isKey3 = 0;

        switch (manualFlag) {
        case 0:
            System.Switch1 = !System.Switch1; //继电器-降温状态取反
            break;

        case 1:
            System.Switch2 = !System.Switch2;//继电器-加湿状态取反
            break;

        case 2:
            System.Switch3 = !System.Switch3;//继电器-排风状态取反
            break;

        case 3:
            System.Switch4 = !System.Switch4;//报警状态取反
            break;

        case 4:
            System.Switch5 = !System.Switch5;//灯光状态取反
            break;
        }
    }

    // 如果标志位小于3 显示第一页
    if (manualFlag < 3) {
        // 手动模式
        oled_ShowCHinese(16 * 2, 2 * 0, 8);
        oled_ShowCHinese(16 * 3, 2 * 0, 9);
        oled_ShowCHinese(16 * 4, 2 * 0, 10);
        oled_ShowCHinese(16 * 5, 2 * 0, 11);
        // 降温
        oled_ShowCHinese(16 * 0, 2 * 1, 12);
        oled_ShowCHinese(16 * 1, 2 * 1, 4);
        oled_ShowString(16 * 2, 2 * 1, ":", 16);

        if (System.Switch1 == 1) {
            oled_ShowCHinese(16 * 3, 2 * 1, 28);
            oled_ShowCHinese(16 * 4, 2 * 1, 29);//开启
        } else {
            oled_ShowCHinese(16 * 3, 2 * 1, 30);
            oled_ShowCHinese(16 * 4, 2 * 1, 31);//关闭
        }

        // 加湿
        oled_ShowCHinese(16 * 0, 2 * 2, 14);
        oled_ShowCHinese(16 * 1, 2 * 2, 15);
        oled_ShowString(16 * 2, 2 * 2, ":", 16);

        if (System.Switch2 == 1) {
            oled_ShowCHinese(16 * 3, 2 * 2, 28);
            oled_ShowCHinese(16 * 4, 2 * 2, 29);//开启
        } else {
            oled_ShowCHinese(16 * 3, 2 * 2, 30);
            oled_ShowCHinese(16 * 4, 2 * 2, 31);//关闭
        }

        // 排风
        oled_ShowCHinese(16 * 0, 2 * 3, 42);
        oled_ShowCHinese(16 * 1, 2 * 3, 43);
        oled_ShowString(16 * 2, 2 * 3, ":", 16);

        if (System.Switch3 == 1) {
            oled_ShowCHinese(16 * 3, 2 * 3, 28);
            oled_ShowCHinese(16 * 4, 2 * 3, 29);//开启
        } else {
            oled_ShowCHinese(16 * 3, 2 * 3, 30);
            oled_ShowCHinese(16 * 4, 2 * 3, 31);//关闭
        }
    }
    // 标志位大于等于4 显示第二页
    else {
        // 报警
        oled_ShowCHinese(16 * 0, 2 * 0, 20);
        oled_ShowCHinese(16 * 1, 2 * 0, 21);
        oled_ShowString(16 * 2, 2 * 0, ":", 16);

        if (System.Switch4 == 1) {
            oled_ShowCHinese(16 * 3, 2 * 0, 28);
            oled_ShowCHinese(16 * 4, 2 * 0, 29);//开启
        } else {
            oled_ShowCHinese(16 * 3, 2 * 0, 30);
            oled_ShowCHinese(16 * 4, 2 * 0, 31);//关闭
        }

        // 灯光
        oled_ShowCHinese(16 * 0, 2 * 1, 18);
        oled_ShowCHinese(16 * 1, 2 * 1, 19);
        oled_ShowString(16 * 2, 2 * 1, ":", 16);

        if (System.Switch5 == 1) {
            oled_ShowCHinese(16 * 3, 2 * 1, 28);
            oled_ShowCHinese(16 * 4, 2 * 1, 29);//开启
        } else {
            oled_ShowCHinese(16 * 3, 2 * 1, 30);
            oled_ShowCHinese(16 * 4, 2 * 1, 31);//关闭
        }
    }
}

// 光标显示函数
void Asterisk(uint8_t A)
{
    if (A == 3 || A == 7) {
        oled_ShowString(16 * 7, 2 * 0, "*", 16);
    } else {
        oled_ShowString(16 * 7, 2 * 0, " ", 16);
    }

    if (A == 0 || A == 4) {
        oled_ShowString(16 * 7, 2 * 1, "*", 16);
    } else {
        oled_ShowString(16 * 7, 2 * 1, " ", 16);
    }

    if (A == 1 || A == 5) {
        oled_ShowString(16 * 7, 2 * 2, "*", 16);
    } else {
        oled_ShowString(16 * 7, 2 * 2, " ", 16);
    }

    if (A == 2 || A == 6) {
        oled_ShowString(16 * 7, 2 * 3, "*", 16);
    } else {
        oled_ShowString(16 * 7, 2 * 3, " ", 16);
    }
}

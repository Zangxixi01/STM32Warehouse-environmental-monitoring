#include "sys.h"

SENSOR SensorData;   // 传感器结构体定义
THRESHOLD Threshold; // 阈值结构体定义
SYSTEM System;       // 系统标志位结构体定义
u32 CO2Data;
u32 sgp30_dat;        //SGP30传感器数据定义
int main(void)
{
    delay_init();        // 延时函数初始化
    NVIC_Config();       // 中断优先级配置
    BdgFlashInit();      // flash初始化包含阈值参数初始化与开关		
    KEY_Init();          // 按键初始化
    RELAY_GPIO_Config(); // 继电器初始化
    Adc_Init();          // adc初始化，读取传感器值
    oled_Init();         // oled初始化
    Beep_Init();         // 蜂鸣器初始化
    LED_GPIO_Config();   // LED灯初始化
    DHT11_Init(); // 温湿度传感器初始化
    SGP30_Init();//二氧化碳传感器初始化
	  SGP30_Write(0x20, 0x08);
		sgp30_dat = SGP30_Read();//读取SGP30的值
		CO2Data = (sgp30_dat & 0xffff0000) >> 16;//移位读取寄存器数据
		TIM2_Init(499, 7199); // 定时器2初始化 定时扫描按键
    oled_Clear();

    /*******************************************/
    while (1) {
        DHT11_Read_TempAndHumidity(&dht11Data); // 获取温湿度;

        if (dht11Data.isvalid) {
            SensorData.TempVal = dht11Data.temp_int;//传递温度数据
            SensorData.HumiVal = dht11Data.humi_int;//传递湿度数据
        }

        SensorData.LightVal = (int)R_to_Lux(Get_Adc_Average(0, 10)); // 获取光照强度;
        //获取烟雾浓度
        SensorData.SmogVal = (float)Get_Adc_Average(1, 10) / 41; //获取烟雾浓度;

        if (SensorData.SmogVal > 10) { //零点校准
            SensorData.SmogVal = SensorData.SmogVal - 10;
        } else {
            SensorData.SmogVal = 0;
        }

        SensorData.SmogVal = SensorData.SmogVal * 100;
        //获取二氧化碳浓度
        SGP30_Write(0x20, 0x08);
        sgp30_dat = SGP30_Read();//读取SGP30的值
        CO2Data = (sgp30_dat & 0xffff0000) >> 16;//移位读取寄存器数据
        SensorData.CO2Val = CO2Data; //传递传感器数据
        Mode_selection();                                           // 模式判定 （按键1选择模式）
        save_Threshold();                                           // 当阈值数据有改变是存入flash函数
    }
}

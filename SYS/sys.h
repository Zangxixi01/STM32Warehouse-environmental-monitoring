#ifndef __SYS_H
#define __SYS_H
// 标准库
#include "math.h"
#include "stdio.h"
#include "string.h"

#include "stm32f10x.h"     //固件包头文件
#include "./delay/delay.h" //内核SysTick

#include "stdarg.h" //keil提供编译器

#include "./time/time2.h"        // 定时器驱动
#include "./adc/adc.h"           //adc驱动
#include "./led/led.h"           //LED驱动
#include "./key/key.h"           //KEY驱动
#include "./beep/bsp_beep.h"     //beep驱动
#include "./dht11/DHT11.h"       //DHT11驱动
#include "./relay/relay.h"       //RELAY驱动
#include "./flash/BridgeFlash.h" //Flash驱动
#include "./illumination/illumination.h"   //光照驱动
#include "./sgp30/sgp30.h"    //二氧化碳驱动

#include "./oled/bsp_oled_iic.h" //oled驱动
#include "./oled/oledFont.h"     //字体库
#include "./menu/menu.h"         //OLED显示

/* 直接操作寄存器的方法控制IO */
#define digitalHi(p, i) \
    {                   \
        p->BSRR = i;    \
    } // 输出为高电平
#define digitalLo(p, i) \
    {                   \
        p->BRR = i;     \
    } // 输出低电平
#define digitalToggle(p, i) \
    {                       \
        p->ODR ^= i;        \
    } // 输出反转状态

#define BITBAND(addr, bitnum) ((addr & 0xF0000000) + 0x2000000 + ((addr & 0xFFFFF) << 5) + (bitnum << 2))
#define MEM_ADDR(addr) *((volatile unsigned long *)(addr))
#define BIT_ADDR(addr, bitnum) MEM_ADDR(BITBAND(addr, bitnum))
// IO口地址映射
#define GPIOA_ODR_Addr (GPIOA_BASE + 12) // 0x4001080C
#define GPIOB_ODR_Addr (GPIOB_BASE + 12) // 0x40010C0C
#define GPIOC_ODR_Addr (GPIOC_BASE + 12) // 0x4001100C

#define GPIOA_IDR_Addr (GPIOA_BASE + 8) // 0x40010808
#define GPIOB_IDR_Addr (GPIOB_BASE + 8) // 0x40010C08
#define GPIOC_IDR_Addr (GPIOC_BASE + 8) // 0x40011008

// IO口操作,只对单一的IO口!

#define PAout(n) BIT_ADDR(GPIOA_ODR_Addr, n) // 输出
#define PAin(n) BIT_ADDR(GPIOA_IDR_Addr, n)  // 输入

#define PBout(n) BIT_ADDR(GPIOB_ODR_Addr, n) // 输出
#define PBin(n) BIT_ADDR(GPIOB_IDR_Addr, n)  // 输入

#define PCout(n) BIT_ADDR(GPIOC_ODR_Addr, n) // 输出
#define PCin(n) BIT_ADDR(GPIOC_IDR_Addr, n)  // 输入

// 传感器数据结构体
typedef struct {
    uint8_t TempVal;  // 温度值
    uint8_t HumiVal;  // 湿度
    uint16_t LightVal; // 光照强度
    float  SmogVal; // 烟雾浓度
    uint16_t CO2Val; // 二氧化碳浓度
} SENSOR;

// 阈值数据结构体
typedef struct {
    uint8_t TempThreshold; // 温度阈值
    uint8_t HumiThreshold; // 湿度阈值
    uint16_t LightThreshold; // 光照阈值
    uint16_t  SmogThreshold; // 烟雾阈值
    uint16_t CO2Threshold; // 二氧化碳阈值
} THRESHOLD;

// 系统标志位结构体
typedef struct {
    uint8_t Switch1; // 降温
    uint8_t Switch2; // 排风
    uint8_t Switch3; // 加湿
    uint8_t Switch4; // 报警
    uint8_t Switch5; // 灯光
} SYSTEM;

extern THRESHOLD Threshold; // 定义阈值结构体
extern SENSOR SensorData;   // 定义传感器结构体
extern SYSTEM System;       // 定义系统标志位结构体
void NVIC_Config(void);

#endif

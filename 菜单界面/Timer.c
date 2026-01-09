#include <STC89C5xRC.H>
#include "Timer.h"

// 全局变量
unsigned int Timer_Count = 0;      // 计时器秒计数
unsigned char Clock_Hour = 12;     // 时钟小时
unsigned char Clock_Minute = 0;    // 时钟分钟
unsigned char Clock_Second = 0;    // 时钟秒钟

// 计时器专用变量
static unsigned int timer_ms_count = 0;  // 计时器毫秒计数
static unsigned char timer_running = 0;  // 计时器运行状态：0=停止，1=运行

// 时钟专用变量  
static unsigned int clock_ms_count = 0;  // 时钟毫秒计数

/**
  * @brief  定时器0初始化函数
  */
void Timer0_Init(void)
{
    // 设置定时器0为16位定时器模式
    TMOD &= 0xF0;   // 清零定时器0的模式位
    TMOD |= 0x01;   // 设置定时器0为模式1（16位定时器）
    
    // 设置定时器初值，50ms定时
    TL0 = 0x00;     // 定时器低8位
    TH0 = 0x4C;     // 定时器高8位，50ms @11.0592MHz
    
    ET0 = 1;        // 使能定时器0中断
    EA = 1;         // 开启总中断
    TR0 = 1;        // 启动定时器0
    
    // 初始化变量
    timer_ms_count = 0;
    clock_ms_count = 0;
    timer_running = 0;  // 初始状态：计时器停止
}

/**
  * @brief  定时器0中断服务函数
  */
void Timer0_Routine() interrupt 1
{
    // 重装定时器初值
    TL0 = 0x00;
    TH0 = 0x4C;
    
    // 计时器毫秒计数（只在计时器运行时计数）
    if(timer_running)
    {
        timer_ms_count++;
    }
    
    // 时钟毫秒计数（始终计数）
    clock_ms_count++;
    
    // 计时器功能：每20次中断（1秒）计数一次
    if(timer_running && timer_ms_count >= 20)  // 20 * 50ms = 1000ms = 1秒
    {
        timer_ms_count = 0;   // 重置毫秒计数
        Timer_Count++;        // 秒计数加1
    }
    
    // 时钟功能：每20次中断（1秒）更新一次时间
    if(clock_ms_count >= 20)  // 1秒
    {
        clock_ms_count = 0;   // 重置毫秒计数
        Clock_Second++;       // 秒钟加1
        
        // 秒钟进位处理
        if(Clock_Second >= 60)
        {
            Clock_Second = 0;     // 秒钟归零
            Clock_Minute++;       // 分钟加1
            
            // 分钟进位处理
            if(Clock_Minute >= 60)
            {
                Clock_Minute = 0; // 分钟归零
                Clock_Hour++;     // 小时加1
                
                // 小时进位处理
                if(Clock_Hour >= 24)
                {
                    Clock_Hour = 0; // 小时归零（24小时制）
                }
            }
        }
    }
}

/**
  * @brief  启动计时器计数
  */
void Timer_StartCount(void)
{
    timer_running = 1;  // 设置计时器运行标志
}

/**
  * @brief  停止计时器计数
  */
void Timer_StopCount(void)
{
    timer_running = 0;  // 清除计时器运行标志
}

/**
  * @brief  重置计时器
  */
void Timer_ResetCount(void)
{
    Timer_Count = 0;      // 重置秒计数
    timer_ms_count = 0;   // 重置毫秒计数
    timer_running = 0;    // 停止计时器
}

/**
  * @brief  获取计时器运行状态
  * @retval 1=运行，0=停止
  */
unsigned char Timer_IsRunning(void)
{
    return timer_running;
}
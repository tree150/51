#ifndef __TIMER_H__
#define __TIMER_H__

// 定时器功能函数声明
void Timer0_Init(void);                    // 初始化定时器0
void Timer_StartCount(void);               // 启动计时
void Timer_StopCount(void);                // 停止计时
void Timer_ResetCount(void);               // 重置计时器
unsigned char Timer_IsRunning(void);       // 获取计时器运行状态

// 全局变量声明
extern unsigned int Timer_Count;           // 计时器计数值（秒）
extern unsigned char Clock_Hour;           // 时钟小时
extern unsigned char Clock_Minute;         // 时钟分钟
extern unsigned char Clock_Second;         // 时钟秒钟

#endif
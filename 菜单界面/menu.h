#ifndef __MENU_H__
#define __MENU_H__

// 菜单函数声明
int menu(void);                    // 主菜单函数
void LED_Func(void);               // LED功能菜单
void UART_Func(void);              // 串口功能菜单
void Timer_Func(void);             // 定时器功能菜单

// LED功能子函数
void LED_Switch(void);             // LED开关控制
void PWM_Control(void);            // PWM调光控制

// 串口功能子函数
void UART_SendMenu(void);          // 串口发送菜单
void UART_ReceiveMenu(void);       // 串口接收菜单
void UART_AutoTest(void);          // 串口自动测试
void UART_DisplayData(void);       // 显示接收数据

// 定时器功能子函数
void Timer_Counter(void);          // 计时器功能
void Clock_System(void);           // 时钟系统功能

// 全局变量声明
extern unsigned char current_selection;  // 当前选择的菜单项

#endif
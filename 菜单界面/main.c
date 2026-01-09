#include <STC89C5xRC.H>
#include "LCD1602.h"
#include "Delay.h"
#include "Key.h"
#include "menu.h"
#include "PWM.h"
#include "Timer.h"
#include "UART.h"

// 引脚定义
sbit LED = P2^0;

void System_Init(void)
{
    // 系统初始化
    LCD_Init();
    PWM_Init();
    Timer0_Init();
    UART_Init();  // 串口初始化
    
    // 关闭所有外设
    LED = 1;
    PWM_SetDuty(0);
    
    Delay(100);
}

void main()
{
    int mode;
    
    // 系统初始化
    System_Init();
    
    while(1)
    {
        mode = menu();
        
        switch(mode)
        {
            case 1: LED_Func(); break;
            case 2: UART_Func(); break;
            case 3: Timer_Func(); break;
        }
        
        // 更新PWM
        PWM_Update();
    }
}
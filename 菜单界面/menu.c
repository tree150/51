#include <STC89C5xRC.H>
#include "LCD1602.h"
#include "Delay.h"
#include "Key.h"
#include "PWM.h"
#include "Timer.h"
#include "UART.h"
#include "menu.h"

// 引脚定义
sbit LED = P2^0;  // LED控制引脚

// 全局变量
unsigned char current_selection = 1;  // 当前选中的菜单项，初始为1

// ============ 主菜单函数 ============
int menu(void)
{
    unsigned char key = 0;  // 存储按键值
    
    LCD_Init();             // 初始化LCD显示
    current_selection = 1;  // 重置当前选择为第一项
    
    while(1)
    {
        // 根据当前选择显示不同的菜单项
        switch(current_selection)
        {
            case 1: // LED选项
                LCD_ShowString(1,1,"*1.LED         ");      // 第一行显示，*表示选中
                LCD_ShowString(2,1," 2.UART 3.Timer");      // 第二行显示其他选项
                break;
            case 2: // UART选项
                LCD_ShowString(1,1," 1.LED         ");
                LCD_ShowString(2,1,"*2.UART 3.Timer");
                break;
            case 3: // Timer选项
                LCD_ShowString(1,1," 1.LED         ");
                LCD_ShowString(2,1," 2.UART*3.Timer");
                break;
        }
        
        key = Key();  // 获取按键值
        
        if(key == 1) // 确认键被按下
        {
            return current_selection;  // 返回当前选择的功能编号
        }
        else if(key == 2) // 下翻键被按下
        {
            current_selection++;       // 切换到下一个选项
            if(current_selection > 3)  // 如果超过最大值
                current_selection = 1; // 回到第一个选项
            Delay(200);                // 延时防抖
        }
        
        Delay(50);  // 短延时，降低CPU占用
    }
}

// ============ LED功能菜单 ============
void LED_Func(void)
{
    unsigned char key = 0;
    unsigned char sub_selection = 1;  // 子菜单选择
    
    LCD_Init();  // 清屏并初始化显示
    
    while(1)
    {
        // 显示LED功能子菜单
        switch(sub_selection)
        {
            case 1: // LED开关控制
                LCD_ShowString(1,1,"LED Switch     ");
                LCD_ShowString(2,1,"*ON  OFF Back  ");
                break;
            case 2: // PWM调光控制
                LCD_ShowString(1,1,"PWM Control    ");
                LCD_ShowString(2,1,"*Adjust Back   ");
                break;
            case 3: // 返回主菜单
                LCD_ShowString(1,1,"Back to Main   ");
                LCD_ShowString(2,1,"*Yes           ");
                break;
        }
        
        key = Key();  // 获取按键
        
        if(key == 1) // 确认键
        {
            // 根据子菜单选择执行相应功能
            switch(sub_selection)
            {
                case 1: LED_Switch(); break;   // LED开关控制
                case 2: PWM_Control(); break;  // PWM调光控制
                case 3: return;                // 返回主菜单
            }
            LCD_Init();  // 功能执行完后重新初始化显示
        }
        else if(key == 2) // 下翻键
        {
            sub_selection++;           // 切换到下一个子菜单项
            if(sub_selection > 3)      // 循环选择
                sub_selection = 1;
            Delay(200);
        }
        else if(key == 3) // 返回键
        {
            return;  // 直接返回主菜单
        }
        
        Delay(50);
    }
}

// LED开关控制功能
void LED_Switch(void)
{
    unsigned char key = 0;
    unsigned char led_on = 0;  // LED状态标志，0=关，1=开
    
    LCD_Init();
    LED = 1;  // 初始状态：LED关闭
    
    while(1)
    {
        // 根据LED状态显示相应信息
        if(led_on)
        {
            LCD_ShowString(1,1,"LED: ON        ");
            LED = 0;  // 点亮LED
        }
        else
        {
            LCD_ShowString(1,1,"LED: OFF       ");
            LED = 1;  // 关闭LED
        }
        
        LCD_ShowString(2,1,"OK:Toggle  Back");  // 操作提示
        
        key = Key();
        
        if(key == 1) // 确认键 - 切换LED状态
        {
            led_on = !led_on;  // 切换开关状态
            Delay(200);
        }
        else if(key == 3) // 返回键
        {
            LED = 1;  // 退出时确保LED关闭
            return;
        }
        
        Delay(50);
    }
}

// PWM调光控制功能
void PWM_Control(void)
{
    unsigned char key = 0;
    unsigned char brightness = 0;  // 亮度值，0-100
    
    LCD_Init();
    
    while(1)
    {
        // 显示PWM控制界面
        LCD_ShowString(1,1,"PWM Control    ");
        LCD_ShowNum(2,1,brightness,3);  // 显示当前亮度百分比
        LCD_ShowString(2,4,"%   + - Back");
        
        // 简单的PWM实现：根据亮度值控制LED
        if(brightness > 0)
            LED = 0;  // 亮度>0时点亮LED
        else
            LED = 1;  // 亮度=0时关闭LED
        
        key = Key();
        
        if(key == 1) // 确认键 - 增加亮度
        {
            if(brightness < 100)
                brightness += 10;  // 每次增加10%
            Delay(150);
        }
        else if(key == 2) // 下翻键 - 减少亮度
        {
            if(brightness > 0)
                brightness -= 10;  // 每次减少10%
            Delay(150);
        }
        else if(key == 3) // 返回键
        {
            LED = 1;  // 退出时关闭LED
            return;
        }
        
        Delay(100);
    }
}

// ============ UART功能菜单 ============
void UART_Func(void)
{
    unsigned char key = 0;
    unsigned char sub_selection = 1;
    
    LCD_Init();
    UART_Init();  // 初始化串口
    
    while(1)
    {
        // 显示UART功能子菜单
        switch(sub_selection)
        {
            case 1: // 发送模式
                LCD_ShowString(1,1,"UART Send      ");
                LCD_ShowString(2,1,"*Send Msg      ");
                break;
            case 2: // 接收模式
                LCD_ShowString(1,1,"UART Receive   ");
                LCD_ShowString(2,1,"*Show Data     ");
                break;
            case 3: // 返回主菜单
                LCD_ShowString(1,1,"Back to Main   ");
                LCD_ShowString(2,1,"*Yes           ");
                break;
        }
        
        key = Key();
        
        if(key == 1) // 确认键
        {
            switch(sub_selection)
            {
                case 1: UART_SendMenu(); break;     // 串口发送菜单
                case 2: UART_ReceiveMenu(); break;  // 串口接收菜单
                case 3: return;                     // 返回主菜单
            }
            LCD_Init();
        }
        else if(key == 2) // 下翻键
        {
            sub_selection++;
            if(sub_selection > 3) 
                sub_selection = 1;
            Delay(150);
        }
        else if(key == 3) // 返回键
        {
            return;
        }
        
        Delay(50);
    }
}

// UART发送菜单 - 针对ISP串口助手优化
void UART_SendMenu(void)
{
    unsigned char key = 0;
    unsigned char msg_selection = 1;
    
    LCD_Init();
    
    while(1)
    {
        LCD_ShowString(1,1,"Send to ISP:    ");
        
        switch(msg_selection)
        {
            case 1:
                LCD_ShowString(2,1,">Hello World   ");
                break;
            case 2:
                LCD_ShowString(2,1,">MCU Info      ");
                break;
            case 3:
                LCD_ShowString(2,1,">Test Data     ");
                break;
            case 4:
                LCD_ShowString(2,1,">Counter       ");
                break;
            case 5:
                LCD_ShowString(2,1,">Back          ");
                break;
        }
        
        key = Key();
        
        if(key == 1) // 确认键 - 发送选中的消息
        {
            switch(msg_selection)
            {
                case 1:
                    UART_SendString("Hello from STC89C52!");
                    LCD_ShowString(1,1,"Sent: Hello    ");
                    Delay(800);
                    break;
                case 2:
                    UART_SendString("MCU: STC89C52RC");
                    UART_SendString("Freq: 11.0592MHz");
                    UART_SendString("UART: 9600 8N1");
                    LCD_ShowString(1,1,"Sent: MCU Info ");
                    Delay(800);
                    break;
                case 3:
                    UART_SendString("Test Message 123");
                    UART_SendString("ABCDEFGHIJKLMNOP");
                    LCD_ShowString(1,1,"Sent: Test Data");
                    Delay(800);
                    break;
								case 4:
								{
										static unsigned char counter = 0;  // 正确：在代码块中声明
										UART_SendString("Counter: ");
										UART_SendNumber(counter++);
										LCD_ShowString(1,1,"Sent: Counter  ");
										Delay(800);
								}
								break;
                case 5:
                return;
            }
        }
        else if(key == 2) // 下翻键 - 切换消息
        {
            msg_selection++;
            if(msg_selection > 5) 
                msg_selection = 1;
            Delay(150);
        }
        else if(key == 3) // 返回键
        {
            return;
        }
        
        Delay(50);
    }
}

// UART接收菜单 - 针对ISP串口助手优化
void UART_ReceiveMenu(void)
{
    unsigned char key = 0;
    unsigned char display_mode = 0; // 0:等待, 1:显示数据
    
    LCD_Init();
    UART_ClearBuffer();
    
    while(1)
    {
        switch(display_mode)
        {
            case 0: // 等待模式
                LCD_ShowString(1,1,"UART Receive   ");
                LCD_ShowString(2,1,"OK:Start Back  ");
                
                // 检查是否有数据
                if(UART_HasCompleteLine())
                {
                    LCD_ShowString(1,1,"Data Ready!    ");
                }
                break;
                
            case 1: // 显示模式
                UART_DisplayData();
                LCD_ShowString(2,1,"OK:Clear Back  ");
                break;
        }
        
        key = Key();
        
        if(key == 1) // 确认键
        {
            if(display_mode == 0)
            {
                display_mode = 1; // 进入显示模式
            }
            else if(display_mode == 1)
            {
                UART_ClearBuffer();
                LCD_ShowString(1,1,"Buffer Cleared! ");
                LCD_ShowString(2,1,"                ");
                Delay(800);
                display_mode = 0;
                LCD_Init();
            }
        }
        else if(key == 3) // 返回键
        {
            return;
        }
        
        // 自动检测新数据
        if(UART_HasCompleteLine() && display_mode == 1)
        {
            LCD_Init();
            LCD_ShowString(1,1,"New Data!       ");
            Delay(500);
            LCD_Init();
        }
        
        Delay(100);
    }
}

// 显示接收数据 - 针对ISP串口助手优化
void UART_DisplayData(void)
{
    unsigned char i;
    
    if(UART_HasCompleteLine())
    {
        char* buffer = UART_GetBuffer();
        
        LCD_ShowString(1,1,"Received:       ");
        
        // 显示第一行（最多16字符）
        for(i = 0; i < 16 && buffer[i] != '\0'; i++)
        {
            if(buffer[i] >= 32 && buffer[i] <= 126) // 可显示字符
            {
                LCD_ShowChar(1, 10 + i, buffer[i]);
            }
            else
            {
                LCD_ShowChar(1, 10 + i, '.'); // 非显示字符用点代替
            }
        }
        
        // 显示第二行（如果有更多数据）
        if(buffer[16] != '\0')
        {
            for(i = 16; i < 32 && buffer[i] != '\0'; i++)
            {
                if(buffer[i] >= 32 && buffer[i] <= 126)
                {
                    LCD_ShowChar(2, i-15, buffer[i]);
                }
                else
                {
                    LCD_ShowChar(2, i-15, '.');
                }
            }
        }
        else
        {
            LCD_ShowString(2,1,"                ");
        }
    }
    else
    {
        // 显示等待信息
        static unsigned char wait_count = 0;
        wait_count++;
        
        LCD_ShowString(1,1,"Waiting Data... ");
        
        if(wait_count < 20)
            LCD_ShowString(2,1,"[#             ]");
        else if(wait_count < 40)
            LCD_ShowString(2,1,"[##            ]");
        else if(wait_count < 60)
            LCD_ShowString(2,1,"[###           ]");
        else
            wait_count = 0;
    }
}

// ============ Timer功能菜单 ============
void Timer_Func(void)
{
    unsigned char key = 0;
    unsigned char sub_selection = 1;
    
    LCD_Init();
    Timer0_Init();  // 初始化定时器0
    
    while(1)
    {
        // 显示定时器功能子菜单
        switch(sub_selection)
        {
            case 1: // 计时器
                LCD_ShowString(1,1,"Timer Counter  ");
                LCD_ShowString(2,1,"*Start/Stop    ");
                break;
            case 2: // 时钟系统
                LCD_ShowString(1,1,"Clock System   ");
                LCD_ShowString(2,1,"*Show Time     ");
                break;
            case 3: // 返回主菜单
                LCD_ShowString(1,1,"Back to Main   ");
                LCD_ShowString(2,1,"*Yes           ");
                break;
        }
        
        key = Key();
        
        if(key == 1) // 确认键
        {
            switch(sub_selection)
            {
                case 1: Timer_Counter(); break;  // 计时器功能
                case 2: Clock_System(); break;   // 时钟系统功能
                case 3: return;                  // 返回主菜单
            }
            LCD_Init();
        }
        else if(key == 2) // 下翻键
        {
            sub_selection++;
            if(sub_selection > 3) 
                sub_selection = 1;
            Delay(200);
        }
        else if(key == 3) // 返回键
        {
            return;
        }
        
        Delay(50);
    }
}

// 计时器功能
void Timer_Counter(void)
{
    unsigned char key = 0;
    unsigned int seconds = 0;          // 当前秒数
    unsigned int last_seconds = 0;     // 上次显示的秒数
    unsigned char running = 0;         // 运行状态
    
    LCD_Init();
    
    // 获取当前计时器状态
    running = Timer_IsRunning();
    seconds = Timer_Count;
    
    while(1)
    {
        seconds = Timer_Count;  // 获取当前计时值
        
        // 只有当秒数变化时才更新显示，避免闪烁
        if(seconds != last_seconds)
        {
            // 显示计时器界面
            LCD_ShowString(1,1,"Timer:         ");
            LCD_ShowNum(1,7,seconds/60,2);   // 显示分钟
            LCD_ShowChar(1,9,':');
            LCD_ShowNum(1,10,seconds%60,2);  // 显示秒钟
            
            // 显示运行状态
            if(running)
            {
                LCD_ShowString(1,13,"Run");  // 运行中
            }
            else
            {
                LCD_ShowString(1,13,"Stop"); // 已停止
            }
            
            last_seconds = seconds;
        }
        
        // 显示控制选项
        if(running)
        {
            LCD_ShowString(2,1,"Stop  Reset Back");
        }
        else
        {
            LCD_ShowString(2,1,"Start Reset Back");
        }
        
        key = Key();
        
        if(key == 1) // 确认键 - 开始/停止计时
        {
            if(running)
            {
                Timer_StopCount();     // 停止计时
                running = 0;
            }
            else
            {
                Timer_StartCount();    // 开始计时
                running = 1;
            }
            Delay(200);
        }
        else if(key == 2) // 下翻键 - 重置计时器
        {
            Timer_ResetCount();        // 重置计时器
            seconds = 0;
            running = 0;
            last_seconds = 0;          // 强制更新显示
            Delay(200);
        }
        else if(key == 3) // 返回键
        {
            // 注意：退出时不停止计时器，保持当前状态
            // 下次进入时可以继续计时
            return;
        }
        
        Delay(100);
    }
}

// 时钟系统功能
void Clock_System(void)
{
    unsigned char key = 0;
    unsigned char last_second = 255;  // 记录上次显示的秒数
    
    LCD_Init();
    Timer_StartCount();  // 启动时钟计时
    
    while(1)
    {
        // 只有当秒数变化时才更新显示，避免闪烁
        if(Clock_Second != last_second)
        {
            // 显示当前时间（时:分:秒）
            LCD_ShowString(1,1,"Time: ");
            LCD_ShowNum(1,7,Clock_Hour,2);     // 小时
            LCD_ShowChar(1,9,':');
            LCD_ShowNum(1,10,Clock_Minute,2);  // 分钟
            LCD_ShowChar(1,12,':');
            LCD_ShowNum(1,13,Clock_Second,2);  // 秒钟
            last_second = Clock_Second;
        }
        
        LCD_ShowString(2,1,"Back to Menu   ");  // 操作提示
        
        key = Key();
        
        if(key == 3) // 返回键
        {
            Timer_StopCount();  // 停止时钟计时
            return;
        }
        
        Delay(100);
    }
}
#include <STC89C5xRC.H>
#include "UART.h"
#include "Delay.h"

// 全局变量定义
char uart_receive_buffer[64] = {0};    // 增大缓冲区
unsigned char uart_buffer_index = 0;
bit uart_data_ready = 0;

/**
  * @brief  串口初始化 - 针对ISP串口助手优化
  */
void UART_Init(void)
{
    // 设置定时器1为8位自动重装模式
    TMOD &= 0x0F;
    TMOD |= 0x20;
    
    // 设置波特率为9600 @11.0592MHz
    TH1 = 0xFD;
    TL1 = 0xFD;
    
    // 设置串口模式1，允许接收
    SCON = 0x50;
    
    // 波特率不加倍
    PCON &= 0x7F;
    
    // 启动定时器1
    TR1 = 1;
    
    // 开启串口中断和总中断
    ES = 1;
    EA = 1;
    
    // 初始化接收缓冲区
    UART_ClearBuffer();
}

/**
  * @brief  发送一个字节
  */
void UART_SendByte(unsigned char byte)
{
    SBUF = byte;
    while(TI == 0);  // 等待发送完成
    TI = 0;          // 清除发送标志
}

/**
  * @brief  发送字符串 - 针对ISP串口助手优化
  */
void UART_SendString(char *str)
{
    while(*str != '\0')
    {
        UART_SendByte(*str);
        str++;
    }
    // ISP串口助手需要换行才能正确显示
    UART_SendByte('\r');
    UART_SendByte('\n');
}

/**
  * @brief  发送数字
  */
void UART_SendNumber(unsigned int num)
{
    unsigned char digits[5];
    unsigned char i = 0;
    
    if(num == 0)
    {
        UART_SendByte('0');
        return;
    }
    
    // 提取各位数字
    while(num > 0)
    {
        digits[i++] = '0' + (num % 10);
        num /= 10;
    }
    
    // 逆序发送
    while(i > 0)
    {
        UART_SendByte(digits[--i]);
    }
}

/**
  * @brief  接收一个字节（查询方式）
  */
unsigned char UART_ReceiveByte(void)
{
    if(RI == 1)
    {
        RI = 0;
        return SBUF;
    }
    return 0;
}

/**
  * @brief  检查是否有数据可读
  */
bit UART_DataAvailable(void)
{
    return (RI == 1);
}

/**
  * @brief  检查是否有完整的一行数据
  */
bit UART_HasCompleteLine(void)
{
    return uart_data_ready;
}

/**
  * @brief  清空接收缓冲区
  */
void UART_ClearBuffer(void)
{
    unsigned char i;
    for(i = 0; i < 64; i++)
    {
        uart_receive_buffer[i] = 0;
    }
    uart_buffer_index = 0;
    uart_data_ready = 0;
}

/**
  * @brief  获取接收缓冲区指针
  */
char* UART_GetBuffer(void)
{
    return uart_receive_buffer;
}

/**
  * @brief  串口中断服务函数 - 针对ISP串口助手优化
  */
void UART_Interrupt() interrupt 4
{
    unsigned char received_char;  // 变量声明必须放在函数开头
    
    if(RI == 1)  // 接收中断
    {
        RI = 0;
        received_char = SBUF;  // 赋值操作
        
        // 存储接收到的字符
        if(uart_buffer_index < 63)  // 保留一个位置给结束符
        {
            uart_receive_buffer[uart_buffer_index] = received_char;
            uart_buffer_index++;
            
            // 检测字符串结束符（ISP串口助手通常以回车结束）
            if(received_char == '\r' || received_char == '\n')
            {
                uart_receive_buffer[uart_buffer_index-1] = '\0'; // 替换结束符
                uart_data_ready = 1;
                uart_buffer_index = 0;
            }
        }
        else
        {
            // 缓冲区满，强制结束
            uart_receive_buffer[63] = '\0';
            uart_data_ready = 1;
            uart_buffer_index = 0;
        }
    }
    
    if(TI == 1)  // 发送中断
    {
        TI = 0;
    }
}
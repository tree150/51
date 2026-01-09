#ifndef __UART_H__
#define __UART_H__

// 串口功能函数声明
void UART_Init(void);                    // 初始化串口
void UART_SendString(char *str);         // 发送字符串
void UART_SendByte(unsigned char byte);  // 发送单个字节
unsigned char UART_ReceiveByte(void);    // 接收单个字节（查询方式）
void UART_ClearBuffer(void);             // 清空接收缓冲区
void UART_SendNumber(unsigned int num);  // 发送数字
bit UART_DataAvailable(void);            // 检查是否有数据可读
bit UART_HasCompleteLine(void);          // 检查是否有完整的一行数据
char* UART_GetBuffer(void);              // 获取接收缓冲区指针

// 全局变量声明
extern char uart_receive_buffer[64];     // 接收缓冲区（增大）
extern unsigned char uart_buffer_index;  // 缓冲区索引
extern bit uart_data_ready;              // 数据就绪标志

#endif
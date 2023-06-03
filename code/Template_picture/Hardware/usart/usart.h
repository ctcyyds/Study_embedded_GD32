#ifndef _USART_H
#define _USART_H

#include "gd32f4xx.h"
#include "systick.h"
#include <stdio.h>
#include "string.h"

//RX和TX引脚时钟
#define USART_TX_RX_RCU          RCU_GPIOA
//USART0时钟
#define USART_RCU             RCU_USART0

//引脚类型
#define USART_GPIO  	GPIOA
//引脚编号
#define USART_RX_PIN GPIO_PIN_10
#define USART_TX_PIN GPIO_PIN_9

//USART编号
#define USART_NUM USART0

/* 串口缓冲区的数据长度 */
#define USART_RECEIVE_LENGTH  4096

//USART关于GPIO配置
void usart_gpio_config(void);

//USART关于参数配置
void usart_param_config(uint32_t baudval);

//包含gpio和usart参数配置
void usart_all_config(uint32_t baudval);

//usart功能运行
void usart_run(void);

//发送数据
void send_data(uint8_t data);

//发送字符串
void send_string(char *data);

#endif 
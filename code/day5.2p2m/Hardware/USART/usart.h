#ifndef __USART_H__
#define __USART_H__
#include "gd32f4xx.h"
#include "systick.h"
#include <stdio.h>
#include "string.h"

//定义TX、RX
#define BSP_USART_TX_RX_RCU          RCU_GPIOA
//USART0时钟
#define BSP_USART_RCU             RCU_USART0
//引脚类型
#define USART_GPIO GPIOA
//引脚编号
#define USART_RX_PIN GPIO_PIN_10
#define USART_TX_PIN GPIO_PIN_9

//USRAT编号
#define USART_NUM USART0

/* 串口缓冲区的数据长度 */
#define USART_RECEIVE_LENGTH  4096

//USART关于GPIO的配置
void usart_gpio_config(void);

//USART关于参数的配置
void usart_parameter_config(uint32_t baudval);

//包含gpio和usart参数配置
void usart_all_config(uint32_t baudval);

//USART运行
void usart_run(void);

//发送数据
void send_data(uint8_t data);

//发送字符串
void send_string(char *data);


#endif
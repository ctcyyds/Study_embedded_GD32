#ifndef __USART_H__
#define __USART_H__
#include "gd32f4xx.h"
#include "systick.h"
#include <stdio.h>

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

void usart_gpio_config(void);
void usart_parameter_config(uint32_t baudval);
void usart_run(void);
void send_data(uint8_t data);

#endif
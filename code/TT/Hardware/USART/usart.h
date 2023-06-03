#ifndef __USART_H__
#define __USART_H__
#include "gd32f4xx.h"
#include "systick.h"

//定义TX、RX
#define BSP_USART_TX_RCU          RCU_GPIOA
#define BSP_USART_RX_RCU          RCU_GPIOA
//USART0时钟
#define BSP_USART_RCU             RCU_USART0
//引脚类型
#define USRAT_GPIO GPIOA
//引脚编号
#define USRAT_RX_PIN GPIO_PIN_10
#define USRAT_TX_PIN GPIO_PIN_9

//USRAT编号


void usart_gpio_config(void);
void usart_parameter_config(uint32_t baudval);
void usart_run(void);
void send_data(uint8_t data);

#endif
#ifndef __DMA_H_
#define __DMA_H_

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

//USART0的DMA时钟
#define BSP_DMA_RCU RCU_DMA1
#define BSP_DMA DMA1
#define BSP_DMA_CHANNEL_RX DMA_CH2
#define BSP_DMA_CHANNEL_TX DMA_CH7

/* 串口缓冲区的数据长度 */
#define USART_RECEIVE_LENGTH  4096

//USART关于GPIO配置
void usart_gpio_config(void);

//USART关于参数配置
void usart_param_config(uint32_t baudval);
//DMA发送配置
int32_t usart_dma_tx_init(uint32_t periph_addr);

//DMA接收配置
int32_t usart_dma_rx_init(uint32_t periph_addr);

//包含gpio和usart参数配置
void usart_dma_config(uint32_t baudval);
//通过DMA发送
int32_t uart_dma_send(uint8_t *data,uint32_t len);

#endif
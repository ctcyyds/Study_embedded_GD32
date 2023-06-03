#ifndef __DMA_H_
#define __DMA_H_

#include "gd32f4xx.h"
#include "systick.h"
#include <stdio.h>
#include "string.h"

//RX��TX����ʱ��
#define USART_TX_RX_RCU          RCU_GPIOA
//USART0ʱ��
#define USART_RCU             RCU_USART0

//��������
#define USART_GPIO  	GPIOA
//���ű��
#define USART_RX_PIN GPIO_PIN_10
#define USART_TX_PIN GPIO_PIN_9

//USART���
#define USART_NUM USART0

//USART0��DMAʱ��
#define BSP_DMA_RCU RCU_DMA1
#define BSP_DMA DMA1
#define BSP_DMA_CHANNEL_RX DMA_CH2
#define BSP_DMA_CHANNEL_TX DMA_CH7

/* ���ڻ����������ݳ��� */
#define USART_RECEIVE_LENGTH  4096

//USART����GPIO����
void usart_gpio_config(void);

//USART���ڲ�������
void usart_param_config(uint32_t baudval);
//DMA��������
int32_t usart_dma_tx_init(uint32_t periph_addr);

//DMA��������
int32_t usart_dma_rx_init(uint32_t periph_addr);

//����gpio��usart��������
void usart_dma_config(uint32_t baudval);
//ͨ��DMA����
int32_t uart_dma_send(uint8_t *data,uint32_t len);

#endif
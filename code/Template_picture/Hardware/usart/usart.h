#ifndef _USART_H
#define _USART_H

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

/* ���ڻ����������ݳ��� */
#define USART_RECEIVE_LENGTH  4096

//USART����GPIO����
void usart_gpio_config(void);

//USART���ڲ�������
void usart_param_config(uint32_t baudval);

//����gpio��usart��������
void usart_all_config(uint32_t baudval);

//usart��������
void usart_run(void);

//��������
void send_data(uint8_t data);

//�����ַ���
void send_string(char *data);

#endif 
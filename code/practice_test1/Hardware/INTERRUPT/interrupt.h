#ifndef __INTERRUPT_H
#define __INTERRUPT_H

#include "gd32f4xx.h"
#include "gd32f4xx_exti.h"
#include "systick.h"
#include "led.h"
#include "stdio.h"

//����LED8����(LED_SW=P45=PD4)
/*
KEY4=P54=D0
LED8=P20=PD5
*/

//����ʱ��
#define BSP_KEY_RCU     RCU_GPIOD
//GPIO����
#define BSP_KEY_PORT GPIOD
//���ź�
#define BSP_KEY_PIN GPIO_PIN_0

// �ⲿ�ж�0
#define BSP_KEY_EXTI_IRQN			 		EXTI0_IRQn
// �ⲿ�ж϶˿���Դ
#define BSP_KEY_EXTI_PORT_SOURCE 	EXTI_SOURCE_GPIOD
// �ⲿ�ж�������Դ
#define BSP_KEY_EXTI_PIN_SOURCE  	EXTI_SOURCE_PIN0
//EXTI�ߺ�
#define BSP_KEY_EXTI_LINE 				EXTI_0
// �ⲿ�жϺ�����
#define BSP_KEY_EXTI_IRQHANDLER   EXTI0_IRQHandler

//��ʼ������
void key_interrupt_init(void);
//��ȡ����״̬
void key_interrupt_run(void);

#endif
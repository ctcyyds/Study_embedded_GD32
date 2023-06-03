#ifndef _KEY_H
#define _KEY_H

#include "gd32f4xx.h"
#include "systick.h"
#include "led.h"
#include "stdio.h"

//����A PG9
#define KEYA_RCU RCU_GPIOG
//KEY��GPIO���ͺ�����
#define KEYA_GPIO GPIOG
//����
#define KEYA_PIN GPIO_PIN_9
//�����ж�����
#define  KEYA_NVIC_IRQ    EXTI5_9_IRQn
//EXTI�ж�Դ
#define KEYA_EXTI_SOURCE_GPIO  EXTI_SOURCE_GPIOG
#define KEYA_EXTI_SOURCE_PIN  EXTI_SOURCE_PIN9
//�ж���·
#define KEYA_EXTI_LINE EXTI_9
//�жϴ�����
#define KEYA_EXTI_IRQHandler EXTI5_9_IRQHandler

//����B PB15
#define KEYB_RCU RCU_GPIOB
//KEY��GPIO���ͺ�����
#define KEYB_GPIO GPIOB
//����
#define KEYB_PIN GPIO_PIN_15
//�����ж�����
#define  KEYB_NVIC_IRQ    EXTI10_15_IRQn
//�ж���·
#define KEYB_EXTI_LINE EXTI_15
//EXTI�ж�Դ
#define KEYB_EXTI_SOURCE_GPIO  EXTI_SOURCE_GPIOB
#define KEYB_EXTI_SOURCE_PIN  EXTI_SOURCE_PIN15
//�жϴ�����
#define KEYB_EXTI_IRQHandler EXTI10_15_IRQHandler


//����L PA0
#define KEYL_RCU RCU_GPIOA
//KEY��GPIO���ͺ�����
#define KEYL_GPIO GPIOA
//����
#define KEYL_PIN GPIO_PIN_0
//�����ж�����
#define  KEYL_NVIC_IRQ    EXTI0_IRQn
//�ж���·
#define KEYL_EXTI_LINE EXTI_0
//EXTI�ж�Դ
#define KEYL_EXTI_SOURCE_GPIO  EXTI_SOURCE_GPIOA
#define KEYL_EXTI_SOURCE_PIN  EXTI_SOURCE_PIN0
//�жϴ�����
#define KEYL_EXTI_IRQHandler EXTI0_IRQHandler


//����R PB2
#define KEYR_RCU RCU_GPIOB
//KEY��GPIO���ͺ�����
#define KEYR_GPIO GPIOB
//����
#define KEYR_PIN GPIO_PIN_2
//�����ж�����
#define  KEYR_NVIC_IRQ    EXTI2_IRQn
//�ж���·
#define KEYR_EXTI_LINE EXTI_2
//EXTI�ж�Դ
#define KEYR_EXTI_SOURCE_GPIO  EXTI_SOURCE_GPIOB
#define KEYR_EXTI_SOURCE_PIN  EXTI_SOURCE_PIN2
//�жϴ�����
#define KEYR_EXTI_IRQHandler EXTI2_IRQHandler


//����������ʼ��
void single_key_interrupt_config(rcu_periph_enum periph,uint32_t gpio_periph
																	,uint32_t pin,uint8_t nvic_irq,
																		uint8_t exti_port, uint8_t exti_pin,
																		exti_line_enum linex);

void all_key_interrupt_config();


#endif 

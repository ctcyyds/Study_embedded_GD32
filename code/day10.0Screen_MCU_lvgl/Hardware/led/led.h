#ifndef _LED_H
#define _LED_H

#include "gd32f4xx.h"
#include "systick.h"



//GPIOD��ʱ��
#define LED1_RCU RCU_GPIOE

//LED2��GPIO���ͺ�����
#define LED1_GPIO GPIOE
//����
#define LED1_PIN GPIO_PIN_3

//GPIOD��ʱ��
#define LED2_RCU RCU_GPIOD

//LED2��GPIO���ͺ�����
#define LED2_GPIO GPIOD
//����
#define LED2_PIN GPIO_PIN_7

//GPIOD��ʱ��
#define LED3_RCU RCU_GPIOG

//LED2��GPIO���ͺ�����
#define LED3_GPIO GPIOG
//����
#define LED3_PIN GPIO_PIN_3


//GPIOD��ʱ��
#define LED4_RCU RCU_GPIOA

//LED2��GPIO���ͺ�����
#define LED4_GPIO GPIOA
//����
#define LED4_PIN GPIO_PIN_5


void led_config(void);

#endif
#ifndef __LED_H__
#define __LED_H__
#include "gd32f4xx.h"
#include "systick.h"

// GPIOD��ʱ��
#define LED4_RCU RCU_GPIOA

// LED2��Ӧ��GPIO���ͺ�����
#define LED4_GPIO GPIOA
#define LED4_PIN GPIO_PIN_5
void led_config(void);
#endif
#ifndef __LED_H__
#define __LED_H__
#include "gd32f4xx.h"
#include "systick.h"

// GPIOD��ʱ��
#define LED2_RCU RCU_GPIOD

// LED2��Ӧ��GPIO���ͺ�����
#define LED2_GPIO GPIOD
#define LED2_PIN GPIO_PIN_7

void led_control(void);
#endif
#ifndef __KEY_H__
#define __KEY_H__
#include "gd32f4xx.h"
#include "systick.h"
#include "led.h"

//GPIOA��ʱ��
#define KEY_RCU RCU_GPIOA
//GPIO��Ӧ�����ź�����
#define KEY_GPIO GPIOA
#define KEY_PIN GPIO_PIN_0
void key_config(void);
void key_control(void);
#endif
#ifndef __DIGITAL_H__
#define __DIGITAL_H__
#include "gd32f4xx.h"
#include "systick.h"
//P44=PD4=DI
//P43=PD1=RCK
//P42=PD0=SCK
//����ʱ��
#define BSP_DI_RCU RCU_GPIOD
#define BSP_RCK_RCU RCU_GPIOD
#define BSP_SCK_RCU RCU_GPIOD

//��������
#define BSP_DI_PORT GPIOD
#define BSP_RCK_PORT GPIOD
#define BSP_SCK_PORT GPIOD

//��������
#define BSP_DI_PIN GPIO_PIN_4
#define BSP_RCK_PIN GPIO_PIN_1
#define BSP_SCK_PIN GPIO_PIN_0

void dig_gpio_config(void);

//�������ʾ�߼�
void dig_config(void);

void dig_run(void);

#endif
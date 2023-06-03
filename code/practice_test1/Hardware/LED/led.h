#ifndef __LED_H__
#define __LED_H__
#include "gd32f4xx.h"
#include "systick.h"
//����LED8����(LED_SW=P45=PD4)
/*
KEY4=P54=D0
LED8=P20=PD5
*/

//����LED8��ʱ���Լ�LED_SW��ʱ��
#define BSP_LED8_RCU RCU_GPIOD
#define BSP_LEDSW_RCU RCU_GPIOD

//����LEDSW&LED8������
#define BSP_LEDSW_PORT GPIOD
#define BSP_LED8_PORT GPIOD

//LEDSW&LED8������
#define BSP_LEDSW_PIN GPIO_PIN_4
#define BSP_LED8_PIN GPIO_PIN_5

//��ʼ��
void led_init(void);
//�������Ÿߵ͵�ƽ
//void led_control(void);
//����
//void led_run(void);

#endif
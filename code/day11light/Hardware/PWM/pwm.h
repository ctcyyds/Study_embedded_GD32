#ifndef __PWM_H__
#define __PWM_H__
#include "gd32f4xx.h"
#include "systick.h"

//*************************[PWM�˿ڡ���������]*************************
// PWM�˿�ʱ��
#define BSP_PWM_RCU     RCU_GPIOA

// PWM�˿�
#define BSP_PWM_PORT    GPIOA

// PWM����
#define BSP_PWM_PIN     GPIO_PIN_5

//pwm���Ÿ���
#define BSP_PWM_AF			GPIO_AF_1
//===================================================

//*************************[��ʱ������]*************************
//��ʱ��ʱ��
#define BSP_PWM_TIMER_RCU	RCU_TIMER1

//��ʱ��
#define BSP_PWM_TIMER	TIMER1

//��ʱ��ͨ��
#define BSP_PWM_CHANNEL TIMER_CH_0
//===================================================

//*************************[����ʵ��]*************************
//GPIO��ʼ��
void pwm_gpio__init(void);

//PWM����
void pwm_config(uint16_t pre,uint16_t per);

//�����ƾ���ʵ��
void pwm_led_config(void);

//��������
void pwm_run(void);
//===================================================

#endif
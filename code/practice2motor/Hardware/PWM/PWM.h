#ifndef __PWM_H
#define __PWM_H
#include "gd32f4xx.h"
#include "gd32f4xx_exti.h"
#include "systick.h"
//P01=PA0

// PWM�˿�ʱ��
#define BSP_PWM_RCU     RCU_GPIOA
// PWM�˿�
#define BSP_PWM_PORT    GPIOA
// PWM����
#define BSP_PWM_PIN     GPIO_PIN_0
//pwm���Ÿ���
#define BSP_PWM_AF			GPIO_AF_1

//��ʱ��ʱ��
#define BSP_PWM_TIMER_RCU	RCU_TIMER1
//��ʱ��
#define BSP_PWM_TIMER	TIMER1
//��ʱ��ͨ��
#define BSP_PWM_CHANNEL TIMER_CH_0

//pwm����
void pwm_config(uint16_t pre,uint16_t per);
//����
void pwm_breathing_lamp(void);
//��������
void pwm_breathing_lamp_run(void);

#endif
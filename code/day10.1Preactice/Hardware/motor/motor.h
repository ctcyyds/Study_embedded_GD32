#ifndef _PWM_H
#define _PWM_H
#include "gd32f4xx.h"
#include "systick.h"

// PWM�˿�ʱ��
#define BSP_PWM_RCU     RCU_GPIOA
// PWM�˿�
#define BSP_PWM_PORT    GPIOA
// PWM����
#define BSP_PWM_PIN     GPIO_PIN_2    
//pwm���Ÿ���
#define BSP_PWM_AF			GPIO_AF_1

//��ʱ��ʱ��
#define BSP_PWM_TIMER_RCU	RCU_TIMER1
//��ʱ��
#define BSP_PWM_TIMER	TIMER1
//��ʱ��ͨ��
#define BSP_PWM_CHANNEL TIMER_CH_2

//gpio��ʼ��
void pwm_gpio_config(void);
//��ʱ��pwm����
void pwm_config(uint16_t pre,uint16_t per);
//pwm����
void pwm_run(void);

#endif
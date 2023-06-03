#ifndef __MOTOR_H_
#define __MOTOR_H_

#include "gd32f4xx.h"
#include "systick.h"
//PA0	Timer4	CH0	AF2
//PA1 Timer4	CH1	AF2

//PWM�˿�ʱ��
#define BSP_PWM_RCU RCU_GPIOA
//PWM�˿�
#define BSP_PWM_PORT GPIOA
//PWM����
#define BSP_PWM_PIN GPIO_PIN_0 | GPIO_PIN_1
//#define BSP_PWM_PIN GPIO_PIN_1
//PWM���Ÿ���
#define BSP_PWM_AF GPIO_AF_2

//��ʱ��ʱ��
#define BSP_PWM_TIMER_RCU	RCU_TIMER4
//��ʱ��
#define BSP_PWM_TIMER	TIMER4
//��ʱ��ͨ��
#define BSP_PWM_CHANNEL0 TIMER_CH_0
#define BSP_PWM_CHANNEL1 TIMER_CH_1

void motor_init(void);

void motor_set_pwm();

void motor_test(void);
#endif
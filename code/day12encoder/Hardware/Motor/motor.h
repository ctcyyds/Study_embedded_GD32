#ifndef __MOTOR_H_
#define __MOTOR_H_
#include "gd32f4xx.h"
#include "systick.h"

/**
 PA0	Timer4	CH0	AF2
 PA1	Timer4	CH1	AF2
 PA2	Timer4	CH2	AF2
 PA3	Timer4	CH3	AF2
*/

#define BSP_PWM_RCU RCU_GPIOA
#define BSP_PWM_GPIO GPIOA
#define BSP_PWM_PIN0 GPIO_PIN_0
#define BSP_PWM_PIN1 GPIO_PIN_1
#define BSP_PWM_PIN2 GPIO_PIN_2
#define BSP_PWM_PIN3 GPIO_PIN_3
#define BSP_PWM_AF GPIO_AF_2

#define BSP_PWM_TIMER_RCU RCU_TIMER4
#define BSP_PWM_TIMER TIMER4
#define BSP_PWM_CH0 TIMER_CH_0
#define BSP_PWM_CH1 TIMER_CH_1
#define BSP_PWM_CH2 TIMER_CH_2
#define BSP_PWM_CH3 TIMER_CH_3

void motor_init(void);

void motor_set_pwm(int pwm);

void motor_test(void);

#endif
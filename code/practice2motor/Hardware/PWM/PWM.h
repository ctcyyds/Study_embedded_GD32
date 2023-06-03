#ifndef __PWM_H
#define __PWM_H
#include "gd32f4xx.h"
#include "gd32f4xx_exti.h"
#include "systick.h"
//P01=PA0

// PWM端口时钟
#define BSP_PWM_RCU     RCU_GPIOA
// PWM端口
#define BSP_PWM_PORT    GPIOA
// PWM引脚
#define BSP_PWM_PIN     GPIO_PIN_0
//pwm引脚复用
#define BSP_PWM_AF			GPIO_AF_1

//定时器时钟
#define BSP_PWM_TIMER_RCU	RCU_TIMER1
//定时器
#define BSP_PWM_TIMER	TIMER1
//定时器通道
#define BSP_PWM_CHANNEL TIMER_CH_0

//pwm配置
void pwm_config(uint16_t pre,uint16_t per);
//马达
void pwm_breathing_lamp(void);
//程序运行
void pwm_breathing_lamp_run(void);

#endif
#ifndef _PWM_H
#define _PWM_H
#include "gd32f4xx.h"
#include "systick.h"

// PWM端口时钟
#define BSP_PWM_RCU     RCU_GPIOA
// PWM端口
#define BSP_PWM_PORT    GPIOA
// PWM引脚
#define BSP_PWM_PIN     GPIO_PIN_2    
//pwm引脚复用
#define BSP_PWM_AF			GPIO_AF_1

//定时器时钟
#define BSP_PWM_TIMER_RCU	RCU_TIMER1
//定时器
#define BSP_PWM_TIMER	TIMER1
//定时器通道
#define BSP_PWM_CHANNEL TIMER_CH_2

//gpio初始化
void pwm_gpio_config(void);
//定时器pwm配置
void pwm_config(uint16_t pre,uint16_t per);
//pwm生成
void pwm_run(void);

#endif
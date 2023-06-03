#ifndef __PWM_H__
#define __PWM_H__
#include "gd32f4xx.h"
#include "systick.h"

//*************************[PWM端口、引脚配置]*************************
// PWM端口时钟
#define BSP_PWM_RCU     RCU_GPIOA

// PWM端口
#define BSP_PWM_PORT    GPIOA

// PWM引脚
#define BSP_PWM_PIN     GPIO_PIN_5

//pwm引脚复用
#define BSP_PWM_AF			GPIO_AF_1
//===================================================

//*************************[定时器配置]*************************
//定时器时钟
#define BSP_PWM_TIMER_RCU	RCU_TIMER1

//定时器
#define BSP_PWM_TIMER	TIMER1

//定时器通道
#define BSP_PWM_CHANNEL TIMER_CH_0
//===================================================

//*************************[具体实现]*************************
//GPIO初始化
void pwm_gpio__init(void);

//PWM配置
void pwm_config(uint16_t pre,uint16_t per);

//呼吸灯具体实现
void pwm_led_config(void);

//程序运行
void pwm_run(void);
//===================================================

#endif
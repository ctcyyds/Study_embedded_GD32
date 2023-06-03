#ifndef __LED_H__
#define __LED_H__
#include "gd32f4xx.h"
#include "systick.h"
//控制LED8亮灭(LED_SW=P45=PD4)
/*
KEY4=P54=D0
LED8=P20=PD5
*/

//定义LED8的时钟以及LED_SW的时钟
#define BSP_LED8_RCU RCU_GPIOD
#define BSP_LEDSW_RCU RCU_GPIOD

//定义LEDSW&LED8的类型
#define BSP_LEDSW_PORT GPIOD
#define BSP_LED8_PORT GPIOD

//LEDSW&LED8的引脚
#define BSP_LEDSW_PIN GPIO_PIN_4
#define BSP_LED8_PIN GPIO_PIN_5

//初始化
void led_init(void);
//控制引脚高低电平
//void led_control(void);
//运行
//void led_run(void);

#endif
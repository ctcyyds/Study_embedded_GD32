#ifndef _KEY_H
#define _KEY_H

#include "gd32f4xx.h"
#include "systick.h"
#include "led.h"
#include "stdio.h"

//按键A PG9
#define KEYA_RCU RCU_GPIOG
//KEY的GPIO类型和引脚
#define KEYA_GPIO GPIOG
//引脚
#define KEYA_PIN GPIO_PIN_9
//定义中断类型
#define  KEYA_NVIC_IRQ    EXTI5_9_IRQn
//EXTI中断源
#define KEYA_EXTI_SOURCE_GPIO  EXTI_SOURCE_GPIOG
#define KEYA_EXTI_SOURCE_PIN  EXTI_SOURCE_PIN9
//中断线路
#define KEYA_EXTI_LINE EXTI_9
//中断处理函数
#define KEYA_EXTI_IRQHandler EXTI5_9_IRQHandler

//按键B PB15
#define KEYB_RCU RCU_GPIOB
//KEY的GPIO类型和引脚
#define KEYB_GPIO GPIOB
//引脚
#define KEYB_PIN GPIO_PIN_15
//定义中断类型
#define  KEYB_NVIC_IRQ    EXTI10_15_IRQn
//中断线路
#define KEYB_EXTI_LINE EXTI_15
//EXTI中断源
#define KEYB_EXTI_SOURCE_GPIO  EXTI_SOURCE_GPIOB
#define KEYB_EXTI_SOURCE_PIN  EXTI_SOURCE_PIN15
//中断处理函数
#define KEYB_EXTI_IRQHandler EXTI10_15_IRQHandler


//按键L PA0
#define KEYL_RCU RCU_GPIOA
//KEY的GPIO类型和引脚
#define KEYL_GPIO GPIOA
//引脚
#define KEYL_PIN GPIO_PIN_0
//定义中断类型
#define  KEYL_NVIC_IRQ    EXTI0_IRQn
//中断线路
#define KEYL_EXTI_LINE EXTI_0
//EXTI中断源
#define KEYL_EXTI_SOURCE_GPIO  EXTI_SOURCE_GPIOA
#define KEYL_EXTI_SOURCE_PIN  EXTI_SOURCE_PIN0
//中断处理函数
#define KEYL_EXTI_IRQHandler EXTI0_IRQHandler


//按键R PB2
#define KEYR_RCU RCU_GPIOB
//KEY的GPIO类型和引脚
#define KEYR_GPIO GPIOB
//引脚
#define KEYR_PIN GPIO_PIN_2
//定义中断类型
#define  KEYR_NVIC_IRQ    EXTI2_IRQn
//中断线路
#define KEYR_EXTI_LINE EXTI_2
//EXTI中断源
#define KEYR_EXTI_SOURCE_GPIO  EXTI_SOURCE_GPIOB
#define KEYR_EXTI_SOURCE_PIN  EXTI_SOURCE_PIN2
//中断处理函数
#define KEYR_EXTI_IRQHandler EXTI2_IRQHandler


//单个按键初始化
void single_key_interrupt_config(rcu_periph_enum periph,uint32_t gpio_periph
																	,uint32_t pin,uint8_t nvic_irq,
																		uint8_t exti_port, uint8_t exti_pin,
																		exti_line_enum linex);

void all_key_interrupt_config();


#endif 

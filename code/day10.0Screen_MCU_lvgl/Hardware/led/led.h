#ifndef _LED_H
#define _LED_H

#include "gd32f4xx.h"
#include "systick.h"



//GPIOD的时钟
#define LED1_RCU RCU_GPIOE

//LED2的GPIO类型和引脚
#define LED1_GPIO GPIOE
//引脚
#define LED1_PIN GPIO_PIN_3

//GPIOD的时钟
#define LED2_RCU RCU_GPIOD

//LED2的GPIO类型和引脚
#define LED2_GPIO GPIOD
//引脚
#define LED2_PIN GPIO_PIN_7

//GPIOD的时钟
#define LED3_RCU RCU_GPIOG

//LED2的GPIO类型和引脚
#define LED3_GPIO GPIOG
//引脚
#define LED3_PIN GPIO_PIN_3


//GPIOD的时钟
#define LED4_RCU RCU_GPIOA

//LED2的GPIO类型和引脚
#define LED4_GPIO GPIOA
//引脚
#define LED4_PIN GPIO_PIN_5


void led_config(void);

#endif
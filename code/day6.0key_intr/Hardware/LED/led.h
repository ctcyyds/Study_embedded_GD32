#ifndef __LED_H__
#define __LED_H__
#include "gd32f4xx.h"
#include "systick.h"

//LED1=PE3
//LED2=PD7
//LED3=PG3
//LED4=PA5

//*************************[LED1]*************************
//GPIO时钟
#define LED1_RCU RCU_GPIOE
//GPIO类型和引脚
#define LED1_GPIO GPIOE
#define LED1_PIN GPIO_PIN_3
//===================================================

//*************************[LED2]*************************
//GPIO时钟
#define LED2_RCU RCU_GPIOD
//GPIO类型和引脚
#define LED2_GPIO GPIOD
#define LED2_PIN GPIO_PIN_7
//===================================================

//*************************[LED3]*************************
//GPIO时钟
#define LED3_RCU RCU_GPIOG
//GPIO类型和引脚
#define LED3_GPIO GPIOG
#define LED3_PIN GPIO_PIN_3
//===================================================

//*************************[LED4]*************************
//GPIO时钟
#define LED4_RCU RCU_GPIOA
//GPIO类型和引脚
#define LED4_GPIO GPIOA
#define LED4_PIN GPIO_PIN_5
//===================================================

void led_config(void);


#endif
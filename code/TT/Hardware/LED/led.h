#ifndef __LED_H__
#define __LED_H__
#include "gd32f4xx.h"
#include "systick.h"

// GPIOD的时钟
#define LED4_RCU RCU_GPIOA

// LED2对应的GPIO类型和引脚
#define LED4_GPIO GPIOA
#define LED4_PIN GPIO_PIN_5
void led_config(void);
#endif
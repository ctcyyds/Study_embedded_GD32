#ifndef __LED_H__
#define __LED_H__
#include "gd32f4xx.h"
#include "systick.h"

// GPIOD的时钟
#define LED2_RCU RCU_GPIOD

// LED2对应的GPIO类型和引脚
#define LED2_GPIO GPIOD
#define LED2_PIN GPIO_PIN_7

void led_control(void);
#endif
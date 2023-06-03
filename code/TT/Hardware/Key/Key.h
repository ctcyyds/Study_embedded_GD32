#ifndef __KEY_H__
#define __KEY_H__
#include "gd32f4xx.h"
#include "systick.h"
#include "led.h"

//GPIOA的时钟
#define KEY_RCU RCU_GPIOA
//GPIO对应的引脚和类型
#define KEY_GPIO GPIOA
#define KEY_PIN GPIO_PIN_0
void key_config(void);
void key_control(void);
#endif
#ifndef _TIMER_H
#define _TIMER_H

#include "gd32f4xx.h"
#include "systick.h"
#include "led.h"
#include "stdio.h"

void timer_config(uint16_t pre,uint16_t per);

#endif
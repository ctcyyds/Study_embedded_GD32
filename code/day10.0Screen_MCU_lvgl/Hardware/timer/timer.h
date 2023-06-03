#ifndef _TIMER_H
#define _TIMER_H

#include "gd32f4xx.h"
#include "systick.h"
#include "lvgl.h"

#define BSP_TIMER_RCU  				RCU_TIMER5            // ��ʱ��ʱ��
#define BSP_TIMER      				TIMER5                // ��ʱ��
#define BSP_TIMER_IRQ  				TIMER5_DAC_IRQn       // ��ʱ���ж�
#define BSP_TIMER_IRQHANDLER  TIMER5_DAC_IRQHandler // ��ʱ���жϷ�����



void basic_timer_config(uint16_t pre,uint16_t per); // ������ʱ������

#endif
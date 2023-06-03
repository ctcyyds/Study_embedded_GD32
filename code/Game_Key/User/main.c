

#include "gd32f4xx.h"
#include "systick.h"
#include <stdio.h>
#include "key_interrupt.h"
#include "timer.h"
#include "pwm.h"
#include "dma_m2p.h"
#include "Lcd.h"
#include "Gui.h"
#include "pic.h"

int main(void)
{
		//NVIC优先级分组,配置一个全局就行  0~3   0~3
		nvic_priority_group_set(NVIC_PRIGROUP_PRE2_SUB2);
		
    systick_config();
		//usart初始化
		usart_dma_config(115200);
	
		printf("hello\n");
		
		//led初始化
		led_config();
		//初始化按键
		all_key_interrupt_config();
		while(1) {
    }
}

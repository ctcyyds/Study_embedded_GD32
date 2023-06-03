#include "gd32f4xx.h"
#include "systick.h"
#include <stdio.h>
#include "main.h"
#include "pwm.h"
#include "usart.h"
#include "dma.h"

int main(void)
{
			//NVIC优先级分组,配置一个全局就行  0~3   0~3
			nvic_priority_group_set(NVIC_PRIGROUP_PRE2_SUB2);
    systick_config();
	//usart初始化
		usart_all_config(115200);
    
    while(1) {
    }
}

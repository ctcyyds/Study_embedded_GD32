#include "gd32f4xx.h"
#include "systick.h"
#include <stdio.h>
#include "main.h"
#include "led.h"
#include "ws2812.h"


int main(void)
{
    systick_config();
	 //NVIC优先级分组,配置一个全局就行  0~3   0~3
    nvic_priority_group_set(NVIC_PRIGROUP_PRE2_SUB2);
   ws2812_test();
    while(1) {
    }
}

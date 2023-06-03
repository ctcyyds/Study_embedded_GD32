#include "gd32f4xx.h"
#include "systick.h"
#include <stdio.h>
#include "main.h"
#include "pwm.h"
#include "dma.h"
#include "usart.h"

int main(void)
{
    systick_config();
    //NVIC优先级分组,配置一个全局就行  0~3   0~3
    nvic_priority_group_set(NVIC_PRIGROUP_PRE2_SUB2);
    //usart初始化
    usart_all_config(115200);
    dma_init();
    while(1) {
    }
}

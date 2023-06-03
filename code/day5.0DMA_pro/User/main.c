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
    //NVIC���ȼ�����,����һ��ȫ�־���  0~3   0~3
    nvic_priority_group_set(NVIC_PRIGROUP_PRE2_SUB2);
    //usart��ʼ��
    usart_all_config(115200);
    dma_init();
    while(1) {
    }
}

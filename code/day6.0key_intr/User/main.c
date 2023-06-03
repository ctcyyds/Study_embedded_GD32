#include "gd32f4xx.h"
#include "systick.h"
#include <stdio.h>
#include "main.h"
#include "key.h"
#include "usart.h"


int main(void)
{
    systick_config();
    //NVIC���ȼ�����,����һ��ȫ�־���  0~3   0~3
    nvic_priority_group_set(NVIC_PRIGROUP_PRE2_SUB2);
    //USART��ʼ��
    usart_all_config(115200);
		
		key_led_run();
    while(1) {
			
    }
}

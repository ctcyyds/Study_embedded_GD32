#include "gd32f4xx.h"
#include "systick.h"
#include <stdio.h>
#include "main.h"
#include "dma_m2p.h"
#include "battery.h"

int main(void)
{
    systick_config();
    usart_dma_config(115200);
		
    adc_run_test();
    while(1) {

    }
}

#include "gd32f4xx.h"
#include "systick.h"
#include <stdio.h>
#include "main.h"
#include "usart.h"
#include "adc.h"

int main(void)
{
	systick_config();
	usart_run();
	adc_config();
	adc_key_scan();
    while(1) {
			
    }
}

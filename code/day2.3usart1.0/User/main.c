#include "gd32f4xx.h"
#include "systick.h"
#include <stdio.h>
#include "main.h"
#include "usart.h"

int main(void)
{
	systick_config();
	usart_run();
    while(1) {
			
    }
}

#include "gd32f4xx.h"
#include "systick.h"
#include <stdio.h>
#include "main.h"
#include "led.h"
#include "usart.h"
#include "interrupt.h"

int main(void)
{
    systick_config();
    usart_run();
    while(1) {
        key_interrupt_run();
    }
}

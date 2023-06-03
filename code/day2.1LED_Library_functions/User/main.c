#include "gd32f4xx.h"
#include "systick.h"
#include <stdio.h>
#include "main.h"
#include "led.h"

int main(void)
{
	systick_config();
	led_control();
    while(1) {
			led_control();
    }
}

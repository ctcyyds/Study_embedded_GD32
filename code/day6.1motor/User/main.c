#include "gd32f4xx.h"
#include "systick.h"
#include <stdio.h>
#include "main.h"
#include "PWM.h"
#include "usart.h"

int main(void)
{
	systick_config();
	usart_run();
	pwm_breathing_lamp_run();		
    while(1) {
	
    }
}

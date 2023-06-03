#include "gd32f4xx.h"
#include "systick.h"
#include <stdio.h>
#include "main.h"
#include "led.h"
#include "pwm.h"

int main()
{
    systick_config();
    while(1) {
			pwm_run();
    }
}

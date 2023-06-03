#include "gd32f4xx.h"
#include "systick.h"
#include <stdio.h>
#include "main.h"
#include "led.h"
#include "pwm.h"

int main(void)
{
    systick_config();
    pwm_run();
    while(1) {
    }
}

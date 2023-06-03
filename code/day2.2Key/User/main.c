#include "gd32f4xx.h"
#include "systick.h"
#include <stdio.h>
#include "main.h"
#include "led.h"
#include "Key.h"

int main(void)
{
    systick_config();
	key_control();
    while(1) {
        
    }
}

#include "gd32f4xx.h"
#include "systick.h"
#include <stdio.h>
#include "main.h"
#include "digital.h"

int main(void)
{
	systick_config();
	dig_run();
    while(1) {
			
    }
}
